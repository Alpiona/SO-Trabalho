#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINHATAM 100
#define COMANDOTAM 20
#define ARGSTAM 5

int pid;

void executaPipe(int in, int out, char* cmd[]){
  int pid;
  if ((pid = fork())== 0){
    if (in != 0) {
        dup2 (in, 0);
        close (in);
      }

    if (out != 1) {
        dup2 (out, 1);
        close (out);
    }
    execvp (cmd[0],cmd);
  }
}

void executaComando (int n, int p, char* cmd[n][p]){
  int in = 0, fd[2];
  int i;
  for(i =0;i<n-1;i++){
    pipe(fd);
    executaPipe(in,fd[1],cmd[i]);
    close(fd[1]);
    in = fd[0];
  }
  if (in != 0)
    dup2(in,0);
  execvp(cmd[i][0],cmd[i]);
}

void preparaComando(char* linha){
  int qntComando=1,j=0, k=0, y=0;
  char executavel[COMANDOTAM];
  for(int i=0; i<strlen(linha);i++){
    if (linha[i] == '|'){
      qntComando++;
    }
  }
  char comando[ARGSTAM*qntComando][COMANDOTAM];
  char* aux[ARGSTAM];
  char* cmd[qntComando][ARGSTAM];
  for (int i=0; i<strlen(linha);i++){
    executavel[i-k] = linha[i];
    if (linha[i] == ' ' || i+1 == strlen(linha)){
      executavel[i-k] = 0;
      strcpy (comando[j+(y*qntComando)],executavel);
      aux[j] = (char*)comando[j+(y*qntComando)];
      cmd[y][j] = aux[j];
      j++;
      if(linha[i+1]== '|'){
        aux[j]=NULL;
        cmd[y][j] = NULL;
        i+=2;
        j=0;
        y++;
      }
      k=i+1;
    }
  }
  aux[j]=NULL;
  cmd[y][j]=NULL;
  executaComando(qntComando, ARGSTAM, cmd);
}

void executaLinha(char* linha){
  linha[strlen(linha)] = 0;
  int k=0;;
  char comando[COMANDOTAM], ch;
  for (int i=0;i<strlen(linha);i++){
    comando[i-k] = linha[i];
    if (linha[i] == '&' || i+1 == strlen(linha)){
      if(linha[i] == '&')
        comando[i-k] = 0;
      else
        comando[i-k+1] = 0;
      if (linha[i] == '&'){
        pid = fork();
        if (pid == 0){
          preparaComando(comando);
        }
      }
      else{
        preparaComando(comando);
      }
      for (k=0; k<COMANDOTAM;k++){
        comando[k]=0;
      }
      i++;
      k = i+1;
    }
  }
}

int main(){
  char linha[LINHATAM];
  // fprintf(stderr, "spsh.%s ", getenv("PWD"));
  // fgets(linha, sizeof linha, stdin);
  strcpy(linha,"ps -aux | grep kworker\n");
  executaLinha(linha);
}
