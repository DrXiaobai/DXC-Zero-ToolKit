#include <stdio.h>

int main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Useage: touch [path] ...\n");
    return 0;
  }

  for(int i=1;i<argc;i++){
    const char *path = argv[i];
    FILE *fp = fopen(path, "w");
    if(!fp){
      perror(path);
      return 1;
    }
    fclose(fp);
  }

  return 0;
}

