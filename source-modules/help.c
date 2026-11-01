#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char p_buf[PATH_MAX+32];
    
    ssize_t len = readlink("/proc/self/exe", p_buf, sizeof(p_buf) - 1);
    if (len == -1){
        perror("readlink failed");
        return 1;
    }
    p_buf[len] = '\0';

    char *p_last_slash = strrchr(p_buf, '/');
    if (p_last_slash != NULL){
        *p_last_slash = '\0';
    }

    char *p_prev_slash = strrchr(p_buf, '/');
    if (p_prev_slash != NULL){
        *p_prev_slash = '\0';
    }

    char p_help_path[PATH_MAX];
    snprintf(p_help_path, sizeof(p_help_path), "%s/data/help.data", p_buf);

    FILE *p_file = fopen(p_help_path, "r");
    if (p_file == NULL){
        fprintf(stderr, "Error: Cannot open help data file at %s\n", p_help_path);
        return 1;
    }

    char p_line[256];
    while (fgets(p_line, sizeof(p_line), p_file)){
        printf("%s", p_line);
    }

    fclose(p_file);
    return 0;
}
