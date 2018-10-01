#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef
    struct No {
        void* info;
        struct No* prox;
        struct No* ant;
    }No;

typedef //A lista nesse caso será desordenada devido a demanda do projeto;
    struct Lista {
        No* inicio;
        No* ult;
        char* (*toString) (void*);
        int (*equals) (void*, void*);
    }Lista;

char* toStringStr (char* str) {
    return str;
}

int equalsStr (char* a, char* b) {
    if(strcmp(a, b) == 0)
        return 1;

    return 0;
}

//Checa se já existe algum nó na lista com determinada informacao;
int jaTem (Lista* lis, void* rInfo) {
    No* aux = lis -> inicio;

    while (aux != NULL){
        if(lis -> equals(aux -> info, rInfo))
            return 1; //Verdadeiro
        aux = aux -> prox;
    }

    return 0;
}

//Insere no ultimo
void insira (Lista* lis, void* nInfo) {
    //N permite intens repetidos;
    if(jaTem(lis, nInfo))
        return;

    if(lis -> inicio == NULL){
        lis -> inicio = (No*)malloc(sizeof(No));
        lis -> inicio -> info = nInfo;
        lis -> inicio -> prox = NULL;
        lis -> inicio -> ant = NULL;
        lis -> ult = lis -> inicio;
        return;
    }

    No* aux = (No*)malloc(sizeof(No));
    aux -> info = nInfo;
    aux -> prox = NULL;
    aux -> ant = lis -> ult;
    lis -> ult -> prox = aux;
    lis -> ult = lis -> ult -> prox;

}

void remova (Lista* lis, void* rInfo) {
    No* aux = lis-> inicio;

    if(aux == NULL)
        return;

    if(lis -> equals(aux -> info, rInfo)){
        lis -> inicio = NULL;
        return;
    }

    aux = aux -> prox;
    while(aux != NULL){
        if(lis -> equals(aux -> info, rInfo)){
            aux -> ant -> prox = aux -> prox;
            free(aux);
            break;
        }
        aux = aux -> prox;
    }

    //Se chegar aqui, não há nada a se remover;
}

void printar (Lista* lis) {
    No* aux = lis -> inicio;

    char* ret = (char*)malloc(100000 * sizeof(char));
    strcpy(ret, "{");

    while(aux != NULL){
        strcat(ret, lis-> toString(aux -> info));
        aux = aux -> prox;
        if(aux != NULL){
            strcat(ret, ", ");
        }
    }
    strcat(ret, "}");

    printf("%s", ret);
}


void main (){

}












