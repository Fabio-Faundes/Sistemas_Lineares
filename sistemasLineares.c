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

typedef
    struct Sistema {
        int qtdIcog;
        Lista* lisIcog;
        float** matrizCoeficientes;
        float* linhaResultados;
    }Sistema;

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

void* getElemento (Lista* lis, int pos) {
    if(lis -> inicio == NULL)
        return;

    void* ret;
    int count = 0;
    No* aux = lis -> inicio;
    while(aux != NULL){
        if(count == pos)
            break;
        aux = aux -> prox;
    }

    if(count == pos){
        ret = aux -> info;
        return ret;
    }

    ret = NULL;
    return ret;

}

//i é a linha e j é a coluna
float** formarComplementar (float** matriz, int i, int j, int ordem) {
int a, b, c = 0, d = 0;

    //alocando memória para a matriz que será utilizada
    float** mComplementar = (float**)malloc((ordem-1)*sizeof(float*));
    for(a = 0; a < ordem-1; a++)
        *(mComplementar+a) = (float*)malloc((ordem -1)*sizeof(float));

    //aqui teremos que percorrer a matriz original inteiramente
    // a é a linha da matriz original e b é a coluna;
    //c é a linha da matriz complemento e d é a coluna;
    for(a = 0; a < ordem; a++){
        for(b = 0; b < ordem; b++){
            //Significa que não é uma das linhas a serem descartadas
            if(a != i -1 && b != j -1){
                *(*(mComplementar + c)+ d) = *(*(matriz + a) + b);

                //temos que resetar
                if(c == ordem -2){
                    c = 0;
                    d++;
                    break;
                }
                else{
                    c++;
                }

            }
            else{}//Não faz nada, o for já incrementará a posição da matriz original;
        }
        //quer dizer que acabou;
        if(d == ordem -1)
            break;
    }

    return mComplementar;
}

//Método para calcular determinante
float det (float** matriz, int ordem){
    float* linha;
    float** mAux;
    int i;
    int j=0;
    float determinante = 0;
    int aux;
    //Eh uma matriz de ordem 1 portanto tem somente um elemento, o primeiro
    //A determinante eh igual ao elemento;
    if(ordem == 1){
        return *(*matriz);
    }


    //usaremos a primeira linha da matriz para calcular
    linha = (float*)malloc(ordem * sizeof(float));
    for(i = 0; i < ordem; i++){
        *(linha + i) = *(*matriz + i);
    }
    i = 0;


    for(j = 0; j < ordem; j++){
        if((i + j) % 2 == 0)
            aux = 1;
        else
            aux = -1;
        mAux = formarComplementar(matriz,i+1 , j+1,ordem);
        determinante += linha[j] * aux * det(mAux, ordem-1);
        free(mAux);//Descarta depois de usar;
    }

    free(linha);
    return determinante;

}

//Forma a matriz da icógnita no Teorema de Cramer;
float** matrizIcognita (float** coeficientes, float* resultados, int pos, int qtd){
    int i, j, a = 0;

    float** ret = (float**)malloc(qtd * sizeof(float*));
    for(i = 0; i < qtd; i++)
        ret[i] = (float*)malloc(qtd * sizeof(float));

    for(i = 0; i < qtd; i++){
        for(j = 0; j < qtd; j++){
            if(j == pos -1){ //Se estiver na posição correspondente, colocamos o valor dos resultados;
                ret[i][j] = resultados[a];
                a++;
            }
            else//Se não, copia da matriz dos coeficientes;
                ret[i][j] = coeficientes[i][j];
        }
    }

    return ret;
}

/*
//Recebe um ponteiro de Sistema pelo parâmetro e devolve um vetor com os valores das icógnitas em ordem;
float* resolverSistema (Sistema* sis){
    int i, j;
    float determinateC;
    float determinateIcog;
    float** aux;
    float* ret = (float*)malloc(sis -> qtdIcog * sizeof(float));;
    float* ret = (float*)malloc(sis -> qtdIcog * sizeof(float));

    determinanteIcog = (float*)malloc(sis -> qtdIcog * sizeof(float));

    determinateC = det(sis -> matrizCoeficientes, sis -> qtdIcog);
    for(i = 0; i < sis -> qtdIcog; i++){
        aux = matrizIcognita(sis -> matrizCoeficientes, sis -> linhaResultados, i+1, sis -> qtdIcog);
        determinateIcog = det(aux, sis->qtdIcog);
        free(aux);//Lembrar de descartar o que não for mais usada;
        ret[i] = determinateC/determinateIcog;
    }

    return ret;
}

void printarSistema (Sistema* sis){
    int i, j;
    Lista* lis = sis -> lisIcog;

    printf(\n"{"\n);
    for(i = 0; i < sis -> qtdIcog;){//Printa a linha;
        printf("\t");//Devemos dar um espço no começo da linha;
        for(j = 0; j < sis -> icog; j++){//Printa as colunas;
            printf("%2.f", sis -> coeficientes[i][j]);

            char* a = (char*)getElemento(lis, j);
            printf("%s", a);

            if( j + 1 < sis -> icog)//Significa que tem mais icognitas;
                if(sis -> coeficientes[i][j+1] < 0)
                    printf(" ");//Deixa espaço em branco pois o coeficiente já terá um "-";
                else
                    printf(" +");
        }
        printf(" = ");
        printf("%2.f\n", sis -> linhaResultados[i]);
    }

}

void printarResultado (Sistema* sis) {
    int i;
    Lista* lis = sis -> lisIcog;
    float* resolucao = resolverSistema(sis);

    printf("\nSolução: ");
    for(i = 0; i < sis -> qtdIcog; i++){
        char * a = (char*)getElemento(lis, i);
        printf("%s = %2.f", a, resolucao[i]);

        if(i + 1 < sis -> qtdIcog)
            printf(", ");
    }

    printf(".");

}

*/

char* lerArquivo(FILE* arq)
{
    int linhas;
    fscanf(arq, "%d", &linhas);
    printf("%d", linhas);

    fseek(arq, 0, SEEK_END);
    long int buffer_size = ftell(arq);
    fseek(arq, 1, SEEK_SET);

    char* concat = (char*)malloc(buffer_size);

    int i = 0;
    while (!feof(arq)) {
        *(concat+i) = fgetc(arq);
        if()

        i++;
        //if (c >= 43 && c <= 57)
    }

    printf("%s", concat);
}

int main()
{
	FILE* arq;

	char c;

	char* nome = (char*)malloc(sizeof(char) * 1000);


    printf("Digite o nome do arquivo: ");
    scanf("%s", nome);

    arq = fopen(nome, "r");
	if (!arq)
		return 1;

    lerArquivo(arq);

	fclose(arq);

    return  0;
}
