#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define LINHATAM 100
#define COMANDOTAM 20
#define ARGSTAM 5

#define NENHUM 0
#define ENTRADA 1
#define SAIDA 2

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

void executaComando (int n, int p, char* cmd[n][p], int inOut, char* filename){
  int in = 0, out=1, fd[2];
  if (inOut == ENTRADA){
    in = open(filename, O_RDONLY);
  }
  else if (inOut == SAIDA){
    out = open(filename, O_TRUNC | O_RDWR | O_CREAT);
  }
  int i;
  for(i =0;i<n-1;i++){
    pipe(fd);
    executaPipe(in,fd[1],cmd[i]);
    close(fd[1]);
    in = fd[0];
  }
  if (in != 0)
    dup2(in,0);

  executaPipe(in, out, cmd[i]);
}

void preparaComando(char* linha){
  int qntComando=1,j=0, k=0, y=0, inOut=NENHUM;
  char executavel[COMANDOTAM];
  for(int i=0; i<strlen(linha);i++){
    if (linha[i] == '|'){
      qntComando++;
    }
  }
  char comando[ARGSTAM*qntComando][COMANDOTAM];
  char filename[50];
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
      if (linha[i+1] == '<' || linha[i+1] == '>'){
        if(linha[i+1] == '<'){
          inOut = ENTRADA;
        }
        else{
          inOut = SAIDA;
        }
        aux[j] = NULL;
        cmd[y][j] = NULL;
        k = 0;
        i = i+3;
        while(i<strlen(linha)-1){
          filename[k] = linha[i];
          k++;
          i++;
        }
        filename[k] = 0;
        break;
      }
      k=i+1;
    }
  }
  aux[j]=NULL;
  cmd[y][j]=NULL;
  executaComando(qntComando, ARGSTAM, cmd, inOut, filename);
}

void executaLinha(char* linha){
  linha[strlen(linha)] = 0;
  int k=0, pid;
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
  fprintf(stderr, "spsh.%s ", getenv("PWD"));
  fgets(linha, sizeof linha, stdin);
  executaLinha(linha);
}
