/* Gaussian elimination without pivoting.
 *  * Compile with "gcc gauss.c" 
 *   */

/* ****** ADD YOUR CODE AT THE END OF THIS FILE. ******
 *  * You need not submit the provided code.
 *   */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include "timer.h" 
#define randm() 4|2[uid]&3

int aux = 0; // auxiliar na detecção de matriz singular
#define MAXN 10000  // valor máximo de N
int N;  // tamanho da matriz
double DETERMINANTE = 1; 
double DETERMINANTESEQUENCIAL = 1;


int NUMTHREADS; //NUMERO DE THREADS
int NORM = 0; // VARIÁVEL COMPARTILHADA ENTRE AS THREADS
pthread_mutex_t mutex; 
pthread_cond_t cv; 

typedef struct info_t {
	int inicio; /* Starting row */
	int fim; /* Ending row */
} info;


/* VETORES E MATRIZES */
volatile double A[MAXN][MAXN], B[MAXN], X[MAXN];
volatile double Asequencial[MAXN][MAXN], Bsequencial[MAXN], Xsequencial[MAXN];



//gauss concorrente
void gauss();  


// SEQUENCIAL APENAS ESCALONANDO 
void gaussSequencial(){
    int i,j,k;
    for(i=0;i<N-1;i++){
        for(k=i+1;k<N;k++){
            double  term=Asequencial[k][i]/Asequencial[i][i];
            for(j=0;j<N;j++){
                Asequencial[k][j]=Asequencial[k][j]-term*Asequencial[i][j];
            }
        }
    }
             
}

//SEQUENCIAL USANDO O MÉTODO A.X=B 
void gaussSequencialAXB(){
  int i, j, k, l, m;
  
    //ETAPA DE ESCALONAMENTO
    for (k = 0; k < N - 1; k++) {
        double max = abs(Asequencial[k][k]);
        int maxIndex = k;
        //procura o maior k-ésimo coeficiente em módulo
        for (i = k + 1; i < N; i++) {
            if (max < abs(Asequencial[i][k])) {
                max = abs(Asequencial[i][k]);
                maxIndex = i;
            }
        }
        if (maxIndex != k) {
            /*
             troca a equação k pela equação com o
             maior k-ésimo coeficiente em módulo
             */
            for (j = 0; j < N; j++) {
                double temp = Asequencial[k][j];
                Asequencial[k][j] = Asequencial[maxIndex][j];
                Asequencial[maxIndex][j] = temp;
            }
            double temp = Bsequencial[k];
            Bsequencial[k] = Bsequencial[maxIndex];
            Bsequencial[maxIndex] = temp;
        }
        //Se A[k][k] é zero, então a matriz dos coeficiente é singular
        //det A = 0
        if (Asequencial[k][k] == 0) {
 	    aux = 1; 
            
        } else {
            //realiza o escalonamento
            for (m = k + 1; m < N; m++) {
                double F = -Asequencial[m][k] / Asequencial[k][k];
                Asequencial[m][k] = 0; 
                Bsequencial[m] = Bsequencial[m] + F * Bsequencial[k];
                for (l = k + 1; l < N; l++) {
                    Asequencial[m][l] = Asequencial[m][l] + F * Asequencial[k][l];
                }
            }
        }
    }
    
    
    //ETAPA DE RESOLUÇÃO DO SISTEMA
    if(aux == 0){
    for (i = N - 1; i >= 0; i--) {
        Xsequencial[i] = Bsequencial[i];
        for (j = i + 1; j < N; j++) {
            Xsequencial[i] = Xsequencial[i] - Xsequencial[j] * Asequencial[i][j];
        }
        Xsequencial[i] = Xsequencial[i] / Asequencial[i][i];
    }
	}    
    
}


//CALCULANDO DETERMINANTE
void determinante(int id){
  
   if(id == 1){
  
   for(int i = 0; i < N; i++){
        DETERMINANTE *= A[i][i];
   }
   
  }else{
	for(int i = 0; i < N; i++){
  	DETERMINANTESEQUENCIAL *= Asequencial[i][i];
   }  
  
  }
   
}


