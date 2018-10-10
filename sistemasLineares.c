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

}*/

char* lerArquivo(FILE* arq)
{
    int linhas;
    fscanf(arq, "%d", &linhas);
    printf("%d", linhas);

    char c[2];
    char concat[1000];

    c[1] = '\0';
    while (!feof(arq)) {
        c[0] = fgetc(arq);
        strcat(concat, &c);
        //if (c >= 43 && c <= 57)
    }

    printf("%s", concat);
}


int main()
{
	FILE* arq;

	char c;

	char* nome = (char*)malloc(sizeof(int) * 1000);


    printf("Digite o nome do arquivo: ");
    scanf("%s", nome);

    arq = fopen(nome, "r");
	if (!arq)
		return 1;

    lerArquivo(arq);

	fclose(arq);

    return  0;
}

