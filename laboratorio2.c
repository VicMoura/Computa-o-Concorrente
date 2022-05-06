/* Multiplicacao de matrizes (considerando matrizes quadradas)
	VICTOR DA SILVA MOURA. DRE: 120124419 LABORATORIO 2 
 */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h" 


float *matA; //matriz A de entrada
float *matB; //matriz B de entrada
float *saidaSequencial; // resultado utilizando o modo sequencial
float *saidaConcorrente; // resultado utilizando o modo concorrente
int nthreads; //numero de threads


typedef struct{
    int id; //identificador do elemento que a thread irá processar
    int dim; //dimensao das estruturas de entrada
} tArgs;



//funcao que as threads executarão
void * tarefa(void *arg){
    tArgs *args = (tArgs*) arg;
    float aux = 0;
    for(int i = args->id; i<args->dim; i+=nthreads){
       for(int j = 0; j < (args->dim); j++) {
         saidaConcorrente[(i)*(args->dim)+j] = 0;
       for(int x = 0; x < (args->dim); x++) {
          aux +=  matA[(i)*(args->dim)+x] * matB[x*(args->dim)+j];
          }
         saidaConcorrente[(i)*(args->dim)+j] = aux;
         aux = 0;
       }
     }
   
   pthread_exit(NULL);
}




//funcao do calculo sequencial
void calculosequencial(int dim){
  float aux = 0;
  for(int i = 0; i < dim; i++) {
	for(int j = 0; j < dim; j++) {
		saidaSequencial[i*dim+j] = 0;
	for(int x = 0; x < dim; x++) {
		aux +=  matA[i*dim+x] * matB[x*dim+j];
	}
	saidaSequencial[i*dim+j] = aux;
	aux = 0;
	}
	}
}



//funcao de comparaçao 
void comparacao(int dim){
	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
		    if(saidaSequencial[i*dim+j] != saidaConcorrente[i*dim+j]) {
		    	printf("AS MATRIZES NAO SAO IGUAIS!, LOGO TEM ALGUM ERRO NA RESOLUÇAO\n"); 
		    }
		}	
	}
	printf("SAO IGUAIS!, A RESOLUÇAO FUNCIONA!\n"); 
}


//funcao principal
int main(int argc, char* argv[]) {
   srand(time(NULL)); 
   
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args;  //identificadores locais das threads e dimensão
   int dim; //dimensao da matriz de entrada
 
   double inicio, fim, delta1, delta2; 

   //leitura e avaliação dos parametros de entrada
    if(argc<3){
        printf("Digite: %s <dimensao da matriz> e <numeros de threads> \n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads>dim){
        nthreads = dim;
    }

   //alocacao de memoria para as estruturas de dados
   matA = (float *) malloc(sizeof(float) * dim * dim);
   if (matA == NULL) {printf("ERRO--malloc\n"); return 2;}
   matB = (float *) malloc(sizeof(float) * dim * dim);
   if (matB == NULL) {printf("ERRO--malloc\n"); return 2;}
   saidaSequencial = (float *) malloc(sizeof(float) * dim * dim);
   if (saidaSequencial == NULL) {printf("ERRO--malloc\n"); return 2;}
   saidaConcorrente = (float *) malloc(sizeof(float) * dim * dim);
   if ( saidaConcorrente == NULL) {printf("ERRO--malloc\n"); return 2;}
   
   

   //inicializacao das estruturas de dados de entrada e saida
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         matA[i*dim+j] = rand() % 100;    //Gerando valores aleatorios
         matB[i*dim+j] = rand() % 100;
      }
   }
   
   //CALCULANDO USANDO A FORMA SEQUENCIAL 
   
   GET_TIME(inicio); 
   calculosequencial(dim);
   GET_TIME(fim); 
   delta1 = fim - inicio;
   
   
 
  //Multiplicacao das matrizes
  //alocacao das estruturas
  tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
  if(tid == NULL) {puts("ERRO -- malloc"); return 2;}
 
  args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
  if(args == NULL) {puts("ERRO -- malloc"); return 2;}
 
 
  GET_TIME(inicio); 
  //criação das threads
  for(int i = 0; i < nthreads; i++) {
    (args+i)->id = i;
    (args+i)->dim = dim;
    if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
        puts("ERRO--pthread_create");
        return 3;
    }
     
  }
 
  //espera pelo termino das threads
   for(int i = 0; i < nthreads; i++) {
       pthread_join(*(tid+i), NULL);
   }
 
   GET_TIME(fim); 	
   delta2 = fim - inicio; 
   
   
   //Comparaçao 
   comparacao(dim); 
   
   printf("O tempo utilizando a forma sequencial, foi de : %lf\n", delta1); 
   printf("O tempo utilizando a forma concorrente, foi de : %lf\n", delta2); 
   printf("O desempenho ganho foi de : %lf\n", (delta1/delta2)); 
	
   
   
   free(matA);
   free(matB);
   free(saidaSequencial);
   free(saidaConcorrente);
   free(args);
   
   return 0;
}
