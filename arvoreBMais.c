#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//verificar a ordem D de modo que possa ser dinâmico.
#define ORDEM 4

typedef struct tipoRegistro{
	int valor;
}tipoRegistro;

//Estrutura da Arvore B+
typedef struct tipoPagina{
	
	int totalChaves;                  //quantidade de chaves na páginaDaArvore
	int chaves[ORDEM];                      //vetor de chaves inteiras
	struct tipoPagina *paginasFilhas[ORDEM+1];  //ponteiro para as paginas filhas
	
	int isFolha;                      //pergunta se é folha
	tipoRegistro *registros;         //registros das folhas
	struct tipoPagina *prox;          //proxima pagina folha

}tipoPagina;


int buscaBinaria(tipoDadosDePessoas *v, int tam, char nome[]) {
  int meio,inicio=0, fim=tam-1;
  int tmp;

  while(inicio <= fim) {
    meio = (inicio + fim)/2;
    tmp = strcmp(nome, v[meio].nome);
    if(tmp > 0) {
      inicio = meio+1;
    }
    else if (tmp < 0) {
      fim = meio -1;
    }
    else {
      return meio;
    }
  }
  return -1;
}

void main(){
	int i, n;
	void *c;

	scanf("%d", &n);
    c = malloc(sizeof(void*)*n);

    for (i = 0; i <= n; i++) {
        c[i] = &i;
    }

}
