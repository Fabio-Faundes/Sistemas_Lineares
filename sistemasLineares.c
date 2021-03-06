#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef
    struct No {
        void* info;
        struct No* prox;
        struct No* ant;
    }No;

typedef //A lista nesse caso ser� desordenada devido a demanda do projeto;
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
        Lista* lisEqua;
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

//Checa se j� existe algum n� na lista com determinada informacao;
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

    //Se chegar aqui, n�o h� nada a se remover;
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

//i � a linha e j � a coluna
float** formarComplementar (float** matriz, int i, int j, int ordem) {
int a, b, c = 0, d = 0;

    //alocando mem�ria para a matriz que ser� utilizada
    float** mComplementar = (float**)malloc((ordem-1)*sizeof(float*));
    for(a = 0; a < ordem-1; a++)
        *(mComplementar+a) = (float*)malloc((ordem -1)*sizeof(float));

    //aqui teremos que percorrer a matriz original inteiramente
    // a � a linha da matriz original e b � a coluna;
    //c � a linha da matriz complemento e d � a coluna;
    for(a = 0; a < ordem; a++){
        for(b = 0; b < ordem; b++){
            //Significa que n�o � uma das linhas a serem descartadas
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
            else{}//N�o faz nada, o for j� incrementar� a posi��o da matriz original;
        }
        //quer dizer que acabou;
        if(d == ordem -1)
            break;
    }

    return mComplementar;
}

//M�todo para calcular determinante
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

//Forma a matriz da ic�gnita no Teorema de Cramer;
float** matrizIcognita (float** coeficientes, float* resultados, int pos, int qtd){
    int i, j, a = 0;

    float** ret = (float**)malloc(qtd * sizeof(float*));
    for(i = 0; i < qtd; i++)
        ret[i] = (float*)malloc(qtd * sizeof(float));

    for(i = 0; i < qtd; i++){
        for(j = 0; j < qtd; j++){
            if(j == pos -1){ //Se estiver na posi��o correspondente, colocamos o valor dos resultados;
                ret[i][j] = resultados[a];
                a++;
            }
            else//Se n�o, copia da matriz dos coeficientes;
                ret[i][j] = coeficientes[i][j];
        }
    }

    return ret;
}

//Recebe um ponteiro de Sistema pelo par�metro e devolve um vetor com os valores das ic�gnitas em ordem;
float* resolverSistema (Sistema* sis){
    int i, j;
    float determinanteC;
    float determinanteIcog;
    float** aux;
    float* ret = (float*)malloc(sis -> qtdIcog * sizeof(float));

    determinanteC = det(sis -> matrizCoeficientes, sis -> qtdIcog);
    for(i = 0; i < sis -> qtdIcog; i++){
        aux = matrizIcognita(sis -> matrizCoeficientes, sis -> linhaResultados, i+1, sis -> qtdIcog);
        determinanteIcog = det(aux, sis->qtdIcog);
        free(aux);//Lembrar de descartar o que n�o for mais usada;
        ret[i] = determinanteC/determinanteIcog;
    }

    return ret;
}

void printarSistema (Sistema* sis){
    int i, j;
    Lista* lis = sis -> lisIcog;

    printf("\n{\n");
    for(i = 0; i < sis -> qtdIcog;){//Printa a linha;
        printf("\t");//Devemos dar um esp�o no come�o da linha;
        for(j = 0; j < sis -> qtdIcog; j++){//Printa as colunas;
            printf("%2.f", sis -> matrizCoeficientes[i][j]);

            char* a = (char*)getElemento(lis, j);
            printf("%s", a);

            if( j + 1 < sis -> qtdIcog)//Significa que tem mais icognitas;
                if(sis -> matrizCoeficientes[i][j+1] < 0)
                    printf(" ");//Deixa espa�o em branco pois o coeficiente j� ter� um "-";
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

    printf("\nSolu��o: ");
    for(i = 0; i < sis -> qtdIcog; i++){
        char * a = (char*)getElemento(lis, i);
        printf("%s = %2.f", a, resolucao[i]);

        if(i + 1 < sis -> qtdIcog)
            printf(", ");
    }

    printf(".");

}



char* lerArquivo(FILE* arq)
{

    fseek(arq, 0, SEEK_END);
    long int buffer_size = ftell(arq);
    fseek(arq, 0, SEEK_SET);
    char* concat = (char*)malloc(buffer_size);
    int i = 0;
    while (!feof(arq)) {
        *(concat+i) = fgetc(arq);
        i++;
    }
    *(concat+i) = '\0';
    return concat;
}

void separarEquacoes(FILE* arq, Sistema* sis)
{
    char* texto = lerArquivo(arq);

    Lista* lis = (Lista*)malloc(sizeof(Lista));
    char* s = (char*)malloc(sizeof(char)*1024);
    char* equacao = (char*)malloc(sizeof(char)*1024);
    strcpy(s, texto);
    equacao = strtok(s, "\n");
    while (equacao)
    {
        insira(lis, (void*)equacao);
        equacao = strtok(NULL, "\n");
    }
    printar(lis);
    sis->lisEqua = &lis;
}

void extrairCoeficientes(Sistema* sis, char* nome)
{
    FILE* arq;
    arq = fopen(nome, "r");
    separarEquacoes(arq, sis);
    fclose(arq);
}

int main()
{
	FILE* arq;
    Sistema* sis;
    sis->qtdIcog = (int) malloc(sizeof(int));
    sis->qtdIcog =0;
    printf("a");
    sis->lisIcog = NULL;
    sis->lisEqua = NULL;
    sis-> matrizCoeficientes = NULL/*(float**)malloc(sizeof(float**))*/;
    sis->linhaResultados = 0;
    char* nome = (char*)malloc(sizeof(char) * 1000);


    printf("Digite o nome do arquivo: ");
    scanf("%s", &nome);
    extrairCoeficientes(sis, nome);


    return  0;
}

