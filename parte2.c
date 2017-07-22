#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define DIR 1
#define ESQ 2
#define GOR 3

int direcao, qnt, gorilasEsperando;
sem_t atravessando;
sem_t gorilas;

void direita (int arg){
  sleep(arg*5);
  while(1){
    sem_getvalue(&gorilas,&gorilasEsperando);
    if (gorilasEsperando != 0 && direcao == ESQ){
      sem_post(&atravessando);
      // printf("Macaco %d está atravessando para esquerda\n", arg);
      printf("Macaco %d <<<<<<<<<<<<<<<<<<<\n", arg);
      sleep(2);
      // printf("Macaco %d chegou do lado esquerdo\n", arg);
      printf("Macaco %d CHEGOU\n", arg);
      sem_wait(&atravessando);
      esquerda(arg);
    }
  }
}

void esquerda (int arg){
  sleep(arg*5);
  while(1){
    sem_getvalue(&gorilas,&gorilasEsperando);
    if (gorilasEsperando != 0 && direcao == DIR){
      sem_post(&atravessando);
      // printf("Macaco %d está atravessando para direita\n", arg);
      printf("Macaco %d >>>>>>>>>>>>>>>>\n", arg);
      sleep(2);
      // printf("Macaco %d chegou do lado direito\n", arg);
      printf("Macaco %d CHEGOU\n", arg);
      sem_wait(&atravessando);
      direita(arg);
    }
  }
}

void exclusivo (int arg){
  while(1){
    sleep(15);
    sem_wait(&gorilas);
    while(1){
      sem_getvalue(&atravessando,&qnt);
      if (qnt == 0){
        direcao = GOR;
        printf("Gorila %d atravessando\n", arg);
        sleep(3);
        printf("Gorila %d chegou\n", arg);
        break;
      }
    }
    sem_post(&gorilas);
  }
}

int main (){
  direcao = DIR;
  sem_init (&atravessando, 0, 0);
  sem_init (&gorilas, 0, 1);
  pthread_t macaco[10];
  pthread_t gorila[2];
  int i =0;
  while (i<5){
    pthread_create(&macaco[i], NULL, (void*)direita, i+1);
    i++;
  }
  while (i<10){
    pthread_create(&macaco[i], NULL, (void*)esquerda, i+1);
    i++;
  }
  pthread_create(&gorila[0], NULL, (void*)exclusivo, 1);
  pthread_create(&gorila[1], NULL, (void*)exclusivo, 2);
  while(1){
    sem_getvalue(&atravessando,&qnt);
    if (qnt == 0){
      if (direcao == DIR){
        direcao = ESQ;
      }
      else {
        direcao = DIR;
      }
      sleep(1);
    }
  }
}
