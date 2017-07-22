#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINETAM 100
#define ARGSTAM 10

int main(){
  char line[LINETAM];
  char aux[15];
  char args[ARGSTAM][15];
  char* args2[ARGSTAM];
  int j = 0, k=0;
  fprintf(stderr, "spsh.%s ", getenv("PWD"));
  fgets(line, sizeof line, stdin);
  line[strlen(line)-1] = 0;
  for (int i=0; i<strlen(line);i++){
    if (line[i] != ' '){
      aux[i-k] = line[i];
    }
    else{
      strcpy(args[j],aux);
      j++;
      k=i+1;
    }
  }
  strcpy(args[j],aux);
  for (int i=0;i<ARGSTAM;i++){
    args2[i] = args[i];
  }
  args2[j+1] = NULL;
  execvp(args2[0],args2);
}
