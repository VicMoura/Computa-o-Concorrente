/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 9 */
/* Aluno: VICTOR DA SILVA MOURA. DRE: 120124419 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  5

/* Variaveis globais */
sem_t um, dois;



/* Thread 5 */
void *Cinco (void *t) {

  //SEGURAR UM POUCO A THREAD
  int boba1, boba2;
  
  /* faz alguma coisa pra gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;		
	
  
  printf("Seja bem-vindo\n"); 
  
  
  //SINAL PARA AS THREAS 2, 3 E 4
  sem_post(&um);
  sem_post(&um);
  sem_post(&um);
  
  pthread_exit(NULL);		
}

/* Thread 4 */
void *Quatro (void *t) {
  
  //ESPERA O SINAL DA THREAD 1
  sem_wait(&um);
  
  //IMPRIME DPS Q FOI LIBERADO
  printf("Aceita um copo d'água? \n"); 
  
  
  //SINAL PARA THREAD1 
  sem_post(&dois);
  
  pthread_exit(NULL);
}



/* Thread 3 */
void *Tres (void *t) {
  
  //ESPERA O SINAL DA THREAD 1
  sem_wait(&um);
  
  //IMPRIME DPS Q FOI LIBERADO
  printf("Sente-se por favor. \n"); 
  
  //SINAL PARA THREAD1 
  sem_post(&dois);
  pthread_exit(NULL);
}




/* Thread 2 */
void *Dois (void *t) {
  
  
  //ESPERA O SINAL DA THREAD 1
  sem_wait(&um);
  
  //IMPRIME DPS Q FOI LIBERADO
  printf("Fique a vontade. \n"); 
  
  
  //SINAL PARA THREAD1 
  sem_post(&dois);
  
  pthread_exit(NULL);
}



/* Thread 1 */
void *Um (void *t) {


  //CONSUMIR OS TRÊS SINAIS DA THREAD 2, 3 E 4 	
  for(int i =0; i<3; i++){
      sem_wait(&dois);
  }


  // FOI DESBLOQUEADA, LOGO IMPRIME A ÚLTIMA FRASE 
  printf("Volte sempre!\n"); 
  
 
  pthread_exit(NULL);
}



/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];
  
   //inicia o semaforo 
  sem_init(&um, 0, 0);
  sem_init(&dois, 0, 0);
  
  

  /* Cria as threads */
  pthread_create(&threads[4], NULL, Cinco, NULL);
  pthread_create(&threads[3], NULL, Quatro, NULL);
  pthread_create(&threads[2], NULL, Tres, NULL);
  pthread_create(&threads[1], NULL, Dois, NULL);
  pthread_create(&threads[0], NULL, Um, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  pthread_exit(NULL);
  return 0;
}
