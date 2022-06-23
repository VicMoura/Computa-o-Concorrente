// Monitor que implementa a logica do padrao leitores/escritores
class LE {
  private int leit, escr, aux;  
  
  // Construtor
  LE() { 
     this.leit = 0; 
     this.escr = 0; 
     this.aux =  0; //variável do LABORATÓRIO 8
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id, int identificadorUnico) {
    try { 
      while (this.escr > 0) {
    	 if(identificadorUnico == 102) {  // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
    		 System.out.println ("le.(T2)Bloqueado("+id+")");
    	 }else if(identificadorUnico == 102) { // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
    		 System.out.println ("le.(T3)Bloqueado("+id+")");
    	 }
         wait();  
      }
      this.leit++;  
      
      if(identificadorUnico == 102) { // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
    	  System.out.println ("le.(T2)Lendo("+id+")");
    	  if(this.aux % 2 == 0) {  //VERIFICA SE É PAR OU ÍMPAR
    		  System.out.println ("le.(T2)Valor da variável é "+this.aux+" e é PAR("+ id +")");
    	  }else {
    		  System.out.println ("le.(T2)Valor da variável é "+this.aux+" e é ÍMPAR("+ id +")");
    	  }
      }else if(identificadorUnico == 103) {  // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
 		System.out.println ("le.(T3)Lendo("+id+")");
 		System.out.println ("le.(T3)Valor da variável é "+this.aux+" ("+ id +")");
      }
      
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id, int identificadorUnico) {
     this.leit--; 
     if (this.leit == 0) 
           this.notify(); 
     if(identificadorUnico == 102) { // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
   	  System.out.println ("le.(T2)Saindo("+id+")");
     }else if(identificadorUnico == 103) { // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
		System.out.println ("le.(T3)Saindo("+id+")");
     }
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id, int identificadorUnico) {
    try { 
      while ((this.leit > 0) || (this.escr > 0)) {
    	  
    	  if(identificadorUnico == 101) { // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
    		  System.out.println ("le.(T1)Bloqueado("+id+")");
          }else if(identificadorUnico == 103) { // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
        	  System.out.println ("le.(T3)Bloqueado("+id+")");
          }
         wait();   
      }
      this.escr++; 
      if(identificadorUnico == 101) {// PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
		  System.out.println ("le.(T1)Escrevendo("+id+")");
		  this.aux++; 
      }else if(identificadorUnico == 103) {// PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
    	  System.out.println ("le.(T3)Escrevendo("+id+")");
    	  this.aux = id; 
      }
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id, int identificadorUnico) {
     this.escr--; 
     notifyAll(); 
     if(identificadorUnico == 101) { // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
		  System.out.println ("le.(T1)Saindo("+id+")");
     }else if(identificadorUnico == 103) { // PARA AJUDAR NA VISUALIZAÇÃO DE QUE THREAD ESTÁ SENDO EXECUTADA
    	 System.out.println ("le.(T3)Saindo("+id+")");
     }
  }
}


/*IDENTIFICADORES ÚNICOS, THREAD 1 = 101, THREAD 2 == 102, THREAD 3 == 103 */


// T1
class T1 extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo
  int identificadorUnico; // PARA AJUDAR NA IDENTIFICAÇÃO
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  T1 (int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
    this.identificadorUnico = 101; 
  }

  // Método executado pela thread
  public void run () {
    double j=777777777.7, i;
    try {
      for (;;) {
    	  this.monitor.EntraEscritor(this.id, this.identificadorUnico); 
    	  for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de ESCREVER
          this.monitor.SaiEscritor(this.id, this.identificadorUnico); 
          sleep(this.delay); //atraso bobo...
      }
    } catch (InterruptedException e) { return; }
  }
}


//T2
class T2 extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo...
  int identificadorUnico;  // PARA AJUDAR NA IDENTIFICAÇÃO
  LE monitor; //objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  T2 (int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
    this.identificadorUnico = 102; 
  }

  // Método executado pela thread
  public void run () {
    double j=777777777.7, i;
    try {
      for (;;) {
        this.monitor.EntraLeitor(this.id, this.identificadorUnico); 
        for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de LEITURA
        this.monitor.SaiLeitor(this.id, this.identificadorUnico); 
        sleep(this.delay); //atraso bobo...
      }
    } catch (InterruptedException e) { return; }
  }
}

//T3
class T3 extends Thread {
	int id; //identificador da thread
	int delay; //atraso bobo...
	int identificadorUnico;  // PARA AJUDAR NA IDENTIFICAÇÃO
	LE monitor; //objeto monitor para coordenar a lógica de execução das threads

// Construtor
	T3 (int id, int delayTime, LE m) {
		this.id = id;
		this.delay = delayTime;
		this.monitor = m;
		this.identificadorUnico = 103;
	}

// Método executado pela thread
	public void run () {
		double j=777777777.7, i;
		try {
			for (;;) {
				this.monitor.EntraLeitor(this.id, this.identificadorUnico); 
				for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de LEITURA
				this.monitor.SaiLeitor(this.id, this.identificadorUnico); 
				for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de LEITURA
		    	this.monitor.EntraEscritor(this.id, this.identificadorUnico); 
		    	for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de ESCRITA
		    	this.monitor.SaiEscritor(this.id, this.identificadorUnico);
				sleep(this.delay); //atraso bobo...
			}
		} catch (InterruptedException e) { return; }
	}
}



// Classe principal
class LeitorEscritor {
  static final int UM = 1;
  static final int DOIS = 2;
  static final int TRES = 3;


  public static void main (String[] args) {
    int i;
    LE monitor = new LE();            
    T1[] a = new T1[UM];       
    T2[] b = new T2[DOIS];  
    T3[] c = new T3[TRES]; 

    //inicia o log de saida
    System.out.println ("import verificaLE");
    System.out.println ("le = verificaLE.LE()");
    
    for (i=0; i<UM; i++) {
       a[i] = new T1(i+1, (i+1)*500, monitor);
       a[i].start(); 
    }
    for (i=0; i<DOIS; i++) {
       b[i] = new T2(i+1, (i+1)*500, monitor);
       b[i].start(); 
    }
    
    for (i=0; i<TRES; i++) {
        c[i] = new T3(i+1, (i+1)*500, monitor);
        c[i].start(); 
     }
    
  }
}

