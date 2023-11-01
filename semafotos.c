#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <windows.h>
#include <conio.h>

#define THREADS 2
#define max_transito 10
#define MAX_VALORES 100

#define RAIZ_COMUM 0
#define RAIZ_PROCESSOS 1



void gotoyx(int x, int y){						//usando o gotoxy ao contrario pra ficar igual a logica de uma matriz real
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){y-1,x-1});  	   //trabalha com matriz, comeca em [0][0]
}


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
	else if(z->inicio == NULL){       //se a lista era vazia, se nao tem inicio tambem nao tem final

	    gente* aux;
	    aux = (gente*)malloc(sizeof(gente));
	    aux->valores = novo;
	    aux->proximo = NULL;
	    z->inicio = aux;
	    z->fim = aux;

	}else{                              //eu poderia so inserir no final usando o ponteiro fim mas de qualquer jeito eu preciso que o penultimo aponte pro novo item entao ainda preciso andar lista-1
		
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






void imprimir(rua* z){

if(z  == NULL) exit(7);
if(z->inicio == NULL) return;       //vazia

int imprimi = 0;
gente* caminhar;
caminhar = z->inicio;

while(caminhar->proximo != NULL){

    if(caminhar==z->fim) printf("(%d) -> ",caminhar->valores);
    else if(caminhar==z->inicio) printf("{%d} -> ",caminhar->valores);
    else printf("[%d] -> ",caminhar->valores);
    imprimi++;
    caminhar = caminhar->proximo;
}

    if(caminhar==z->fim) printf("(%d) -> ||",caminhar->valores);
    else if(caminhar==z->inicio) printf("{%d} -> ",caminhar->valores);
    else printf("[%d] -> ||",caminhar->valores);
    imprimi++;

    
    printf("\n\nsao X e imprimi %d",imprimi);

    printf("\n\n");
    return;
}




void impressao_tela(rua* z, int y, int x, char icone){
	
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	if(z  == NULL) exit(7);
	if(z->inicio == NULL) return;       //vazia
	
	int imprimi = 0;
	gente* caminhar;
	caminhar = z->inicio;
	
	if(y == 2){
		gotoyx(y,x-2);
		printf("%c",icone);
	}else gotoyx(y,x);
	
	
	
	
	while(caminhar->proximo != NULL){
		
		SetConsoleTextAttribute(hConsole, ((caminhar->valores % 15) + 1) * 16);
	
	    if(caminhar==z->fim) printf("(%02d)",caminhar->valores);
	    else if(caminhar==z->inicio) printf("{%02d}",caminhar->valores);
	    else printf("[%02d]",caminhar->valores);
	    
	    SetConsoleTextAttribute(hConsole,7);
	    printf(" <- ");
	    
	    imprimi++;
	    caminhar = caminhar->proximo;
	}
	
	SetConsoleTextAttribute(hConsole, ((caminhar->valores % 15) + 1) * 16);

    if(caminhar==z->fim) printf("(%02d)",caminhar->valores);
    else printf("[%02d]",caminhar->valores);
    
    SetConsoleTextAttribute(hConsole,7);
    if(y == 2) printf(" <- ||");
    else if(y == 1) printf("%c",187);
    else if(y == 3) printf("%c",188);
    imprimi++;

    
    //printf("\n\nsao X e imprimi %d",imprimi);

    //printf("\n\n");
    return;
}


int main(){
	
	rua* z;
	z = criar();
	
	srand(time(NULL));
	
	int k, x;
	for(k = 1; k <= max_transito; k++){
		
		x = rand() % MAX_VALORES;
		inserir_final(z,x,RAIZ_COMUM);
		
	}
	
	rua** linhas;
	linhas = (rua**)malloc(THREADS*sizeof(rua*));
	
    #pragma omp parallel num_threads(THREADS)
    {
    	
    	linhas[omp_get_thread_num()] = criar();
    	
    	int passagem, novo;
    	while(1){
		
	    	#pragma omp critical
	    	{
			
		    	passagem = remover_primeiro(z);
		    	inserir_final(linhas[omp_get_thread_num()],passagem,RAIZ_PROCESSOS);
		    	
		    	novo = rand() % MAX_VALORES;
		    	inserir_final(z,novo,RAIZ_COMUM);
		    	
		    	if(omp_get_thread_num() == 0){
		    		impressao_tela(linhas[omp_get_thread_num()],1,1,32);
		    		impressao_tela(z,2,39,200);
		    		
				}else if (omp_get_thread_num() == 1){
					impressao_tela(linhas[omp_get_thread_num()],3,1,32);
					impressao_tela(z,2,39,201);
				}
		
		//getch();
			}
    	
    	}
    	
    	

    	
    	
    	
	}
	
	return 0;
	
}