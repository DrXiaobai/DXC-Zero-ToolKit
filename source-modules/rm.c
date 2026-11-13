#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int walk_rm(const char *path)
{
  struct stat st;
  if(lstat(path, &st)){
    perror(path);
    return 1;
  }
  if(!S_ISDIR(st.st_mode)){
    if(unlink(path)){
      perror(path);
      return 1;
    }
  }

  DIR *dir = opendir(path);
  if(!dir){
    perror(path);
    return 1;
  }

  struct dirent *entry;
  while((entry = readdir(dir))!=NULL){
    const char *name = entry->d_name;
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;
    size_t path_len = strlen(path)+strlen(name)+2;
    char new_path[path_len];
    snprintf(new_path, path_len, "%s/%s", path, name);

    struct stat st_child;
    if(lstat(new_path, &st_child)){
      perror(new_path);
      return 1;
    }

    if(!S_ISDIR(st_child.st_mode)){
      if(unlink(new_path)){
        perror(new_path);
        return 1;
      }
    }
    else{
      walk_rm(new_path);
    }
  }

  if(rmdir(path)){
    perror(path);
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Useage: rm [path]...\n");
    return 0;
  }

  for(int i=1;i<argc;i++){
    const char *path = argv[i];
    if(walk_rm(path)) return 1;
  }

  return 0;
}

