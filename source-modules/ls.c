#define _XOPEN_SOURCE 500
#define _GNU_SOURCE

#include <stdio.h>
#include <ftw.h>
#include <stdlib.h>

int print_name(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
  if(ftwbuf->level>1){
    return FTW_CONTINUE;
  }
  else{
    if(typeflag == FTW_D){
      printf("\033[1;34m%s\n", fpath);
    }
    else{
      printf("\033[0;37m%s\n", fpath);
    }
    return 0;
  }
}

void print_dir_contents(char *dir)
{
  nftw(dir, print_name, 1, 0);
}

int main(int argc, char *argv[])
{
  if(argc == 2){
    print_dir_contents(argv[1]);
  }
  else if(argc == 1){
    //char *curr_dir = getenv("PWD");
    char *curr_dir = ".";
    //printf("%s\n", curr_dir);
    print_dir_contents(curr_dir);
  }

  return 0;
}

