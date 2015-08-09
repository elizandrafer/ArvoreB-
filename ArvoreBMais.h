#define minOrdem 3
#define maxOrdem 1024
#define maxRegistro 4096
#define maxNivel 10

//0 - Folha
//1 - Não Folha

typedef struct noDaArvore {

   int tipo;
   struct paginaInterna *pai;

}noDaArvore;

typedef struct arvoreBMais {

    int ordem;
    int registros;
    int nivel;
    struct noDaArvore *raiz;
    struct noDaArvore *cabeca[maxNivel];

}arvoreBMais;

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

typedef struct configuracaoArvore {
 
    int nivel;
    int ordem;
    int registros;  

}configuracaoArvore;

paginaInterna* criarPagina(void) {
    paginaInterna *no = malloc(sizeof(*no));
    
    assert(no != NULL);
    no->tipo = 1;  //não é folha
    
    memset(no->chaves, 0, (maxOrdem-1) * sizeof(int));
    memset(no->subPtr, 0, (maxOrdem) * sizeof(noDaArvore *));
    
    no->pai = NULL;
    no->proximo = NULL;
    no->filhos = 0;
    
    return no;
};

paginaFolha* criarFolha(void) {
    paginaFolha *folha = malloc(sizeof(*folha));

    assert(folha != NULL);
    folha->tipo = 0;   //é folha
    
    memset(folha->chaves, 0, maxRegistro * sizeof(int));
    memset(folha->dados, 0, maxRegistro * sizeof(int));
    
    folha->pai = NULL;
    folha->proximo = NULL;
    folha->registros = 0;

    return folha;
}

arvoreBMais *arvore;

void apagarFolha(paginaFolha *no){
  free(no);
}

void apagarPagina(paginaInterna *no) {
    free(no);
}

static int buscaBinaria(int *v, int tamanho, int valor) {
    int inicio = -1;
    int fim = tamanho;
    while (inicio + 1 < fim) {
        int meio = inicio + (fim - inicio) / 2;
        if (valor > v[meio]) {
            inicio = meio;
        } else {
            fim = meio;
        }
    }
    if (fim >= tamanho || v[fim] != valor) {
        return -fim - 1;
    } else {
        return fim;
    }
}


int buscarNaArvore(int chave) {
    int i;
    paginaInterna *naoFolha;
    paginaFolha *folha;
    noDaArvore *no = arvore->raiz;

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

int inserirNaPagina(paginaInterna *pagina, noDaArvore *noArv, int chave, int nivel)
{
    int i, j;
    int split = 0, split_chave;
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
                printf("Nivel excedido!\n");
                pagina->proximo = sibling->proximo;
                apagarPagina(sibling);
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
            return inserirNaPagina(pai, (noDaArvore*)sibling, split_chave, nivel+1);
        }
    }

    return 0;
}

int inserirNaFolha(paginaFolha *folha, int chave, int dado)
{
    int i, j;
    int split = 0;
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
            return inserirNaPagina(pai, (noDaArvore*)sibling, sibling->chaves[0], 1);
        }
    }

    return 0;
}

