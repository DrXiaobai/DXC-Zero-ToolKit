#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <limits>

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

int image_view(std::string &image_path)
{
  /*pid_t pid = fork();
  if(pid == 0){
    char *args[] = { const_cast<char*>("./modules/image_view"), const_cast<char*>(image_path.c_str()), NULL };
    execvp(args[0], args);
    exit(1);
  }
  else if(pid > 0){
    waitpid(pid, NULL, 0);
  }
  
  istd::string cmd="./modules/image_view " + image_path;
  system(cmd.c_str());*/
  if(!image_path.empty() && image_path[0] == '~'){
    const char *p_home = getenv("HOME");
    if(p_home != 0){
      image_path = std::string(p_home) + image_path.substr(1);
    }
  }

  return do_command({"./modules/image_view", image_path});
}

int main(void)
{
  std::string path_of_image = "";
  std::cin>>path_of_image;
  image_view(path_of_image);

  return 0;
}

