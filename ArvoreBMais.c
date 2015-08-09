#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ArvoreBMais.h" //Biblioteca com as estruturas e funcoes

//CHAMADAS DE FUNCOES BASICAS
int buscarElemento(arvoreBMais *arvore, int chave) {
    int dado = buscarNaArvore(arvore, chave); 
    if (dado) {
        printf("Elemento encontrado!\n");
        return dado;
    } else {
        printf("Elemento nao encontrado!\n");
        return -1;
    }
}

int inserirElemento(arvoreBMais *arvore, int chave, int dado) {
    if (dado) {
        return inserirNaArvore(arvore, chave, dado);
    } 
}

int removerElemento(arvoreBMais *arvore, int chave) {
      return removerDaArvore(arvore, chave);
}

arvoreBMais *inicializarArvore(int nivel, int ordem, int registros) { 
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

void exibirArvore(arvoreBMais *arvore) {
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
void InsereArquivoArvore(arvoreBMais *arvore, FILE* arquivo) {  
  int dado;
  
  while(fscanf(arquivo, "%d", &dado) != EOF){        
    inserirElemento(arvore, dado, dado);
  }
}

void InsereNaArvoreBMais(arvoreBMais *arvore, char* string) {
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

void RemoveArquivoArvore(arvoreBMais *arvore, FILE* arquivo) {    
    int dado;

    while(fscanf(arquivo, "%d", &dado) != EOF){            
        removerElemento(arvore, dado);    
    }  

    printf("Pronto!\n");

    printf("\n>>> Arvore atual \n\n");
    exibirArvore(arvore);

    fclose(arquivo);
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
