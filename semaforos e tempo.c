#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


#define THREADS 2
#define max_transito 10
#define MAX_VALORES 100

#define RAIZ_COMUM 0
#define RAIZ_PROCESSOS 1

#define EXECUCOES 1000000


struct carros{

	int valores;
	struct carros* proximo;

};


struct transito{

	struct carros* inicio;
	struct carros* fim;
	int quantidade;

};


typedef struct carros gente;
typedef struct transito rua;


rua* criar(){

    rua* apontamento;
    apontamento = (rua*)malloc(sizeof(rua));
    if(apontamento==NULL) exit(5);
    apontamento->inicio = NULL;
    apontamento->fim = NULL;
    apontamento->quantidade = 0;

    return apontamento;
}


int remover_primeiro(rua* z) {
    if (z->inicio == NULL) return -1;
    
    gente* aux = z->inicio;
    z->inicio = aux->proximo;
    
    if (z->inicio == NULL) z->fim = NULL;
    
    int guardar = aux->valores;
    free(aux);
    z->quantidade--;
    return guardar;
}


void inserir_final(rua* z, int novo, int thread){

	if(z == NULL) exit(2);
	else if(z->inicio == NULL){

	    gente* aux;
	    aux = (gente*)malloc(sizeof(gente));
	    aux->valores = novo;
	    aux->proximo = NULL;
	    z->inicio = aux;
	    z->fim = aux;

	}else{                              
	
		if(thread && z->quantidade >= 5){
			remover_primeiro(z);
		}
		
	    gente* caminhar;
	    caminhar = z->inicio;
	
	    while(caminhar != NULL){
	
	        caminhar = caminhar->proximo;
	
	    }
	
	    gente* aux;
	    aux = (gente*)malloc(sizeof(gente));
	    aux->valores = novo;
	    aux->proximo = NULL;
	
	    z->fim->proximo = aux;
	    z->fim = aux;


	}
	
	z->quantidade++;
	return;
}


int main(){
	
	//criando a fila de inteiros
	
	rua* z;
	z = criar();
	
	srand(time(NULL));
	
	int k, x;
	for(k = 1; k <= max_transito; k++){
		
		x = rand() % MAX_VALORES;
		inserir_final(z,x,RAIZ_COMUM);
	}
	
	//definindo as variaveis das threads
	
	rua** linhas;
	linhas = (rua**)malloc(THREADS*sizeof(rua*));
	
	int escolhas[2];
	escolhas[0] = 0;
	escolhas[1] = 0;
	
	double tempos[2];
	tempos[0] = 0;
	tempos[1] = 0; 
	
	//iniciando a regiao paralela
	
	double inicio, fim;
	int contagem = 0;
	
    #pragma omp parallel num_threads(THREADS)
    {
    	
    	linhas[omp_get_thread_num()] = criar();
    	
    	int passagem, novo;
    	while(contagem < EXECUCOES){
		
	    	#pragma omp critical
	    	{
	    		
	    		//operacoes na fila por thread
				
				inicio = omp_get_wtime();
				
		    	passagem = remover_primeiro(z);
		    	inserir_final(linhas[omp_get_thread_num()],passagem,RAIZ_PROCESSOS);
		    	
		    	novo = rand() % MAX_VALORES;
		    	inserir_final(z,novo,RAIZ_COMUM);

				fim = omp_get_wtime();
				
				//atualizacao de valores para execucao do programa
				
				escolhas[omp_get_thread_num()]++;
				tempos[omp_get_thread_num()] += (fim - inicio);
				contagem++;
			}
    	}
    	
    	#pragma omp barrier
    	printf("thread %d capturou %d valores em %lf, uma media de %lf por captura\n",omp_get_thread_num(),escolhas[omp_get_thread_num()],tempos[omp_get_thread_num()],(tempos[omp_get_thread_num()]/escolhas[omp_get_thread_num()]));
	}
	
	return 0;
}
