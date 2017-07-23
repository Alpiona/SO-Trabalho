#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINHATAM 100
#define COMANDOTAM 20
#define ARGSTAM 5

int pid;

void executaComando(char* linha){
  int qntComando=1,j=0, k=0;;
  char executavel[COMANDOTAM];
  for(int i=0; i<strlen(linha);i++){
    if (linha[i] == '|'){
      qntComando++;
    }
  }
  char comando[ARGSTAM][COMANDOTAM];
  char* aux[ARGSTAM];
  char** cmd[qntComando];
  for (int i=0; i<strlen(linha);i++){
    executavel[i-k] = linha[i];
    if (linha[i] == ' ' || i+1 == strlen(linha)){
      executavel[i-k] = 0;
      k=i+1;
      strcpy (comando[j],executavel);
      aux[j] = (char*)comando[j];
      j++;
    }
  }
  aux[j]=NULL;
  execvp(aux[0], aux);
  // char** cmd[qntComando];
}

void executaLinha(char* linha){
  linha[strlen(linha)] = 0; //Tira o newline no final
  int k=0;;
  char comando[COMANDOTAM], ch;
  for (int i=0;i<strlen(linha);i++){  //Executa cada comando até achar EOF ou &
    comando[i-k] = linha[i];
    if (linha[i] == '&' || i+1 == strlen(linha)){
      if(linha[i] == '&')
        comando[i-k] = 0;
      else
        comando[i-k+1] = 0;
      if (linha[i] == '&'){
        pid = fork();
        if (pid == 0){
          executaComando(comando);
        }
      }
      else{
        executaComando(comando);
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
