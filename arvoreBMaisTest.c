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

//0 - Folha
//1 - Não Folha

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


typedef struct paginaFolha{

  int tipo;
  struct paginaInterna *pai;
  struct paginaFolha *proximo;
  int registros;
  int chaves[maxRegistro];
  int dados[maxRegistro];

}paginaFolha;


typedef struct arvoreBMais {

  int ordem;
  int registros;
  int nivel;
  struct noDaArvore *raiz;
  struct noDaArvore *cabeça[maxNivel];

}arvoreBMais;

paginaInterna* criarPagina(void) {
  paginaInterna *no = malloc(sizeof(*no));
  assert(no != NULL);
  no->tipo = 1;  //não é folha
  memset(no->chaves, 0 , maxRegistro*sizeof(int));
  memset(no->subPtr, 0, maxRegistro*sizeof(noDaArvore*));
  no->pai = NULL;
  no->proximo = NULL;
  no->filhos = 0;
  
  return no;
};

paginaFolha* criarFolha(void) {
  paginaFolha *folha = malloc(sizeof(*folha));
  assert(folha != NULL);
  folha->tipo = 0;     //é folha
  memset(folha->chaves, 0, maxRegistro* sizeof(int));
  memset(folha->dados, 0, maxRegistro* sizeof(int));
  folha->pai = NULL;
  folha->proximo = NULL;
  folha->registros = 0;

  return folha;
}

void removerFolha(paginaFolha *no){
  free(no);
}

void removerPagina(paginaInterna *no){
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

int buscarNaArvore(arvoreBMais *arvore, int chave)
{
        int i;
        noDaArvore *no = arvore->raiz;
        paginaInterna *naoFolha;
        paginaFolha *folha;

        while (no != NULL) {
                switch (no->tipo) {
                case 0: //folha
                        folha = (paginaFolha*)no;
                        i = buscaBinaria(folha->chaves, folha->registros, chave);
                        if (i >= 0) {
                                return folha->dado[i];
                        } else {
                                return 0;
                        }
                case 1: //não folha
                        naoFolha = (paginaInterna*)no;
                        i = buscaBinaria(naoFolha->chaves, naoFolha->filhos-1, chave);
                        if (i >= 0) {
                                no = naoFolha->subPtr[i+1];
                        } else {
                                i = -i - 1;
                                no = naoFolha->subPtr[i];
                        }
                        break;
                
                default:
                        assert(0);
                }
        }

        return 0;
}

int inserirNaPagina(arvoreBMais *arvore, paginaInterna *pagina, noDaArvore *noArv, int chave, int nivel)
{
        int i, j, split_chave;
        int split = 0;
        paginaInterna *sibling;

        int auxInserir = buscaBinaria(pagina->chaves, pagina->filhos - 1, chave);
        assert(auxInserir < 0);
        auxInserir = -auxInserir - 1;

        if (pagina->filhos == arvore->ordem) {
                
                split = (arvore->ordem + 1) / 2;
                
                sibling = criarPagina();
                sibling->proximo = pagina->proximo;
                pagina->proximo = sibling;
                pagina->filhos = split + 1;
                
                if (auxInserir < split) {
                        i = split - 1;
                        j = 0;
                        split_chave = pagina->chaves[i];
                        sibling->subPtr[j] = pagina->subPtr[i+1];
                        pagina->subPtr[i+1]->pai = sibling;
                        i++;
                        
                        while (i < arvore->ordem - 1) {
                                sibling->chaves[j] = pagina->chaves[i];
                                sibling->subPtr[j+1] = pagina->subPtr[i+1];
                                pagina->subPtr[i+1]->pai = sibling;
                                i++;
                                j++;
                        }

                        sibling->filhos = j + 1;
                                                
                        for (i = pagina->filhos - 1; i > auxInserir; i--) {
                                pagina->chaves[i] = pagina->chaves[i-1];
                                pagina->subPtr[i+1] = pagina->subPtr[i];
                        }

                        pagina->chaves[i] = chave;
                        pagina->subPtr[i+1] = noArv;
                } else if (auxInserir == split) {
                        i = split;
                        j = 0;
                        split_chave = chave;
                        sibling->subPtr[j] = noArv;
                        noArv->pai = sibling;
                        i++;

                        while(i < arvore->ordem - 1){
                                sibling->chaves[j] = pagina->chaves[i];
                                sibling->subPtr[j+1] = pagina->subPtr[i+1];
                                pagina->subPtr[i+1]->pai = sibling;
                                i++;
                                j++;
                        }

                        sibling->filhos = j + 1;
                } else {
                        i = split;
                        j = 0;
                        split_chave = pagina->chaves[i];
                        sibling->subPtr[j] = pagina->subPtr[i+1];
                        pagina->subPtr[i+1]->pai = sibling;
                        i++;
                        while (i < arvore->ordem - 1) {
                                if (j != auxInserir - split) {
                                        sibling->chaves[j] = pagina->chaves[i];
                                        sibling->subPtr[j+1] = pagina->subPtr[i+1];
                                        pagina->subPtr[i+1]->pai = sibling;
                                        i++;
                                }
                                j++;
                        }
                        
                        if (j > auxInserir - split) {
                                sibling->filhos = j+1;
                        } else {
                                sibling->filhos = auxInserir - split+1;
                        }
                        
                        j = auxInserir - split - 1;
                        sibling->chaves[j] = chave;
                        sibling->subPtr[j+1] = noArv;
                        noArv->pai = sibling;
                }
        } else {
                for (i = pagina->filhos - 1; i > auxInserir; i--) {
                        pagina->chaves[i] = pagina->chaves[i-1];
                        pagina->subPtr[i+1] = pagina->subPtr[i];
                }
                pagina->chaves[i] = chave;
                pagina->subPtr[i+1] = noArv;
                pagina->filhos++;
        }

        if (split) {
                paginaInterna *pai = pagina->pai;
                if (pai == NULL) {
                        
                        if (++nivel >= arvore->nivel) {
                                fprintf(stderr, "!!Panic: Nivel exceeded, please expand the arvore nivel, non-leaf ordem or leaf entries for element capacity!\n");
                                pagina->proximo = sibling->proximo;
                                removerPagina(sibling);
                                return -1;
                        }
                        
                        pai = criarPagina();
                        pai->chaves[0] = split_chave;
                        pai->subPtr[0] = (noDaArvore*)pagina;
                        pai->subPtr[1] = (noDaArvore*)sibling;
                        pai->filhos = 2;
                        
                        arvore->raiz = (noDaArvore*)pai;
                        arvore->head[nivel] = (noDaArvore*)pai;
                        pagina->pai = pai;
                        sibling->pai = pai;
                } else {
                        
                        sibling->pai = pai;
                        return inserirNaPagina(arvore, pai, (noDaArvore*)sibling, split_chave, nivel+1);
                }
        }

        return 0;
}

int inserirNaFolha(){

}

int inserirNaArvore(){
  
}

int main(){



}