//PARA IMPRIMIR X
void print_X(int id) {
	int row;
	
	if(id == 1){

		printf("\nX = [");
		for (row = 0; row < N; row++) {
			printf("%5.2f%s", X[row], (row < N-1) ? "; " : "]\n");
		}
	}
	
       
       
       
       if(id == 2){
	
		printf("\nX SEQUENCIAL= [");
		for (row = 0; row < N; row++) {
			printf("%5.2f%s", Xsequencial[row], (row < N-1) ? "; " : "]\n");
		}
	}
	
       
}


int main(int argc, char **argv) { 
	srand((unsigned)time(NULL));
    	char nome[30];
    	FILE *arq;	
    	int i, j; 
    	double inicio, fim, delta1, delta2, desempenho;  // PARA AJUDAR NO CÁLCULO DE DESEMPENHO  
    	int menu; 
    

    	printf("\n------------------------------------------MENU----------------------------------------\n");
   	printf("\n********Qual tamanho de matriz dos arquivos você quer trabalhar?*******\n");
    	printf("\n1-  5X5 (MATRIZ ARQUIVO TXT) \n");
    	printf("\n2-  10X10 (MATRIZ ARQUIVO TXT) \n");
    	printf("\n3-  15X15 (MATRIZ ARQUIVO TXT) \n");
    	printf("\n4-  1000X1000(VERIFICAR DESEMPENHO) \n");
    	printf("\n5-  PREENCHER MATRIZ \n");
    	printf("\nDigite o número correspondente: ");
    	scanf("%d", &menu); 
    	
    	switch ( menu ) {
    
	    case 1 :
	    printf ("\nVocê selecionou a matriz 5x5! \n");
	    printf ("\nPREENCHENDO A MATRIZ! \n\n");
	    N = 5; 

	    arq=fopen("matriz5x5.txt", "r");
	    fgets(nome, 30, arq);// pega o nome do participante
	    for(i=0;i<N;i++){
		for(j=0;j<N;j++){
		    fscanf(arq,"%lf ", &A[i][j]);
	   	 }
	    }

   	    for(i=0;i<N;i++){
   		   fscanf(arq,"%lf ", &B[i]);
   		   X[i] = 0.0;

   	    }
  	    fclose(arq); //fechar arquivo
  	    
  	    
  	    //PARA AJUDAR NA SEQUENCIAL. 
  	    for(i=0;i<N;i++){
		for(j=0;j<N;j++){
		    Asequencial[i][j] = A[i][j];   
	       }
	    }
  	    
  	    for(i=0;i<N;i++){
   		    Bsequencial[i] = B[i];
   		    Xsequencial[i] = 0.0;
   	    }
  	    
	    break;
	    
	    case 2 :
	    printf ("\nVocê selecionou a matriz 10x10! \n");
	    printf ("\nPREENCHENDO A MATRIZ! \n");
	    N = 10; 
	  

	    arq=fopen("matriz10x10.txt", "r");
	    fgets(nome, 30, arq);// pega o nome do participante
	    for(i=0;i<N;i++){
		for(j=0;j<N;j++){
		    fscanf(arq,"%lf ", &A[i][j]);
	   	 }
	    }

   	    for(i=0;i<N;i++){
   		   fscanf(arq,"%lf ", &B[i]);
   		   X[i] = 0.0;

   	    }
  	    fclose(arq); //fechar arquivo
  	    
  	    	
  	    
  	    //PARA AJUDAR NA SEQUENCIAL. 
  	    for(i=0;i<N;i++){
		for(j=0;j<N;j++){
		    Asequencial[i][j] = A[i][j];   
	       }
	    }
  	    
  	    for(i=0;i<N;i++){
   		    Bsequencial[i] = B[i];
   		    Xsequencial[i] = 0.0;
   	    }
  	    
	    break;
	    
	    case 3 :
	    printf ("\nVocê selecionou a matriz 15x15! \n");
	    printf ("\nPREENCHENDO A MATRIZ! \n\n");
	    N = 15;
	    arq=fopen("matriz15x15.txt", "r");
	    fgets(nome, 30, arq);// pega o nome do participante
	    for(i=0;i<N;i++){
		for(j=0;j<N;j++){
		    fscanf(arq,"%lf ", &A[i][j]);
	   	 }
	    }

   	    for(i=0;i<N;i++){
   		   fscanf(arq,"%lf ", &B[i]);
   		   X[i] = 0.0;

   	    }
  	    fclose(arq); //fechar arquivo
  	    
  	   
  	    //PARA AJUDAR NA SEQUENCIAL. 
  	    for(i=0;i<N;i++){
		for(j=0;j<N;j++){
		    Asequencial[i][j] = A[i][j];   
	       }
	    }
  	    
  	    for(i=0;i<N;i++){
   		    Bsequencial[i] = B[i];
   		    Xsequencial[i] = 0.0;
   	    }
  	    
  	    
	    break;
	    
	    case 4 :
	    printf ("\nVocê selecionou a matriz 100x100! \n");
	    printf ("\nPREENCHENDO A MATRIZ! \n\n");
	    N = 1000; 
	  

	   
		for (int col = 0; col < N; col++) {
			for (int row = 0; row < N; row++) {
				A[row][col] = (float)rand() / 32768.0;
			}
			B[col] = (float)rand() / 32768.0;
			X[col] = 0.0;
		}
  	    
  	   
  	    //PARA AJUDAR NA SEQUENCIAL. 
  	    for(i=0;i<N;i++){
		for(j=0;j<N;j++){
		    Asequencial[i][j] = A[i][j];   
	       }
	    }
  	    
  	    for(i=0;i<N;i++){
   		    Bsequencial[i] = B[i];
   		    Xsequencial[i] = 0.0;
   	    }
  	    
  	    
	    break;
	    	
	    		
	 
	    		
	    		
	    case 5:
	    
	    printf ("\nVamos preencher a matriz, digite o tamanho da MATRIZ, lembrando que trabalhamos com matriz quadrada: ");
	    		scanf("%d", &N); 
	    		
	    		for(i=0;i<N;i++){
				for(j=0;j<N;j++){
					printf ("\nDigite o ELEMENTO M[%d][%d]: ", i, j);
		   			scanf("%lf", &A[i][j]);    
	       			}
	  		  }
	    
	    
	    		for(i=0;i<N;i++){
				for(j=0;j<N;j++){
				    Asequencial[i][j] = A[i][j];   
	     			  }
	    		}
	    		
	    		int auxiliar;
	    		printf ("\n******ESCOLHA UMA OPÇÃO******* ");
	    		printf ("\n1- GAUSS PARA ENCONTRAR X DO SISTEMAS A.X = B ");
	    		printf ("\n2- APENAS ESCALONAR A MATRIZ \n");
	    		scanf("%d", &auxiliar); 
	    		
	    		if(auxiliar == 1){
	    			printf ("\nVamos preencher o vetor B ");
	    		
	    			for(i=0;i<N;i++){
					printf ("\nDigite o ELEMENTO B[%d]: ", i);
		   			scanf("%lf", &B[i]);    
	       			
	  			  }
	  			  
	  		       for(i=0;i<N;i++){
   		 		  Bsequencial[i] = B[i];
   				  Xsequencial[i] = 0.0;
   			      }
	  			  
	  			  
	    	      }
	    	      
	    	      if(auxiliar == 2){
	    	      		 for(i=0;i<N;i++){
   		 		  Bsequencial[i] =0.0; 
   		 		  B[i]=  0.0 ;
   				  Xsequencial[i] = 0.0;
   			      }    
	    	      }
	    		
	    
	    break; 
	    
	   
	    
	    default :
	    printf ("Valor invalido!\n");
  	}
  	
  	
  	
  	int menu2;
  	
  	printf ("\nSelecione a opção de cálculo envolvendo matrizes:  \n");
  	printf ("\n1 - ELIMINAÇÃO DE GAUSS USANDO PIVÔ:  \n");
  	printf ("\n2 - DETERMINANTE DA MATRIZ:  \n");
  	printf ("\n3 - VISUALIZAR MATRIZ ESCALONADA:  \n");
  	scanf("%d", &menu2);
  	
  	printf ("\nPor Favor, digite o número de THREADS: ");
	
	scanf("%d", &NUMTHREADS); 
	
  	
  	switch(menu2){
  	
  	case 1: 
  	
  
  		printf("\n---------------------------------------------------------------------------\n");
		printf("\n      *INICIANDO MODO SEQUENCIAL ");
  		GET_TIME(inicio);
		gaussSequencialAXB();
  		if(aux == 1){
  		     printf("\n A matriz dos coeficientes é singular  \n");
  		     break; 
  		}
  		GET_TIME(fim);
  		if(N != 1000 )   print_X(2); // NÃO IMPRIMIR A MATRIZ DESEMPENHO 
  	        delta1 = fim-inicio; 
		printf("\n---------------------------------------------------------------------------\n");
		printf("\nTempo sequencial: %lf\n", delta1);
		
		printf("\n---------------------------------------------------------------------------\n");
		printf("\n      *INICIANDO MODO CONCORRENTE ");
		GET_TIME(inicio);
		gauss();
		if(aux == 1){
  		     printf("\n A matriz dos coeficientes é singular  -- ERRO \n");
  		     break; 
  		}
		GET_TIME(fim);
	        if(N != 1000 )   print_X(1); // NÃO IMPRIMIR A MATRIZ DESEMPENHO 
		delta2 = fim-inicio; 
		printf("\n---------------------------------------------------------------------------\n");
		printf("\nTempo concorrente: %lf\n", delta2);
		desempenho = delta1/delta2; 
		printf("\nO DESEMPENHO FOI DE : %lf \n", desempenho); 
		printf("\nPROGRAMA FINALIZADO!");

		
		
		
		
		
  	break; 
  	
  	case 2: 
  	
  		GET_TIME(inicio);
		gauss();
		determinante(1); 
		GET_TIME(fim);
	        printf ("\nO determinante CONCORRENTE: %lf\n", DETERMINANTE);
		delta1 = fim-inicio; 
		printf("\n---------------------------------------------------------------------------\n");
		printf("\nTempo Concorrente: %lf\n", delta1);
		
		GET_TIME(inicio);
  		gaussSequencial();
	  	determinante(2); 
	  	GET_TIME(fim);
		printf ("\nO determinante SEQUENCIAL: %lf\n", DETERMINANTESEQUENCIAL);
	        delta2 = fim-inicio;
	        desempenho = delta1/delta2;  
		printf("\n---------------------------------------------------------------------------\n");
		printf("\nTempo sequencial: %lf\n", delta2);
		desempenho = delta2/delta1; 
		printf("\nO DESEMPENHO FOI DE : %lf \n", desempenho); 
		printf("\nPROGRAMA FINALIZADO!");
	 
  	break;
  	
  	
  	case 3: 
  		gauss();
  		gaussSequencial();
			printf ("\nIMPRIMINDO A MATRIZ ESCALONADA(CONCORRENTE) : \n");
			for(i=0;i<N;i++){
				for(j=0;j<N;j++){
		 		   printf("%lf ", A[i][j]);
	   			 }
	   			 printf("\n");
	   		 }
	   		 
	   		printf ("\nIMPRIMINDO A MATRIZ ESCALONADA(SEQUENCIAL) : \n");
			for(i=0;i<N;i++){
				for(j=0;j<N;j++){
		 		   printf("%lf ", Asequencial[i][j]);
	   			 }
	   			 printf("\n");
	   		 }
	   		 
	   		printf("\nPROGRAMA FINALIZADO!");
  	
  	
  	break; 

  	
  	default: 
  		printf ("Valor invalido!\n");
  	}

	
	
	
	
	exit(0);
}


