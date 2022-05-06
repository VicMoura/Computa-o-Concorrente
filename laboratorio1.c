/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno: Victor da Silva Moura DRE:120124419 */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: LABORATÓRIO 1  usando threads em C e a funcao que espera as threads terminarem */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define NTHREADS 2
#define TAMANHO 10000  //Criando um tamanho do vetor

int vetor[TAMANHO]; //Definindo o vetor

//funcao executada pelas threads
void * tarefa (void *arg) {
   int ident = * (int *) arg; 
    
    //Aqui dividimos as tarefas, primeira thread fica com metade do vetor. 
    if (ident == 1){
        for(int i = 0; i<TAMANHO/2; i++){
            vetor[i] = vetor[i]*vetor[i];  //Pegamos seu próprio elemento, e elevamos ao quadrado. 
        }    
    }
    
    //Outra thread fica com a outra metade do vetor
    if (ident == 2){
        for(int i = TAMANHO/2; i<TAMANHO; i++){
            vetor[i] = vetor[i]*vetor[i]; //Pegamos seu próprio elemento, e elevamos ao quadrado. 
        }    
    }
            
  pthread_exit(NULL);
}

//funcao principal do programa
int main(void) {
  pthread_t tid[NTHREADS]; //identificadores das threads no sistema
  int ident[NTHREADS]; //variavel auxiliar
    
    
  //Colocando os elementos no vetor, para testar o cálculo   
  for (int i = 0; i<TAMANHO; i++) 
         vetor[i] =  i;  //Neste caso escolhi colocar em ordem crescente, para facilitar a visualização da resposta final. 
  
    
  //Criando as threads
  for(int i = 1; i<=NTHREADS; i++){
      ident[i-1] = i;
      if(pthread_create(&tid[i-1], NULL, tarefa, (void *)&ident[i-1]))
          printf("ERRO -- pthread_create\n");
  }
    
    
  //Espera todas as threads terminarem
  for (int i = 0; i<NTHREADS; i++) {
    if (pthread_join(tid[i], NULL)) 
         printf("--ERRO: pthread_join() \n"); 
  }
    
    
  //Imprimindo o vetor
  for (int i = 0; i<TAMANHO; i++) 
         printf("%d \n", vetor[i]); 
  printf("\n"); 
    
  printf("--Thread principal terminou\n");
  return  0;
}
