package laboratorio7;
import java.util.Random;
import java.util.Scanner;

/* Aluno : Victor da Silva Moura  DRE: 120124419  */
/* LABORATÓRIO 7 - ACHAR OS NÚMEROS PARES EM UM VETOR*/



class Pares{
	
	//RECURSO COMPARTILHADO
	private int QtPar; 
	
	//NÚMERO DE THREADS
	private int nThreads; 
	
	
	// VETOR UTILIZADO
	private int[] vetor; 
	
	
	
	//RECEBENDO OS VALORES
	public Pares(int nThreads, int[] vetor) {
		this.QtPar = 0; 
		this.nThreads = nThreads;
		this.vetor = vetor;
	}
	
	//USANDO SYNCHRONIZED PARA N ACONTECER CONDIÇÃO DE CORRIDA
	public synchronized void achouPar() {
		this.QtPar++; 
	}
	
	public synchronized int getQtPar() {
		return this.QtPar; 
	}
	
	//AQUI NÃO HÁ NECESSIDADE DO SYNCHRONIZED
	
	public int getTam() {
		return this.vetor.length; 
	}
	
	public int getThreads() {
		return this.nThreads; 
	}
	
	public int vetor (int i) {
		return this.vetor[i]; 
	}
	
	
}


class T extends Thread {
	
	//IDENTIFICADOR DA THREAD
	private int id; 
	
	//OBJETO COMPARTILHADO ENTRE AS OUTRAS THREADS
	Pares pares; 
	
	
	//CONSTRUTOR
	public T(int tid, Pares pares) {
		this.id = tid; 
		this.pares = pares; 

	}
	
	public void run() {
		
		for(int i = this.id; i<this.pares.getTam(); i+=this.pares.getThreads()) {
			if((this.pares.vetor(i) % 2) == 0) {
				this.pares.achouPar(); 
			}
		}
		}
	}
	




public class lab7 {
	
	static final int nthreads= 1; 
	
	static final int[] vetor = new int[10000000]; 
	

	public static void	main(String[] args) {
		
		Random gerador = new Random(); //GERAR VALORES ALEATÓRIOS 
		
		
		//PREENCHER VETOR; 
		for(int i = 0; i<vetor.length; i++) {
			vetor[i] = gerador.nextInt();
		}
		
		
		//RESERVA ESPAÇO PARA UM VETOR DE THREADS
		Thread[] threads = new Thread[nthreads];
		
		
		
		//RECURSO COMPARTILHADO
		Pares pares = new Pares(nthreads, vetor); 
		

		
		//CRIA AS THREADS DA APLICAÇÃO
		for(int i = 0; i<threads.length; i++) {
			threads[i] = new T(i, pares);
		}
		
		
		//CALCULAR TEMPO CONCORRENTE 
		long tempoInicial = System.currentTimeMillis(); 
		
		//INICIA AS THREADS
		for(int i = 0; i<threads.length; i++) {
			threads[i].start(); 
		}
		
		//espera pelo termino de todas as threads
	    for (int i=0; i<threads.length; i++) {
	         try { threads[i].join(); } catch (InterruptedException e) { return; }
	     }
	    
	    
	    long tempoFinal = System.currentTimeMillis(); 
	    
	    //ANALISE DO TEMPO CONCORRENTE
	    System.out.printf("TEMPO DE EXECUÇÃO CONCORRENTE  : %.3f ms%n" , (tempoFinal - tempoInicial)/1000d );
	    
	    
	    //MODO SEQUENCIAL 
	    int qtparSeq = 0;
	    
	    long tempoInicialSeq = System.currentTimeMillis(); 
	    
	   
	    for(int i = 0; i<vetor.length; i++) {
			if((vetor[i] % 2) == 0) {
				qtparSeq++; 
			
			}
			
		}
	    
	    
	    long tempoFinalSeq = System.currentTimeMillis(); 
	    
	    //ANALISE DO MODO SEQUENCIAL
	    System.out.printf("TEMPO DE EXECUÇÃO SEQUENCIAL  : %.3f ms%n" , (tempoFinalSeq - tempoInicialSeq)/1000d );
	    
	    
	    //VERIFICANDO SE DÃO O MESMO RESULTADO. 
	    if(qtparSeq == pares.getQtPar()) {
	    System.out.println("Quantidade de pares  = " + qtparSeq); 
	    }else {
	    	System.out.println("ERRO"); 
	    }
	    
	
		
	}
	
	
	
	
	
}
