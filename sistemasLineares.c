#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef
    struct No {
        void* info;
        struct No* prox;
        struct No* ant;
    }No;

typedef
    struct Lista {
        No* inicio;
        No* ult;
        void* (printItem*) (void*);
    }Lista;

typedef
    struct Incognita{

    }Incognita;

void insira ()

void remova (Lista* lis, void* rInfo) {
    No* aux = lis -> inicio;

    if(aux == NULL)
        return;

    if(lis -> compareTo(aux -> info, rInfo) == 0) {
        lis -> inicio = aux -> prox;
        free(aux);
        return;
    }

    No* aux_prox;
    for (;;){
        //Chegou ao último e não achou;
        if (aux -> prox == NULL)
            return;

        aux_prox = aux -> prox;
        int comparacao = lis -> compareTo(aux_prox -> info, rInfo);
        //Deve-se remover
        if(comparacao == 0)
            break;

        //Não achou o item a ser removido
        if(comparacao < 0)
            return;

        aux = aux -> prox;
    }

    aux -> prox = aux_prox -> prox;
    free(aux_prox);

}