void *paralelo(info * row_data) {
	int col;
	int linha, norm;
	
	
	for (norm = 0 ; norm < row_data->fim ; norm++) {
	        // como está já dividio os processos, não precisaremos de mutex nessa área
		//Todas as threads calculam a norma 0 e esperam
		for (linha = row_data->inicio; linha <= row_data->fim; linha++) {
			//pegando o pivô 
			if (linha > norm) {
				double multiplicador = A[linha][norm] / A[norm][norm]; 
				
				//aplica na linha o seu novo valor 
				for (col = norm; col < N; col++) {
					A[linha][col] -= A[norm][col] * (multiplicador);
				}
				B[linha] -= B[norm] * multiplicador;

			}
		}
		
		// ESPERAR AS THREADS TERMINAREM 
		pthread_mutex_lock(&mutex);
		if ((row_data->inicio <= NORM+1) && (NORM+1 <= row_data->fim)) { 
			/* broadcast Eu não sou o último tópico da lista */
			if (!(NORM == row_data->fim)) {
				NORM = NORM + 1;
				pthread_cond_broadcast(&cv);
			}
		}  else {
			/* wait para cálculo de norma de um thread anterior */
			if ((!(row_data->fim == NORM) || NORM<=norm) && ((NORM + 1) < N)) {
				pthread_cond_wait(&cv, &mutex); 
			}
		}
		pthread_mutex_unlock(&mutex);
	}
	free(row_data);
	pthread_exit(0);
}


