#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  FILE *fd[argc];
  if(argc<=1){
    fd[0] = stdin;
    int c;
    while((c=fgetc(fd[0])) != EOF){
      putc(c, stdout);
    }
  }
  else{
    for(int i=0;i<argc-1;i++){
      fd[i] = fopen(argv[i+1], "r");
      if(fd[i]==NULL){
        printf("Invalid file provided at index i=%d\n", i+1);
        exit(-1);
      }
    }
    int c;

    for(int i=0;i<argc-1;i++){
      while((c=fgetc(fd[i])) != EOF){
        putc(c, stdout);
      }
    }
  }

  return 0;
}

