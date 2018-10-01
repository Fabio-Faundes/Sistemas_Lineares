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

int main()
{
	FILE* arq;

	char c;

	char* linha = (char*)malloc(sizeof(char) * 1000);

    int linhas;
	string nome;
    printf("Digite o nome do arquivo: ");
    scanf(%s, nome);


	arq =fopen(nome , "r")

	if (!arq)
		return 1;

    fscanf(arq, "%d", &linhas);
    int i;
    for(i=0; i<=linhas;i++){
        fgets(linha, 1000, arq);
        for(int p = 0; p<=sizeof(linha); )
    }




	fclose(arq);

    return  0;
}

