/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: leitores e escritores usando semáforos */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define L 4
#define E 3

#define NTHREADS L+E

// Variaveis globais
sem_t em_e, em_l, escr, leit; ; //semaforos para sincronizar a ordem de execucao das threads
int e=0, l=0; //numero de leitores lendo





float c, f=23.7; //variaveis auxiliares

//funcao executada pelos leitores
void *Leitor (void *threadid) {
  int tid =  * (int*) threadid, i;
  while(1) {
     fprintf(stderr, "Leitor: %d quer ler!\n", tid);
     sem_wait(&leit); 
     sem_wait(&em_l);
     l++;
     if(l == 1) { 
        fprintf(stderr, "Leitor: %d verifica escrita!\n",  tid);
        sem_wait(&escr);
     }
     sem_post(&em_l);
     sem_post(&leit); 
     
     
     //executa a leitura...
     fprintf(stderr, "Leitor: %d esta lendo!\n", tid);
     for(i=1;i<1000000; i++) c=7.5*c - f;

    
     sem_wait(&em_l); 
     l--; 
     if(l==0) {
        sem_post(&escr); 
     }
     fprintf(stderr, "Leitor: %d terminou de ler!\n",  tid);
     sem_post(&em_l);
     sleep(1);
  }
  pthread_exit(NULL);
}

//funcao executada pelos escritores
void *Escritor (void *threadid) {
  int tid = *(int*) threadid, i;
  while(1) {
     fprintf(stderr, "Escritor: %d quer escrever!\n", tid);
     sem_wait(&em_e);
     e++;
     if(e==1) sem_wait(&leit);
     sem_post(&em_e); 
     sem_wait(&escr);

     //SC
     for(i=1;i<1000000; i++) c=c*7.5 -f;
     fprintf(stderr, "Escritor: %d esta escrevendo!\n", tid);

     sem_post(&escr);
     sem_wait(&em_e);
     e--;
     if(e==0) sem_post(&leit);
     fprintf(stderr, "Escritor: %d terminou de escrever!\n", tid);
     sem_post(&em_e);
     sleep(1);
  }
  pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
  pthread_t tid[L+E];
  int t, id[L+E];
  
  //inicia os semaforos
  sem_init(&leit, 0, 1);
  sem_init(&escr, 0, 1);
  sem_init(&em_e, 0, 1);
  sem_init(&em_l, 0, 1);
  

  //cria as threads
  for (t=0; t<E; t++) {
     id[t]=t;
     if (pthread_create(&tid[t], NULL, Escritor, (void *)&id[t])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  }
  for (t=0; t<L; t++) {
     id[t]=t;
     if (pthread_create(&tid[t], NULL, Leitor, (void *)&id[t])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  }  
  //--espera todas as threads terminarem
  for (t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  } 
  pthread_exit(NULL);
}
