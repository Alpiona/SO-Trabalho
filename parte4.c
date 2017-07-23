#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int fila[15], nEspaco, nMatagal;
sem_t espacoBarco, matagal, barcoSaiu, barcoAtravessou;


void jovem (int arg){
  printf("Jovem %d vai tentar pegar o barco...\n", arg+1);
  if(nEspaco == 0){
    int aux = 0;
    for (int i=0;i<15;i++){
      if (fila[i] > aux){
        aux = fila[i];
      }
    }
    fila[arg] = aux+1;
    printf("Jovem %d entrou na fila, tem %d na frente dele\n",arg+1, fila[arg]);
    while (fila[arg] > 3){

    }
  }
  sem_wait(&espacoBarco);
  printf("Jovem %d entrou no barco e espera que este saia\n", arg+1);
  sem_wait(&barcoSaiu);
  printf("Jovem %d sai com o barco.\n", arg+1);
  sem_wait(&barcoAtravessou);
  sem_wait(&matagal);
  printf("Jovem %d atravessando matagal...\n", arg+1);
  sleep(1);
  printf("Jovem %d entrou no show...\n",arg+1);
  nMatagal--;
  sem_post(&matagal);
}

void barco (void* arg){
  while(1){
    while (nEspaco != 0){
      sem_getvalue(&espacoBarco, &nEspaco);
    }
    printf("Barqueiro recolhe ajuda para a comunidade e sai com o barco...\n");
    for(int i=0;i<3;i++){
      sem_post(&barcoSaiu);
    }
    sleep(3);
    printf("Barqueiro chega na outra margem e libera os 3 passageiros..\n");
    for(int i=0;i<3;i++){
      sem_post(&barcoAtravessou);
    }
    printf("Barqueiro espera todos atravessarem o matagal...\n");
    while(nMatagal != 0){

    }
    printf("Tudo certo! Barqueiro retorna para fazer outra viagem...\n");
    nMatagal = 3;
    sleep(2);
    for(int i=0;i<15;i++){
      fila[i] -=3;
    }
    for(int i=0;i<3;i++){
      sem_post(&espacoBarco);
    }
    nEspaco=3;
  }
}

int main (){
  for (int i =0; i<15;i++){
    fila[i] = 0;
  }
  nEspaco = 3;
  nMatagal = 3;
  pthread_t jovens[15], barqueiro;
  sem_init(&espacoBarco, 0, 3);
  sem_init(&matagal, 0, 1);
  sem_init(&barcoSaiu, 0, 0);
  sem_init(&barcoAtravessou, 0, 0);
  pthread_create(&barqueiro, NULL, (void*)barco, NULL);
  for (int i =0; i<15; i++){
    sleep(4);
    pthread_create(&jovens[i], NULL, (void*)jovem, i);
  }
  while(1){

  }
}
