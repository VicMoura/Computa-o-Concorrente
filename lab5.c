/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Aluno: VICTOR DA SILVA MOURA. DRE: 120124419 */

/***** ATIVIDADE  4****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5

/* Variaveis globais */
int auxiliar, auxiliar2 = 0; // PARA AUXILIAR NA ORDEM DAS THREADS 
pthread_mutex_t x_mutex;
pthread_cond_t x_cond, x_cond2; // CONDIÇÃO DA THREAD UM E CINCO 




/* Thread 5 */
void *Cinco (void *t) {

  //SEGURAR UM POUCO A THREAD
  int boba1, boba2;
  
  /* faz alguma coisa pra gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;		
	
  pthread_mutex_lock(&x_mutex);
  
  printf("Seja bem-vindo\n"); 
  
  //INCREMENTA PARA AUXILIAR NA CONDIÇÃO DAS OUTRAS THREADS 
  auxiliar++; 
  
  //LIBERA TODAS THREADS QUE ESTEJAM PRESAS 
  pthread_cond_broadcast(&x_cond);
  
  
  pthread_mutex_unlock(&x_mutex); 
  
  pthread_exit(NULL);
}

/* Thread 4 */
void *Quatro (void *t) {
  pthread_mutex_lock(&x_mutex);
  
  // CASO A THREAD 5 NÃO TENHA IMPRIMIDO A PRIMEIRA FRASE, IRÁ BLOQUEAR. 
  if(auxiliar == 0){
  	pthread_cond_wait(&x_cond, &x_mutex);
  }
  
  //IMPRIME DPS Q FOI LIBERADO
  printf("Aceita um copo d'água? \n"); 
  
  //INCREMENTA NA AUXILIAR2 PARA AJUDAR NA CONDIÇÃO DA PRIMEIRA THREAD 
  auxiliar2++; 
  
  //VERIFICA SE AUXILIAR2 == 3, SE SIM DA O SINAL PARA A THREAD UM PARA IMPRIMIR A ÚLTIMA FRASE
  // OU SEJA, SIGNIFICA Q TODAS AS THREADS DO MEIO IMPRIMIRAM SUAS MENSAGENS 
  if(auxiliar2 == 3){
  	pthread_cond_signal(&x_cond2); //DESBLOQUEIA A  THREAD  UM
  }
  
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}



/* Thread 3 */
void *Tres (void *t) {
  pthread_mutex_lock(&x_mutex);
  
  // CASO A THREAD 5 NÃO TENHA IMPRIMIDO A PRIMEIRA FRASE, IRÁ BLOQUEAR. 
  if(auxiliar == 0){
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  
  //IMPRIME DPS Q FOI LIBERADO
  printf("Sente-se por favor. \n"); 
  
  //INCREMENTA NA AUXILIAR2 PARA AJUDAR NA CONDIÇÃO DA PRIMEIRA THREAD 
  auxiliar2++; 
  
  
  //VERIFICA SE AUXILIAR2 == 3, SE SIM DA O SINAL PARA A THREAD UM PARA IMPRIMIR A ÚLTIMA FRASE
  // OU SEJA, SIGNIFICA Q TODAS AS THREADS DO MEIO IMPRIMIRAM SUAS MENSAGENS 
  if(auxiliar2 == 3){
  	pthread_cond_signal(&x_cond2); //DESBLOQUEIA A  THREAD  UM
  }
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}




/* Thread 2 */
void *Dois (void *t) {
  pthread_mutex_lock(&x_mutex);
  
   // CASO A THREAD 5 NÃO TENHA IMPRIMIDO A PRIMEIRA FRASE, IRÁ BLOQUEAR. 
  if(auxiliar == 0){
  	pthread_cond_wait(&x_cond, &x_mutex);
  }
  
  //IMPRIME DPS Q FOI LIBERADO
  printf("Fique a vontade. \n"); 
  
  
  //INCREMENTA NA AUXILIAR2 PARA AJUDAR NA CONDIÇÃO DA PRIMEIRA THREAD 
  auxiliar2++; 
  
  
  //VERIFICA SE AUXILIAR2 == 3, SE SIM DA O SINAL PARA A THREAD UM PARA IMPRIMIR A ÚLTIMA FRASE
  // OU SEJA, SIGNIFICA Q TODAS AS THREADS DO MEIO IMPRIMIRAM SUAS MENSAGENS 
  if(auxiliar2 == 3){
  	pthread_cond_signal(&x_cond2);//DESBLOQUEIA A  THREAD  UM
  }
  
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}



/* Thread 1 */
void *Um (void *t) {

	
  pthread_mutex_lock(&x_mutex);
  
  
  //SE AS THREADS DO MEIO AINDA N IMPRIMIRAM, SE BLOQUEIA 
  if(auxiliar2 != 3){
  	pthread_cond_wait(&x_cond2, &x_mutex);
  }
  
  // FOI DESBLOQUEADA, LOGO IMPRIME A ÚLTIMA FRASE 
  printf("Volte sempre!"); 
  
  
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}



/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

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

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  pthread_cond_destroy(&x_cond2);
  return 0;
}
