#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
  if(argc<2){fprintf(stderr, "Usage: %s <file_path>", argv[0]); return 1;}
  
  const char *p_file_path = argv[1];
  FILE *p_file = fopen(p_file_path, "rb");

  if(p_file == NULL){perror("Error opening file"); return 1;}

  unsigned char p_buffer[16];
  size_t bytes_read;
  unsigned long offset = 0;

  printf("Offset    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ASCII\n");
  printf("---------------------------------------------------------------\n");

  while((bytes_read = fread(p_buffer, 1 ,sizeof(p_buffer), p_file)) > 0){
    printf("%08lx", offset);

    for(size_t i=0;i<16;i++){
      if(i<bytes_read){
        printf("%02x", p_buffer[i]);
      }
      else{
        printf("   ");
      }

      if(i==7){
        printf(" ");
      }
    }

    printf(" ");

    for(size_t i=0;i<bytes_read;i++){
      if(isprint(p_buffer[i])){
        printf("%c", p_buffer[i]);
      }
      else{
        printf(".");
      }
    }

    printf("\n");
    offset += bytes_read;
  }

  fclose(p_file);

  return 0;
}