void gauss() {
	int linha, col, i;  
	


	int restante, total_num_linhas, linhas_por_processo, enviado, num_linha_para_enviar,  num_threads = 0;

	/* GAUSS */
	  
	/* Acompanha as threads geradas */
	pthread_t tid[NUMTHREADS];

	/* Calcular o número de linhas para distribuir */
	total_num_linhas = N;

	/* Calcule quantas linhas cada processo terá */
	linhas_por_processo = total_num_linhas/NUMTHREADS;
	if (total_num_linhas > (NUMTHREADS * linhas_por_processo)) {  // se sobrar 
		restante = (total_num_linhas) % (NUMTHREADS); // colocamos numa variável "restante" 
	}
	

	/* Variável para acompanhar as linhas distribuídas até o momento */
	/* O valor inicial é 1, pois nenhum cálculo é necessário para a linha 0 de acordo com a eliminação de gauss*/
	enviado = 1;

	for (i = 0; i< NUMTHREADS; i++) {
		if (restante > 0 ) {  //SE TIVERMOS RESTANTE VAMOS SEPARA IGUALMENTE NO NUM_LINHA_PARA_ENVIAR
			num_linha_para_enviar = linhas_por_processo + 1; 
			restante--;
		} else {  // SE NÃO APENAS ENVIA O NORMAL 
			num_linha_para_enviar = linhas_por_processo;
		}

		//SE TIVERMOS UM NÚMERO DE LINHA PARA ENVIAR MAIOR QUE OQ SOBROU 
		if (num_linha_para_enviar > (N- enviado)) {
			num_linha_para_enviar = N - enviado;
		}

		//ALOCANDO A ESTRUTURA
		info * data = malloc(sizeof(info));
		
		
		// Calcular linha inicial 
		data->inicio = enviado;

		// Calcular linha final
		data->fim = data->inicio + num_linha_para_enviar - 1;
		if (data->fim >= N) data->fim = N-1; 

		// Vamos enviar os dados 
		if (data->inicio <= data->fim) {
			pthread_create(&tid[num_threads], NULL, (void *)paralelo, data);
			enviado = enviado + num_linha_para_enviar;
			num_threads++;
		}
	}
	
	

	// Distribuição concluída.. Vamos esperar as threads terminarem a computação
	for (i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}



	//VERIFICAR SE A MATRIZ É SINGULAR 
	for(int i = 0; i<N; i++){
		if (Asequencial[i][i] == 0) {
          	  aux = 1; 
        	} 
	}
	
	
	
	//Elementos diagonais não são normalizados para 1. Isso é tratado na substituição de volta   
	if(aux == 0){
	/* substituição de volta */
	for (linha = N - 1; linha >= 0; linha--) {
		X[linha] = B[linha];
		for (col = N-1; col > linha; col--) {
			X[linha] -= A[linha][col] * X[col];
		}
		X[linha] /= A[linha][linha];
	}
     }
}
