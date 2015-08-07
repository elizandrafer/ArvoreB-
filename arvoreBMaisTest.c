#include <stdio.h>
#include <stdlib.h>

#define minOrdem 3
#define maxOrdem 4
#define maxRegistro 100
#define maxNivel 10

/*
typedef struct paginaDaArvore{
	
  int chaves[ordem];					//vetor de chaves na pagina
  struct paginaDaArvore* paginaFilha[ordem+1];		//vetor de ponteiros para paginas Filhas
  int totalChaves;					//total de chaves na pagina
  int tipo;						//armazena valor para tipo de Pagina
  struct paginaDaArvore* prox;				//ponteiro para próx pagina

} paginaDaArvore;
*/

typedef struct noDaArvore {
   int tipo;
   struct paginaInterna *pai;

}noDaArvore;

typedef struct paginaInterna{

  int tipo;
  struct paginaInterna *pai;
  struct paginaInterna *proximo;
  int filhos;
  int chaves[maxOrdem-1];
  struct noDaArvore *subPtr[maxOrdem];

}paginaInterna;


typedef struct folhaArvore{

  int tipo;
  struct paginaInterna *pai;
  struct folhaArvore *proximo;
  int registros;
  int chave[maxRegistro];
  int dado[maxRegistro];

}folhaArvore;


typedef struct arvoreBMais {

  int ordem;
  int registros;
  int nivel;
  struct noDaArvore *raiz;
  struct noDaArvore *cabeça[maxNivel];

}arvoreBMais;

static struct paginaInterna* criaArvoreBMais(void)
{

  struct paginaInterna *no = malloc(sizeof(*no));
  assert(no != NULL);
  no->tipo = 1;
  memset(no->chave, 0 , maxRegistro*sizeof(int));
  memset(no->dado, 0, maxRegistro*sizeof(int));
  no->pai = NULL;
  no->proximo = NULL;
  no->registro = 0;
  return no;
  
};

void removePagina(paginaInterna *no){

  free(no);

}

void removeFolha(paginaInterna *no){

  free(no);
  
}





int buscaBinaria(int *v, int tam, int valor) {
  int meio, inicio = 0, fim = tam-1;
  
	while(inicio <= fim) {
 		meio = (inicio + fim)/2;  
		if(valor > v[meio]) {
			inicio = meio + 1;
		}
    	else if (valor < v[meio]) {
    		fim = meio -1;
    	}
    	else {
    		return meio;
    	}
  }
  return -1;
}





int main(){



}
