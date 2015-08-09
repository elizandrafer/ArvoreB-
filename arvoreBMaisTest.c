#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define minOrdem 3
#define maxOrdem 1024
#define maxRegistro 4096
#define maxNivel 10

/*#define minOrdem 3
#define maxOrdem 4
#define maxRegistro 100
#define maxNivel 10
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
  struct noDaArvore *cabeca[maxNivel];

}arvoreBMais;

typedef struct configuracaoArvore {
 
    int nivel;
    int ordem;
    int registros;  

}configuracaoArvore;

paginaInterna* criarPagina(void) {
  paginaInterna *no = malloc(sizeof(*no));
  assert(no != NULL);
  no->tipo = 1;  //não é folha
  memset(no->chaves, 0 , (maxOrdem-1) * sizeof(int));
  memset(no->subPtr, 0, (maxOrdem) * sizeof(noDaArvore *));
  no->pai = NULL;
  no->proximo = NULL;
  no->filhos = 0;
  
  return no;
};

paginaFolha* criarFolha(void) {
  paginaFolha *folha = malloc(sizeof(*folha));
  assert(folha != NULL);
  folha->tipo = 0;     //é folha
  memset(folha->chaves, 0, maxRegistro * sizeof(int));
  memset(folha->dados, 0, maxRegistro * sizeof(int));
  folha->pai = NULL;
  folha->proximo = NULL;
  folha->registros = 0;

  return folha;
}

arvoreBMais arvore;

void removerFolha(paginaFolha *no){
  free(no);
}

void removerPagina(paginaInterna *no){
  free(no);
}

int buscaBinaria(int *v, int tam, int valor) {
  int meio, inicio = -1, fim = tam;
  
  while(inicio + 1 < fim) {
    meio = inicio + (fim - inicio)/2;  
    if(valor > v[meio]) {
      inicio = meio;
    }
    else if (valor < v[meio]) {
      fim = meio;
    }
  }
  if (fim >= tam || v[fim] != valor) {
    return -fim - 1;
  } else {
    return fim;
  }
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
                return folha->dados[i];
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
                    fprintf(stderr, "!!Panic: Nivel exceeded, please expand the arvore nivel, non-leaf ordem or leaf registros for element capacity!\n");
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
                arvore->cabeca[nivel] = (noDaArvore*)pai;
                pagina->pai = pai;
                sibling->pai = pai;
            } else {  
                sibling->pai = pai;
                return inserirNaPagina(arvore, pai, (noDaArvore*)sibling, split_chave, nivel+1);
            }
    }

    return 0;
}

int inserirNaFolha(arvoreBMais *arvore, paginaFolha *folha, int chave, int dado)
{
        int i, j, split = 0;
        paginaFolha *sibling;

        int auxInserir = buscaBinaria(folha->chaves, folha->registros, chave);
        if (auxInserir >= 0) {
                return -1;
        }
        auxInserir = -auxInserir - 1;

        if (folha->registros == arvore->registros) {
                split = (arvore->registros + 1) / 2;
                sibling = criarFolha();
                sibling->proximo = folha->proximo;
                folha->proximo = sibling;
                folha->registros = split;
                
                if (auxInserir < split) {
                        for (i = split - 1, j = 0; i < arvore->registros; i++, j++) {
                                sibling->chaves[j] = folha->chaves[i];
                                sibling->dados[j] = folha->dados[i];
                        }
                        sibling->registros = j;
                        
                        for (i = folha->registros; i > auxInserir; i--) {
                                folha->chaves[i] = folha->chaves[i-1];
                                folha->dados[i] = folha->dados[i-1];
                        }
                        folha->chaves[i] = chave;
                        folha->dados[i] = dado;
                } else {
                        i = split, j = 0;
                        while (i < arvore->registros) {
                                if (j != auxInserir - split) {
                                        sibling->chaves[j] = folha->chaves[i];
                                        sibling->dados[j] = folha->dados[i];
                                        i++;
                                }
                                j++;
                        }

                        if (j > auxInserir - split) {
                                sibling->registros = j;
                        } else {
                                sibling->registros = auxInserir - split + 1;
                        }

                        j = auxInserir - split;
                        sibling->chaves[j] = chave;
                        sibling->dados[j] = dado;
                }
        } else {
                for (i = folha->registros; i > auxInserir; i--) {
                        folha->chaves[i] = folha->chaves[i-1];
                        folha->dados[i] = folha->dados[i-1];
                }
                folha->chaves[i] = chave;
                folha->dados[i] = dado;
                folha->registros++;
        }

        if (split) {
                paginaInterna *pai = folha->pai;
                if (pai == NULL) {
                        pai = criarPagina();
                        pai->chaves[0] = sibling->chaves[0];
                        pai->subPtr[0] = (noDaArvore*)folha;
                        pai->subPtr[1] = (noDaArvore*)sibling;
                        pai->filhos = 2;
                        
                        arvore->raiz = (noDaArvore*)pai;
                        arvore->cabeca[1] = (noDaArvore*)pai;
                        folha->pai = pai;
                        sibling->pai = pai;
                } else {
                        sibling->pai = pai;
                        return inserirNaPagina(arvore, pai, (noDaArvore*)sibling, sibling->chaves[0], 1);
                }
        }

        return 0;
}

int inserirNaArvore(arvoreBMais *arvore, int chave, int dado)
{
        int i;
        noDaArvore *no = arvore->raiz;
        paginaInterna *naoFolha;
        paginaFolha *folha, *raiz;

        while (no != NULL) {
                switch (no->tipo) {
                case 0:
                        folha = (paginaFolha*)no;
                        return inserirNaFolha(arvore, folha, chave, dado);  
                case 1:
                        naoFolha = (paginaInterna*)no;
                        i = buscaBinaria(naoFolha->chaves, naoFolha->filhos - 1, chave);
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

        raiz = criarFolha();
        raiz->chaves[0] = chave;
        raiz->dados[0] = dado;
        raiz->registros = 1;
        arvore->cabeca[0] = (noDaArvore*)raiz;
        arvore->raiz = (noDaArvore*)raiz;

        return 0;
}


//CHAMADAS DE FUNCOES BASICAS

int buscarElemento(arvoreBMais *arvore, int chave)
{
    int dado = buscarNaArvore(arvore, chave); 
    if (dado) {
        printf("Elemento encontrado!\n");
        return dado;
    } else {
        printf("Elemento nao encontrado!\n");
        return -1;
    }
}

int inserirElemento(arvoreBMais *arvore, int chave, int dado)
{
    if (dado) {
        return inserirNaArvore(arvore, chave, dado);
    } 
}

int removerElemento(arvoreBMais *arvore, int chave)
{
      //return bplus_arvore_delete(arvore, chave);
}

arvoreBMais *inicializarArvore(int nivel, int ordem, int registros)
{ 
    assert(maxOrdem > minOrdem);
    assert(nivel <= maxNivel && ordem <= maxOrdem && registros <= maxRegistro);

    arvoreBMais *arvore = malloc(sizeof(*arvore));
    if (arvore != NULL) {
        arvore->raiz = NULL;
        arvore->nivel = nivel;
        arvore->ordem = ordem;
        arvore->registros = registros;
        memset(arvore->cabeca, 0, maxNivel * sizeof(noDaArvore*));
    }

    return arvore;
}

void destruirArvore(arvoreBMais *arvore) {
        free(arvore);
}

void exibirArvore(arvoreBMais *arvore)
{
    int i, j;

    for (i = arvore->nivel - 1; i > 0; i--) {
        paginaInterna *naoFolha = (paginaInterna*)arvore->cabeca[i];
        if (naoFolha != NULL) {
            printf("Nivel %d:\n", i);
            while (naoFolha != NULL) {
                printf("Pagina: ");
                for (j = 0; j < naoFolha->filhos - 1; j++) {
                    printf("%d ", naoFolha->chaves[j]);
                }
                printf("\n\n");
                naoFolha = naoFolha->proximo;
            }
        }
    }

    paginaFolha *folha = (paginaFolha*)arvore->cabeca[0];
    if (folha != NULL) {
        printf("Nivel 0:\n");
        while (folha != NULL) {
            printf("Pagina Folha: ");
            for (j = 0; j < folha->registros; j++) {
                printf("%d ", folha->chaves[j]);
            }
            printf("\n");
            folha = folha->proximo;
        }
    } else {
        printf("Arvore vazia!\n\n");
    }
}


//-----------------------------------------------------------
//FUNCIONAMENTO DO PROGRAMA

//>>>>>INSERCAO 
void InsereArquivoArvore(arvoreBMais *arvore, FILE* arquivo){  
  int dado;
  
  while(fscanf(arquivo, "%d", &dado) != EOF){        
    inserirElemento(arvore, dado, dado);
  }
}

void InsereNaArvoreBMais(arvoreBMais *arvore, char* string){
  FILE* arquivo;
  
  arquivo = fopen(string,"r");
  if(!arquivo){
    printf("Erro ao abrir o arquivo %s! Encerrando Programa...\n", string);
    exit(1);
  }
  
  configuracaoArvore configuracao;
  configuracao.nivel = 5;
  configuracao.ordem = 7;
  configuracao.registros = 10;

  arvore = inicializarArvore(configuracao.nivel, configuracao.ordem, configuracao.registros);
  if (arvore == NULL) {
      printf("Falha ao inicializar!\n");
      exit(1);
  }

  InsereArquivoArvore(arvore, arquivo);
  printf("Pronto!\n");

  printf("\n>>> Arvore atual \n\n");
  exibirArvore(arvore);

  fclose(arquivo);
}

//>>>>>REMOCAO

void RemoveArquivoArvore(arvoreBMais *arvore, FILE* arquivo){    
  int valor;
  
  /*while(fscanf(arquivo, "%d%*c", &valor) == 1){            
    printf("%d %p\n", valor,arvore->raiz);
    printf("%d\n", RemoverDaArvoreRN(arvore, valor));    
  }*/
  
}

