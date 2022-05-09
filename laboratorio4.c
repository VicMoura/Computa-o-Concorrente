/*   
	LABORATÓRIO 4-> VICTOR DA SILVA MOURA DRE: 120124419 

*/
#include<stdio.h> 
#include<stdlib.h> 
#include<pthread.h> 
#include <math.h>
#include "timer.h" 



long int dim; //dimensão do vetor de entrada
int nthreads; //número de threads
long int *vetorEntrada; //vetor de entrada com dimensão dim 
float *vetorSaidaSequencial; //Vetor saída sequencial 
float *vetorSaidaConcorrente; //vetor saída concorrente
long int i_global = 0; // vetor para auxiliar na tarefa
pthread_mutex_t mutex; 




// FUNÇÃO É PRIMO 
int ehPrimo(long int n){
	if(n<=1) return 0;
	if(n==2) return 1;
	if(n%2==0) return 0;
	for(int i=3; i<sqrt(n)+1; i+=2){
		if(n%i == 0) return 0; 
	}
	return 1; 
}



//fluxo das threads
void * tarefa(void *arg){
	
	
   //Variável global, logo, usando lock 
   pthread_mutex_lock(&mutex);	
   long int i_local = i_global; 
   i_global++; 
   pthread_mutex_unlock(&mutex);
   
   
   while(i_local < dim){
      	if (ehPrimo(vetorEntrada[i_local])){
      			//Variável global, logo, usando lock 
      			pthread_mutex_lock(&mutex);
			vetorSaidaConcorrente[i_local] = sqrt(vetorEntrada[i_local]);
			pthread_mutex_unlock(&mutex);
			
			}
		else {
			//Variável global, logo, usando lock 
			pthread_mutex_lock(&mutex);
			vetorSaidaConcorrente[i_local] = vetorEntrada[i_local];
			pthread_mutex_unlock(&mutex);
			
		}
	
	//Variável global, logo, usando lock 
	pthread_mutex_lock(&mutex);
	i_local = i_global; 
	i_global++;
      	pthread_mutex_unlock(&mutex);
   }
   pthread_exit(NULL); 

}



//FUNÇÃO USANDO O MÉTODO SEQUENCIAL. 

void processaPrimos(long int *vetorEntrada, float *vetorSaida, int dim) {
	for(long int i=0; i<dim; i++) {
		if (ehPrimo(vetorEntrada[i])){
			vetorSaida[i] = sqrt(vetorEntrada[i]);
			}
		else {
			vetorSaida[i] = vetorEntrada[i];
		}
	}
}



//VER SE RESULTADOS SÃO IGUAIS

int compararFuncoes(){
	for(long int i = 0; i < dim; i++){
		if(vetorSaidaSequencial[i] != vetorSaidaConcorrente[i]) return 0; 
	}
	
	return 1;	
}




int main(int argc,  char *argv[]){

	pthread_t *tid; //identificadores das threas no sistema 

 	double inicio, fim, delta1, delta2, desempenho;  // Para ajudar no cálculo de desempenho  
 	


	//recebe e valida  os parâmetros de entrada(dimensão do vetor, número de threads)
	if(argc < 3){
		fprintf(stderr, "Digite: %s <dimensao do vetor> <Número de threads>\n",argv[0]);
	        return 1;	
	}
	dim = atoll(argv[1]);
	nthreads = atoi(argv[2]);





	//alocar  o vetor de entrada
	vetorEntrada= (long int *) malloc(sizeof(long int)*dim); 
       	if(vetorEntrada== NULL) {
		fprintf(stderr, "ERRO--malloc\n");
	        return 2; 	
	}
	
	
	
	
	
	//alocar o vetor de saída de sequencial e concorrente. 
	vetorSaidaSequencial= (float *) malloc(sizeof(float)*dim); 
       	if(vetorSaidaSequencial== NULL) {
		fprintf(stderr, "ERRO--malloc\n");
	        return 2; 	
	}
	
	
	vetorSaidaConcorrente= (float *) malloc(sizeof(float)*dim); 
       	if(vetorSaidaConcorrente== NULL) {
		fprintf(stderr, "ERRO--malloc\n");
	        return 2; 	
	}
	




	//preenche o vetor de entrada; 
	for(long int i =0; i<dim; i++){
		vetorEntrada[i] = i;
	}



  

	//USANDO A FUNÇÃO SEQUENCIAL 
	GET_TIME(inicio);  
	processaPrimos(vetorEntrada, vetorSaidaSequencial, dim);
	GET_TIME(fim); 
	delta1 = fim-inicio; 
	printf("\n---------------------------------------------------------------------------\n");
	printf("\nTempo sequencial: %lf\n", delta1);





	
	//Versão concorrente
	GET_TIME(inicio); 
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads); 
        if(tid == NULL){
	       	fprintf(stderr, "ERRO--malloc\n");
                return 2;
	}	
	
	
	//criar as threads
	 for( long int i = 0; i<nthreads; i++){ 	
		if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
			fprintf(stderr, "ERRO--pthread_create\n");
	                return 3;
		}
	 }


	//aguardar o termina das threads 
	for( long int i = 0; i<nthreads; i++){
                if(pthread_join(*(tid+i), NULL)){
                        fprintf(stderr, "ERRO--pthread_create\n");
                        return 3;
                }	
         }
         
         

	GET_TIME(fim);
       	delta2 = fim - inicio; 	
	printf("Tempo concorrente: %lf\n", delta2);
	printf("\n---------------------------------------------------------------------------\n");




	desempenho = delta1/delta2; 


	//exibir os resultados. 
	printf("RESULTADOS DO SEQUENCIAL E CONCORRENTE\n ");
	printf("\nO DESEMPENHO FOI DE : %lf \n", desempenho); 
	printf("\nVERIFICANDO SE OS DOIS VETORES DE SAÍDAS SÃO IGUAIS: ");
	if(compararFuncoes()){ printf("SÃO IGUAIS\n");}
	else {printf("\nNÃO SÃO IGUAIS\n");}
	
	
	//Libera as areas de memoria alocadas 
	free(vetorEntrada); 
	free(tid); 
	free(vetorSaidaSequencial);
	free(vetorSaidaConcorrente);
	pthread_mutex_destroy(&mutex);
	return 0;
}
