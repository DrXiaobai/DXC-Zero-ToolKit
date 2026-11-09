#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <sstream>
#include "./header/defines.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>

void find_command(const std::vector<std::string> &args, const std::string &data_file, const std::string &run_path);

std::string resolve_path(const std::string &run_path, const std::string &exec_path)
{
  if(!exec_path.empty() && exec_path[0]=='/'){
    return exec_path;
  }
  if(exec_path.rfind("./", 0)==0){
    return run_path+"/"+exec_path.substr(2);
  }

  return run_path+"/"+exec_path;
}

std::string get_executable_dir()
{
  char p_buf[PATH_MAX];
  ssize_t len = readlink("/proc/self/exe", p_buf, sizeof(p_buf) - 1);
  if(len != -1){
    p_buf[len] = '\0';
    std::string path(p_buf);
    size_t pos = path.find_last_of('/');
    if(pos != std::string::npos){
      return path.substr(0, pos);
    }
  }

  return ".";
}

int do_command(const std::vector<std::string> &args)
{
  if(args.empty()) return -1;

  std::vector<char*>c_args;
  for(size_t i=0;i<args.size();++i){
    c_args.push_back(const_cast<char*>(args[i].c_str()));
  }

  c_args.push_back(0);

  pid_t pid = fork();
  if(pid == -1){
    perror("fork failed");
    return -1;
  }
  else if(pid == 0){
    execvp(c_args[0], c_args.data());
    perror("execvp failed");
    _exit(127);
  }
  else{
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status): -1;
  }
}

bool file_exists(const std::string &path)
{
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

int init_command_system(const std::string &data_path)
{
  std::ifstream file(data_path);

  if(!file.is_open()){
    std::cerr<<"[Init Error] Command data file not found:"<<data_path<<"\n";
    return -1;
  }

  std::string line;
  int line_num = 0;
  int error_count = 0;

  std::cout<<"[Init Info] Initializing command system......\n";

  while(std::getline(file, line)){
    line_num++;
    if(line.empty()||line[0]=='#') continue;
    std::istringstream iss(line);
    std::string cmd_name;
    std::string exec_path;

    iss>>cmd_name>>exec_path;

    if(exec_path.empty()){
      std::cerr<<" -> [Warning] Line "<<line_num
               <<": Command '"<<cmd_name<<"' is missing an executable path.\n";
      error_count++;
      continue;
    }
    if(cmd_name.empty()){
      std::cerr<<" -> [Warning] Line"<<line_num
               <<": Missing command name(key).\n";
      error_count++;
      continue;
    }
    if(!exec_path.empty() && !file_exists(exec_path)){
      std::cerr<<" -> [Warning] Line"<<line_num
               <<": Command '"<<cmd_name<<"' points to a non-existent file"<<"('"<<exec_path<<"').\n";
      error_count++;
      continue;
    }

    std::cout<<" -> [OK] Registered: "<<cmd_name<<"\n";
  }
  file.close();
  
  std::cerr<<"[Info] Init finished with"<<error_count<<" configuration error(s).\n";

  return 0;
}

int main(void)
{
  const std::string path = get_executable_dir()+" -> ";

  std::string command = "";
  int running = 1;
  
  std::string run_path = get_executable_dir();
  std::string input = "";
  std::string data_file = run_path+"/data/command.data";
  if(init_command_system(run_path+"/data/command.data")){return 1;}
  while(running){
    std::cout<<path;
    
    if(!std::getline(std::cin, input)) break;
    if(input.empty()) continue;
    if(input == "exit") break;
    if(input == "clear"){
      std::cout<<"\033[2J\033[H"<<std::flush;
      continue;
    }
    if(input == "version" || input == "ver"){std::cout<<VERSION<<"\n"; continue;}

    std::vector<std::string> args;
    std::stringstream ss(input);
    std::string word;
    while(ss>>word){
      args.push_back(word);
    }
    if(!args.empty()){
      find_command(args, data_file, run_path);
    }
  }

  return 0;
}

void find_command(const std::vector<std::string> &args, const std::string &data_file, const std::string &run_path)
{
  if(args.empty()) return;
  
  std::ifstream file(data_file);

  if(!file.is_open()){
    std::cerr<<"[Error] Cannot open command data file: "<<data_file<<"\n";
    return;
  }
  std::string line;
  bool found = 0;
  
  while(std::getline(file, line)){
    if(line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string cmd_name;
    std::string exec_path;
    iss>>cmd_name>>exec_path;

    if(cmd_name == args[0]){
      found = 1;
      
      std::vector<std::string> exec_args;
      
      /*if(exec_path.rfind("./", 0) == 0){
        exec_args.push_back(run_path+"/"+exec_path.substr(2));
      }
      else{
        exec_args.push_back(exec_path);
      }*/

      exec_args.push_back(resolve_path(run_path, exec_path));

      for(size_t i=1;i<args.size();++i){
        exec_args.push_back(args[i]);
      }

      int ret = do_command(exec_args);

      if(ret != 0){
        std::cerr<<"[Error] Command failed with exit code: "<<ret<<"\n";
      }
      break;
    }
  }

  file.close();

  if(!found){
    std::cout<<"Unknown command: "<<args[0]<<"\n";
  }
}