//---------------------------------------------------------------------
//MENU PRINCIPAL

//Exibe um de menu para o usuario
int Menu() {
  int opcao;
  printf("\n-------------> Arvore B+ <-------------\n\n");
  printf("    ----->>>>> MENU <<<<<-----\n\n");
  printf("1 - Inserir na Arvore\n");
  printf("2 - Remover da Arvore\n");
  printf("3 - Busca na Arvore\n");
  printf("4 - Visualizar a Arvore\n");
  printf("0 - Sair do programa\n");
  printf("\n>>>>> Digite sua opcao: ");
  scanf("%d", &opcao);
  
  return opcao;
}

//Exibe um menu para insercao na arvore
void MenuParaInsercao(arvoreBMais* arvore){
  char string[30];
  
  printf("\n    ----------- Insercao -----------\n\n");
  printf("Digite o nome do arquivo ou local que contem\nos valores a serem inseridos na arvore:\n");
  scanf("%s", string);
  printf("\nInserindo...\n");
  InsereNaArvoreBMais(arvore, string);

  fflush(stdin);
  getchar();
}
 
//Exibe um menu para remocao da arvore
void MenuParaRemocao(arvoreBMais* arvore){
  FILE* arquivo;
  char string[30];
  
  printf("\n    ----------- Remocao -----------\n\n");
  printf("Digite o nome do arquivo ou local que contem\nos valors a serem removidos da arvore:\n");
  scanf("%s", string);
  printf("\nRemovendo...\n");
  arquivo = fopen(string,"r");
  if(!arquivo){
    printf("Erro ao abrir o arquivo %s! Encerrando Programa...\n", string);
    exit(1);
  }
  RemoveArquivoArvore(arvore, arquivo);
  printf("\nPronto!\n");
  fflush(stdin);
  getchar();
  
}

//Exibe um menu para busca na arvore
void MenuParaBusca(arvoreBMais *arvore){
  int chave;
  
  printf("\n    ----------- Busca -----------\n\n");
  printf("Digite o valor que se deseja buscar: ");
  scanf("%d*c", &chave);
  printf("\nBuscando...\n");
  
  buscarElemento(arvore, chave);
  
  fflush(stdin);
  getchar();
}

void MostrarArvore(arvoreBMais *arvore) {
 
  exibirArvore(arvore);
  fflush(stdin);
  getchar();
}

int main(){
  int opcao = 1, sair = 0;

  while (sair != 1) {
    opcao = Menu();
    switch(opcao) {
      case 1:
        MenuParaInsercao(&arvore);
        break;
      case 2:
        MenuParaRemocao(&arvore);
        break;
      case 3:
        MenuParaBusca(&arvore);
        break;
      case 4:
        MostrarArvore(&arvore);
        break;
      case 0:
        sair = 1;
        break;
      default:
        printf("Comando invalido! Tente novamente.\n");
        break;
    }
  }

  return 0;
}
