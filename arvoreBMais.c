#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//verificar a ordem D de modo que possa ser dinâmico.
#define ORDEM 4
#define bool char
#define true 1
#define false 0

typedef struct tipoRegistro{
	int valor;
}tipoRegistro;

//Estrutura da Arvore B+
typedef struct tipoPagina{
	
	int totalChaves;                            //quantidade de chaves na páginaDaArvore
	int chaves[ORDEM];                          //vetor de chaves inteiras
	struct tipoPagina *paginasFilhas[ORDEM+1];  //ponteiro para as paginas filhas
	
	int isFolha;                      //pergunta se é folha
	tipoRegistro *registros;          //registros das folhas
	struct tipoPagina *prox;          //proxima pagina folha

}tipoPagina;


<<<<<<< HEAD
  //ponteiro para as paginas filhas
  struct *paginaDaArvore paginaFilha[D];   
=======
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
>>>>>>> 25a726ec24c5efd2e9fddc13abe93d24bddfb2ae

//editar questoes de busca
tipoRegistro *buscar(tipoPagina *raiz, int chave, bool verbose) {
    int i = 0;
    tipoPagina *c = buscarFolha(raiz, chave, verbose);
    
    if (c == NULL) 
    	return NULL; 

    for (i = 0; i < c->totalChaves; i++){
        if (c->chaves[i] == chave) 
        	break;
    }

    if (i == c->totalChaves)
        return NULL;
    else
        return (tipoRegistro*)c->paginasFilhas[i];
}

tipoPagina *buscarFolha(tipoPagina *raiz, int chave, bool verbose) {
    int i = 0;

    tipoPagina *c = raiz;
    if (c == NULL) {
        if (verbose)
            printf("Arvore vazia.\n");
        return c;
    }
    while (!c->isFolha) {
        if (verbose) {
            printf("[");
            for (i = 0; i < c->totalChaves - 1; i++)
                printf("%d ", c->chaves[i]);
            printf("%d] ", c->chaves[i]);
        }
        i = 0;
        while (i < c->totalChaves) {
            if (chave >= c->chaves[i]) i++;
            else break;
        }
        if (verbose)
            printf("%d ->\n", i);
        c = (tipoPagina*)c->paginasFilhas[i];
    }
    if (verbose) {
        printf("Folha [");
        for (i = 0; i < c->totalChaves - 1; i++)
            printf("%d ", c->chaves[i]);
        printf("%d] ->\n", c->chaves[i]);
    }
    return c;
}
 
int inserirElemento(tipoPagina *raiz, int chave){
	tipoRegistro *reg;
	tipoPagina *folha;

	if (buscar(raiz, chave, false) != NULL)
        return raiz;	

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