int inserirNaArvore(int chave, int dado) {
    int i;
    paginaInterna *naoFolha;
    paginaFolha *folha, *raiz;
    noDaArvore *no = arvore->raiz;

    while (no != NULL) {
        switch (no->tipo) {
        case 0:
            folha = (paginaFolha*)no;
            return inserirNaFolha(folha, chave, dado);  
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

void removerDaPagina(paginaInterna *no, int rem, int nivel) {
    
    int i, j, k;
    paginaInterna *sibling;

    if (no->filhos <= (arvore->ordem+1)/2) {
        paginaInterna *pai = no->pai;
        
        if (pai != NULL) {
            //Emprestar da esquerda -> emprestar = 0
            //Emprestar da direita -> emprestar = 1

            int emprestar = 0;

            i = buscaBinaria(pai->chaves, pai->filhos-1, no->chaves[0]);
            assert(i < 0);
            i = -i - 1;

            if (i == 0) {
                sibling = (paginaInterna *)pai->subPtr[i+1];
                emprestar = 1;
            } else if (i == pai->filhos-1) {
                sibling = (paginaInterna *)pai->subPtr[i-1];
                emprestar = 0;
            } else {
                paginaInterna *sibEsq = (paginaInterna *)pai->subPtr[i-1];
                paginaInterna *sibDir = (paginaInterna *)pai->subPtr[i+1];
                    
                if(sibEsq->filhos >= sibDir->filhos){
                    sibling = sibEsq;
                    emprestar = 0;
                }
                else {
                    sibling = sibDir;
                    emprestar = 1;
                }
            }

            if (i > 0) {
                i = i - 1;
            }

            if (emprestar == 0) {
                if (sibling->filhos > (arvore->ordem+1)/2) {
                    for (j = rem; j > 0; j--) {
                        no->chaves[j] = no->chaves[j-1];
                    }
                    for (j = rem + 1; j > 0; j--) {
                        no->subPtr[j] = no->subPtr[j-1];
                    }
                    no->chaves[0] = pai->chaves[i];
                    pai->chaves[i] = sibling->chaves[sibling->filhos-2];
                    no->subPtr[0] = sibling->subPtr[sibling->filhos-1];
                    sibling->subPtr[sibling->filhos-1]->pai = no;
                    sibling->filhos--;
                } else {
                    sibling->chaves[sibling->filhos-1] = pai->chaves[i];
                                
                    for (j = sibling->filhos, k = 0; k < no->filhos-1; k++) {
                        if (k != rem) {
                            sibling->chaves[j] = no->chaves[k];
                            j++;
                        }
                    }
                    for (j = sibling->filhos, k = 0; k < no->filhos-1; k++) {
                        if (k != rem + 1) {
                            sibling->subPtr[j] = no->subPtr[k];
                            no->subPtr[k]->pai = sibling;
                            j++;
                        }
                    }
                    sibling->filhos = j;
                                
                    sibling->proximo = no->proximo;
                    apagarPagina(no);
                    removerDaPagina(pai, i, nivel + 1);
                }
            } else {
                while(rem < no->filhos-2) {
                    no->chaves[rem] = no->chaves[rem+1];
                    no->subPtr[rem+1] = no->subPtr[rem+2];
                    rem++;
                }
                no->filhos--;
                if (sibling->filhos > (arvore->ordem + 1) / 2) {
                    no->chaves[no->filhos-1] = pai->chaves[i];
                    pai->chaves[i] = sibling->chaves[0];
                    no->filhos++;
                            
                    no->subPtr[no->filhos-1] = sibling->subPtr[0];
                    sibling->subPtr[0]->pai = no;
                            
                    for (j = 0; j < sibling->filhos-2; j++) {
                        sibling->chaves[j] = sibling->chaves[j+1];
                    }
                    for (j = 0; j < sibling->filhos-1; j++) {
                        sibling->subPtr[j] = sibling->subPtr[j+1];
                    }
                    sibling->filhos--;
                } else {
                    no->chaves[no->filhos-1] = pai->chaves[i];
                    no->filhos++;
                            
                    for (j = no->filhos-1, k = 0; k < sibling->filhos-1; j++, k++) {
                        no->chaves[j] = sibling->chaves[k];
                    }
                    for (j = no->filhos-1, k = 0; k < sibling->filhos; j++, k++) {
                        no->subPtr[j] = sibling->subPtr[k];
                        sibling->subPtr[k]->pai = no;
                    }
                    no->filhos = j;
                            
                    no->proximo = sibling->proximo;
                    apagarPagina(sibling);
                    removerDaPagina(pai, i, nivel+1);
                  }
            }
            return;
        } else {
          if (no->filhos == 2) {
              assert(rem == 0);
              no->subPtr[0]->pai = NULL;
              arvore->raiz = no->subPtr[0];
              arvore->cabeca[nivel] = NULL;
              apagarPagina(no);
              return;
          }
        }
    }
        
    assert(no->filhos > 2);
    
    while(rem < no->filhos-2){
        no->chaves[rem] = no->chaves[rem+1];
        no->subPtr[rem+1] = no->subPtr[rem+2];
        rem++;
    }

    no->filhos--;
}

int removerDaFolha(paginaFolha *folha, int chave) {
    
    int i, j, k;
    paginaFolha *sibling;

    int rem = buscaBinaria(folha->chaves, folha->registros, chave);
    if (rem < 0) {
            return -1;
    }

    if (folha->registros <= (arvore->registros+1)/2) {
        paginaInterna *pai = folha->pai;
        if (pai != NULL) {

            //Emprestar da Esquerda -> emprestar = 0
            //Emprestar da Direita -> emprestar = 1

            int emprestar = 0;
            
            i = buscaBinaria(pai->chaves, pai->filhos-1, folha->chaves[0]);
            if (i >= 0) {
                i = i + 1;
                if (i == pai->filhos-1) {
                    sibling = (paginaFolha *)pai->subPtr[i-1];
                    emprestar = 0;
                } else {
                    paginaFolha *sibEsq = (paginaFolha *)pai->subPtr[i-1];
                    paginaFolha *sibDir = (paginaFolha *)pai->subPtr[i+1];
                   
                    if(sibEsq->registros >= sibDir->registros){
                        sibling = sibEsq;
                        emprestar = 0;
                    }
                    else {
                        sibling = sibDir;
                        emprestar = 1;
                    }  

                }
            } else {
                assert(i == -1);
                i = -i - 1;
                sibling = (paginaFolha *)pai->subPtr[i+1];
                emprestar = 1;
            }

            if (i > 0) {
                i = i - 1;
            }

            if (emprestar == 0) {
                if (sibling->registros > (arvore->registros + 1) / 2) {
                    pai->chaves[i] = sibling->chaves[sibling->registros-1];
                    
                    while(rem > 0){
                        folha->chaves[rem] = folha->chaves[rem-1];
                        folha->dados[rem] = folha->dados[rem-1];
                        rem--;
                    }

                    folha->chaves[0] = sibling->chaves[sibling->registros-1];
                    folha->dados[0] = sibling->dados[sibling->registros-1];
                    sibling->registros--;
                    pai->chaves[i] = folha->chaves[0];

                } else {
                    for (j = sibling->registros, k = 0; k < folha->registros; k++) {
                        if (k != rem) {
                            sibling->chaves[j] = folha->chaves[k];
                            sibling->dados[j] = folha->dados[k];
                            j++;
                        }
                    }
                    sibling->registros = j;
                    
                    sibling->proximo = folha->proximo;
                    apagarFolha(folha);
                    removerDaPagina(pai, i, 1);
                }
            } else {
                while(rem < folha->registros-1){
                    folha->chaves[rem] = folha->chaves[rem + 1];
                    folha->dados[rem] = folha->dados[rem + 1];
                    rem++;
                }
                folha->registros--;
                if (sibling->registros > (arvore->registros + 1) / 2) {
                    
                    folha->chaves[folha->registros] = sibling->chaves[0];
                    folha->dados[folha->registros] = sibling->dados[0];
                    folha->registros++;
                    
                    for (j = 0; j < sibling->registros - 1; j++) {
                        sibling->chaves[j] = sibling->chaves[j + 1];
                        sibling->dados[j] = sibling->dados[j + 1];
                    }
                    sibling->registros--;
                    
                    pai->chaves[i] = sibling->chaves[0];
                } else {                    
                    for (j = folha->registros, k = 0; k < sibling->registros; j++, k++) {
                        folha->chaves[j] = sibling->chaves[k];
                        folha->dados[j] = sibling->dados[k];
                    }
                    folha->registros = j;
                    
                    folha->proximo = sibling->proximo;
                    apagarFolha(sibling);
                    removerDaPagina(pai, i, 1);
                }
            }
            
            return 0;
        } else {
            if (folha->registros == 1) {
                assert(chave == folha->chaves[0]);
                arvore->raiz = NULL;
                arvore->cabeca[0] = NULL;
                apagarFolha(folha);
                return 0;
            }
        }
    }

    while(rem < folha->registros-1){
        folha->chaves[rem] = folha->chaves[rem + 1];
        folha->dados[rem] = folha->dados[rem + 1];
        rem++;
    }
    folha->registros--;
    
    return 0;
}

static int removerDaArvore(int chave)
{
    int i;
    paginaInterna *naoFolha;
    paginaFolha *folha;
    noDaArvore *no = arvore->raiz;

    while (no != NULL) {
        switch (no->tipo) {
        case 0: //folha
            folha = (paginaFolha*)no;
            return removerDaFolha(folha, chave);

        case 1: //nao-folha
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

    return -1;
}
