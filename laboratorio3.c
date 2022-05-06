/*   
	LABORATÓRIO 3 -> VICTOR DA SILVA MOURA DRE: 120124419 

*/
#include<stdio.h> 
#include<stdlib.h> 
#include<pthread.h> 
#include "timer.h" 



long int dim; //dimensão do vetor de entrada
int nthreads; //número de threads
float *vetor; //vetor de entrada com dimensão dim 
float *vetorMaior; //Vetor para auxiliar a encontrar o Maior 
float *vetorMenor; // Vetor para auxiliar a encontrar o Menor
float maiorSequencial; //Auxiliar para encontrar o maior SEQUENCIAL
float menorSequencial;  //Auxiliar para encontrar o menor SEQUENCIAL 



//fluxo das threads
void * tarefa(void *arg){
   
   
   //DIVIDINDO AS THREADS 
   
   long int id = (long int) arg; //identificador da thread
   long int tamBloco = dim/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = dim;
   else fim = ini + tamBloco; //trata o resto se houver
   
   
   
   //PERCORRENDO O VETOR PARA ENCONTRAR O MAIOR E MENOR. 
   float maior = vetor[0];
   float menor = vetor[0]; 
   
   for(long int i=ini; i<fim; i++)
      {
      	if(maior > vetor[i]){
      		maior = vetor[i]; 
      	}
      	
      	if(menor < vetor[i]) {menor = vetor[i]; }
      	      }
      
   //Colocando no vetor auxiliar baseado no ID de cada thread.    
   vetorMaior[id] = maior; 
   vetorMenor[id] = menor;    
      
   pthread_exit(NULL); 

}





//FUNÇÃO PARA ACHAR USANDO O MÉTODO SEQUENCIAL. 
void maiorOuMenorSeq(long int dim){
	
	float auxiliarMaior = vetor[0];
	float auxiliarMenor = vetor[0];

	for(long int i =1; i<dim; i++){
                if(vetor[i]>=auxiliarMaior){
		     auxiliarMaior = vetor[i]; 
		}
		
		if(vetor[i]<=auxiliarMenor) {
		     auxiliarMenor = vetor[i]; 
		}
		
        }

	maiorSequencial = auxiliarMaior;
	menorSequencial = auxiliarMenor; 

}

int main(int argc,  char *argv[]){

	pthread_t *tid; //identificadores das threas no sistema 

 	double inicio, fim, delta1, delta2, desempenho;  // Para ajudar no cálculo de desempenho  
 	
 	float auxiliar, auxiliar2; //Para ajudar na hora de percorrer o vetor de auxilio   



	//recebe e valida  os parâmetros de entrada(dimensão do vetor, número de threads)
	if(argc < 3){
		fprintf(stderr, "Digite: %s <dimensao do vetor> <Número de threads>\n",argv[0]);
	        return 1;	
	}
	dim = atoll(argv[1]);
	nthreads = atoi(argv[2]);





	//alocar  o vetor de entrada
	vetor = (float *) malloc(sizeof(float)*dim); 
       	if(vetor == NULL) {
		fprintf(stderr, "ERRO--malloc\n");
	        return 2; 	
	}

	
	//alocar  o vetor auxiliar Maior
	vetorMaior = (float *) malloc(sizeof(float)*nthreads); 
       	if(vetor == NULL) {
		fprintf(stderr, "ERRO--malloc\n");
	        return 2; 	
	}
	
	//alocar  o vetor auxiliar Menor
	vetorMenor = (float *) malloc(sizeof(float)*nthreads); 
       	if(vetor == NULL) {
		fprintf(stderr, "ERRO--malloc\n");
	        return 2; 	
	}



	//preenche o vetor de entrada; 
	for(long int i =0; i<dim; i++){
		vetor[i] = 1000.1/(i+1);
	}


  

	//USANDO A FUNÇÃO SEQUENCIAL PARA ENCONTRAR O MAIOR E MENOR VETOR
	GET_TIME(inicio);  
	maiorOuMenorSeq(dim); 
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
         
         
         
        //Verificando o maior e menor nos vetores auxiliares. 
        auxiliar = vetorMaior[0]; 
        auxiliar2 = vetorMenor[0];
        for( long int i = 0; i<nthreads; i++){
               if(auxiliar > vetorMaior[i]){
      		auxiliar = vetorMaior[i]; }
      	
      		if(auxiliar2 < vetorMenor[i]) auxiliar2 = vetorMenor[i]; 
         }

	GET_TIME(fim);
       	delta2 = fim - inicio; 	
	printf("Tempo concorrente: %lf\n", delta2);
	printf("\n---------------------------------------------------------------------------\n");


	desempenho = delta1/delta2; 

	//exibir os resultados. 
	printf("\t *****RESULTADOS DO SEQUENCIAL E CONCORRENTE***** \n ");
	printf("\nSEQUENCIAL:  MENOR %lf  e MAIOR %lf\n", menorSequencial, maiorSequencial); 
	printf("\nCONCORRENTE:  MENOR %lf   e MAIOR %lf \n", auxiliar,  auxiliar2); 
	printf("\n---------------------------------------------------------------------------\n");
	printf("O DESEMPENHO FOI DE : %lf \n", desempenho); 
	
	//Libera as areas de memoria alocadas 
	free(vetor); 
	free(tid); 
	free(vetorMaior);
	free(vetorMenor); 
	return 0;
}
