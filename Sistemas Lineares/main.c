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
        int qtd;
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

void freeMatriz (void** mat, int tamanho) {
    int i;
    for(i = 0; i < tamanho; i++)
        free(*(mat + i));
    free(mat);
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
int insira (Lista* lis, void* nInfo) {
    //N permite intens repetidos;
    if(jaTem(lis, nInfo))
        return 0;

    if(lis -> inicio == NULL){
        lis -> inicio = (No*)malloc(sizeof(No));
        lis -> inicio -> info = nInfo;
        lis -> inicio -> prox = NULL;
        lis -> inicio -> ant = NULL;
        lis -> ult = lis -> inicio;
        lis -> qtd++;
        return 1;
    }

    No* aux = (No*)malloc(sizeof(No));
    aux -> info = nInfo;
    aux -> prox = NULL;
    aux -> ant = lis -> ult;
    lis -> ult -> prox = aux;
    lis -> ult = lis -> ult -> prox;
    lis -> qtd++;

    return 1;
}

int remova (Lista* lis, void* rInfo) {
    No* aux = lis-> inicio;

    if(aux == NULL)
        return 0;

    if(lis -> equals(aux -> info, rInfo)){
        lis -> inicio = NULL;
        lis -> qtd--;
        return 1;
    }

    aux = aux -> prox;
    while(aux != NULL){
        if(lis -> equals(aux -> info, rInfo)){
            aux -> ant -> prox = aux -> prox;
            free(aux);
            return 1;
        }
        aux = aux -> prox;
        lis -> qtd--;
    }

    //Se chegar aqui, não há nada a se remover;
    return 0;
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
        return NULL;

    void* ret;
    int count = 0;
    No* aux = lis -> inicio;
    while(aux != NULL){
        if(count == pos)
            break;
        aux = aux -> prox;
        count++;
    }

    if(count == pos){
        ret = (void*)aux -> info;
        return ret;
    }

    ret = NULL;
    return ret;

}

int getPos(Lista* lis, void* nInfo)
{
    if(!jaTem(lis, nInfo))
        return -1;

    int cont = 0;
    No* aux = lis->inicio;
    while(aux != NULL)
    {
        if(lis -> equals(aux -> info, nInfo))
            break;
        aux = aux -> prox;
        cont++;
    }

    return cont;
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
        //Descarta depois de usar;
        freeMatriz((void**)mAux, ordem -1);
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

//Recebe um ponteiro de Sistema pelo parâmetro e devolve um vetor com os valores das icógnitas em ordem;
float* resolverSistema (Sistema* sis){
    int i;
    float determinanteC;
    float determinanteIcog;
    float** aux;
    float* ret = (float*)malloc(sis -> qtdIcog * sizeof(float));

    determinanteC = det(sis -> matrizCoeficientes, sis -> qtdIcog);
    for(i = 0; i < sis -> qtdIcog; i++){
        aux = matrizIcognita(sis -> matrizCoeficientes, sis -> linhaResultados, i+1, sis -> qtdIcog);
        determinanteIcog = det(aux, sis->qtdIcog);
        free(aux);//Lembrar de descartar o que não for mais usada;
        ret[i] = determinanteC/determinanteIcog;
    }

    return ret;
}

void printarSistema (Sistema* sis){
    int i;
    Lista* lisE = sis -> lisEqua;

    printf("\n{\n");
    for(i = 0; i < sis -> qtdIcog; i++)
        printf("%s\n", (char*)getElemento(lisE, i));
    printf("}");

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
    *(concat+i-1) = '\0';
    return concat;
}

Lista* separarEquacoes(char* nome, Sistema* sis)
{
    FILE* arq = fopen(nome, "r");
    char* texto = lerArquivo(arq);

    Lista* lis = (Lista*)malloc(sizeof(Lista));
    lis->inicio = NULL;
    lis->ult = NULL;
    lis->qtd = 0;
    lis -> equals = (int(*)(void*,void*))&equalsStr;
    lis->toString = (char*(*)(void*))&toStringStr;

    char* s = (char*)malloc(sizeof(char)*1024);
    char* equacao = (char*)malloc(sizeof(char)*1024);
    strcpy(s, texto);
    equacao = strtok(s, "\n");
    while (equacao)
    {
        insira(lis, (void*)equacao);
        equacao = strtok(NULL, "\n");
    }
    fclose(arq);
    return lis;
}

char* limparString (char* str, int qtd) {
    int i;
    for(i = 0; i < qtd; i++)
        *(str+i) = "";
    *(str + qtd) = "";

    return str;
}

void extrairCoeficientes(Sistema* sis, char* nome)
{
    sis->lisEqua = separarEquacoes(nome, sis);
    sis -> qtdIcog = sis -> lisEqua -> qtd;

    Lista* lis = (Lista*)malloc(sizeof(Lista)); //INICIALIZACAO DA LISTA DAS VARIAVEIS
    lis->inicio = NULL;
    lis->ult = NULL;
    lis->qtd = 0;
    lis -> equals = (int(*)(void*,void*))&equalsStr;
    lis->toString = (char*(*)(void*))&toStringStr;

    Lista* lisR = (Lista*)malloc(sizeof(Lista)); //INICIALIZACAO DA LISTA DOS RESULTADOS
    lisR->inicio = NULL;
    lisR->ult = NULL;
    lisR->qtd = 0;
    lisR -> equals = (int(*)(void*,void*))&equalsStr;
    lisR->toString = (char*(*)(void*))&toStringStr;

    char* VAZIO = (char*)malloc(sizeof(char)*10);
    strcpy(VAZIO, "");

    int i;
    int j;
    int n;
    int inseriu = 0; // variavel boolean para saber foi inserido um item em uma lista
    int posVariavel = 0; //variavel para controlar quantas variaveis ja foram inseridas na lista
    int cont = 0;//Contador reponsável por inserir os caracteres nas variáveis
    char c;//Caracter lido da equação
    int ehCo = 1;//variável para controlar se um numero é ou nao coeficiente
    char* equacao = (char*)malloc(sizeof(char)*1024);

    char** variavel = (char**)malloc(sizeof(char*) * sis -> qtdIcog);
    for(i = 0; i < sis -> qtdIcog; i++){
        variavel[i] = (char*)malloc(sizeof(char)*100);
        variavel[i][0] = '\0';
    }

    char** resultado = (char**)malloc(sizeof(char*)* sis -> qtdIcog);
    for(i = 0; i < sis -> qtdIcog; i++){
        resultado[i] = (char*)malloc(sizeof(char)* 100);
        resultado[i] = '\0';
    }

    /*char** coeficiente = (char*)malloc(sizeof(char*)*sis->qtdIcog);
    for(i=0; i < sis -> qtdIcog; i++){
        coeficiente[i] = (char*)malloc(sizeof(char)*100);
        coeficiente[i] = '\0';
    }*/

    char* coeficiente = (char*)malloc(sizeof(char)*100);

    char* variavelCmp = (char*)malloc(sizeof(char)); //variavel para guardar temporariamente uma variavel

    sis->matrizCoeficientes = (float**)malloc(sis->lisEqua->qtd * sizeof(float*));
    for(i = 0; i < sis->lisEqua->qtd; i++)
       sis->matrizCoeficientes[i] = (float*)malloc(sis->lisEqua->qtd * sizeof(float));


     //PRIMEIRO FOR PARA DESCOBRIR AS VARIAVEIS
    for(i = 0; i < sis -> lisEqua -> qtd; i++)
    {
        //Já achou todas as icgnitas
        if(posVariavel >= sis -> qtdIcog)
            break;
        equacao = (char*)getElemento(sis->lisEqua,i);
        for(n = 0; n < strlen(equacao); n++)
        {
            c = equacao[n];
            //Quer dizer que o caracter lido eh uma letra
            if(c>=97 && c<=122)
            {
                variavel[posVariavel][cont] = c;
                cont++;
            }
            //Se o caracter for um espaço, acabou o nome da variável
            else if (cont && c == 32)
            {
                variavel[posVariavel][cont] = '\0';
                inseriu = insira(lis, (void*)variavel[posVariavel]);
                cont = 0;

                if(inseriu)
                    posVariavel++;
            }
        }
    }

    sis -> lisIcog = lis;
    printar(sis -> lisIcog);
    cont = 0;

    //Inicializando os componentes qiue serao usados no sistema
    sis -> linhaResultados = (float*)malloc(sizeof(float)*sis -> qtdIcog);
    sis -> matrizCoeficientes = (float**)malloc(sizeof(float*)*sis -> qtdIcog);
    for(i = 0; i < sis -> qtdIcog; i++)
        sis -> matrizCoeficientes[i] = (float*)malloc(sizeof(float) * sis -> qtdIcog);

    //SEGUNDO FOR PARA EXTRAIR OS COEFICIENTES E RESULTADOS
    for(i = 0; i < sis->qtdIcog; i++)
    {
        equacao = (char*)getElemento(sis->lisEqua,i);
        j = 0;
        for(n = 0; n < strlen(equacao); n++) //for que vai caracter por caracter da lista
        {
            c = equacao[n];

                //Se o caracter for um numero de coeficiente
                if(c >= 43 && c <= 57 && ehCo)
                {
                    coeficiente[cont] = c;
                    cont++;

                    //se o proximo for uma letra, termina a string do coeficiente
                    if(equacao[n+1]>=97){
                        coeficiente[cont] = '\0';
                        cont = 0;
                    }
                }
                //se o caracter for um numero do resultado
                else if(c >= 43 && c <= 57 && !ehCo)
                {
                    resultado[i][cont] = c;
                    cont++;
                }
                //Se for o ultimo caracter da equacao, le e insere o resultado
                if(n==strlen(equacao)-1)
                {
                    resultado[i][cont] = '\0';
                    insira(lisR, (void*)resultado[i]);
                    cont = 0;
                }

                //Caracter eh uma letra
                else if(c>=97 && c<=122)
                {
                    variavelCmp[cont] = c;
                    cont++;
                }
                //se for um igual sabemos que os próximos valores serão resultados
                else if(c==61)
                    ehCo = 0;
                //se for um espaço em branco
                else
                {   //se for um espaço depois de uma variavel insere o coeficiente
                    if(ehCo==1 && !equalsStr(variavelCmp, VAZIO))
                    {
                        variavelCmp[cont] = '\0';

                        j = getPos(lis, variavelCmp);
                        sis->matrizCoeficientes[i][j] = strtof(coeficiente[i], NULL);
                        cont = 0;
                        free(variavelCmp);
                        variavelCmp = (char*)malloc(sizeof(char)*100);
                    }
                }

            }

        }

        printf("\n");
        for(i = 0; i < sis -> qtdIcog; i++){
            for(j = 0; j < sis -> qtdIcog; j++)
                printf("%.2f ", sis -> matrizCoeficientes[i][j]);
            printf("\n");
        }

        printf("\n");
        for(i = 0; i < sis -> qtdIcog; i++)
            printf("%.2f ", sis -> linhaResultados[i]);
}

int main()
{

	FILE* arq;
    Sistema sis;
    sis.lisEqua = NULL;
    sis.lisIcog = NULL;
    sis.matrizCoeficientes = NULL;
    sis.qtdIcog = 0;


   char nome [1000];


    //printf("Digite o nome do arquivo: ");
    //scanf("%s", nome);
    nome[0] = 'a';
    nome[1] = '.';
    nome[2] = 't';
    nome[3] = 'x';
    nome[4] = 't';

    nome[5] = '\0';
    extrairCoeficientes(&sis, nome);

/*
    Lista a;
    a.inicio = NULL;
    a.ult = NULL;
    a.equals = (int(*)(void*, void*))&equalsStr;
    a.qtd = 0;
    a.toString = (char*(*)(void*))&toStringStr;

    char* aux = (char*)malloc(sizeof(char) * 255);
    strcpy(aux, "Pei");
    insira(&a, aux);

    printar(&a);*/

    return  0;
}


