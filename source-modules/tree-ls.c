#include <stdio.h>
#include <dirent.h>
#include <string.h>

void print_file(const char *name, int is_dir, int indent)
{
  printf("%*c", indent*2, ' ');
  if(is_dir){
    printf("\033[1;34m%s\033[0m\n", name);
  }
  else{
    printf("%s\n", name);
  }
}

int walk(const char *path, int indent)
{
  DIR *dir = opendir(path);
  if(!dir){
    perror("opendir");
    return 1;
  }

  struct dirent *dp;

  while((dp = readdir(dir)) != NULL){
    const char *name = dp->d_name;

    //ignore files
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0){
      continue;
    }

    if(dp->d_type == DT_DIR){
      print_file(name, 1, indent);

      size_t len = strlen(path)+strlen(name)+2;
      char sub_path[len];
      snprintf(sub_path, len, "%s/%s", path, name);
      
      walk(sub_path, indent+1);
    }
    else{
      print_file(name, 0, indent);
    }
  }

  closedir(dir);
  return 0;
}

int main(int argc, char *argv[])
{
  const char *path = ".";
  if(argc > 1){
    path = argv[1];
  }

  printf("path: %s\n", path);

  int error = walk(path, 0);

  return error;
}

