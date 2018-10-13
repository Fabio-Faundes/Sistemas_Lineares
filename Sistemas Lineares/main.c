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
        lis -> qtd++;
        return;
    }

    No* aux = (No*)malloc(sizeof(No));
    aux -> info = nInfo;
    aux -> prox = NULL;
    aux -> ant = lis -> ult;
    lis -> ult -> prox = aux;
    lis -> ult = lis -> ult -> prox;
    lis -> qtd++;

}

void remova (Lista* lis, void* rInfo) {
    No* aux = lis-> inicio;

    if(aux == NULL)
        return;

    if(lis -> equals(aux -> info, rInfo)){
        lis -> inicio = NULL;
        lis -> qtd--;
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
        lis -> qtd--;
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
        if(aux->info == nInfo)
            break;
        aux = aux -> prox;
        count++;
    }

    return count;

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

//Recebe um ponteiro de Sistema pelo parâmetro e devolve um vetor com os valores das icógnitas em ordem;
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
        free(aux);//Lembrar de descartar o que não for mais usada;
        ret[i] = determinanteC/determinanteIcog;
    }

    return ret;
}

void printarSistema (Sistema* sis){
    int i, j;
    Lista* lis = sis -> lisIcog;

    printf("\n{\n");
    for(i = 0; i < sis -> qtdIcog;){//Printa a linha;
        printf("\t");//Devemos dar um espço no começo da linha;
        for(j = 0; j < sis -> qtdIcog; j++){//Printa as colunas;
            printf("%2.f", sis -> matrizCoeficientes[i][j]);

            char* a = (char*)getElemento(lis, j);
            printf("%s", a);

            if( j + 1 < sis -> qtdIcog)//Significa que tem mais icognitas;
                if(sis -> matrizCoeficientes[i][j+1] < 0)
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

void extrairCoeficientes(Sistema* sis, char* nome)
{
    sis->lisEqua = separarEquacoes(nome, sis);
    Lista* lis = (Lista*)malloc(sizeof(Lista));
    lis->inicio = NULL;
    lis->ult = NULL;
    lis->qtd = 0;
    lis -> equals = (int(*)(void*,void*))&equalsStr;
    lis->toString = (char*(*)(void*))&toStringStr;
    char c;
    int ehCo;
    char* equacao = (char*)malloc(sizeof(char)*1024);
    char* coeficiente = (char*)malloc(sizeof(char)*100);
    char* variavel = (char*)malloc(sizeof(char)*100);
    char* resultado = (char*)malloc(sizeof(char)*100);
    char* vazio = (char*)malloc(sizeof(char)*100);
    int i;
    int n;
    int cont = 0;
    int j;

    sis->matrizCoeficientes = (float**)malloc(sis->lisEqua->qtd * sizeof(float*));
    for(i = 0; i < sis->lisEqua->qtd; i++)
       sis->matrizCoeficientes[i] = (float*)malloc(sis->lisEqua->qtd * sizeof(float));

    *(vazio)="";
    *(coeficiente)=*(vazio);
    *(variavel)=*(vazio);
    *(resultado)=*(vazio);
    ehCo = 1; //variavel para controlar se um numero é ou nao coeficiente

    for(i=0; i<sis->lisEqua->qtd; i++) //PRIMEIRO FOR PARA DESCOBRIR AS VARIAVEIS
    {
        equacao = (char*)getElemento(sis->lisEqua,i);
        int len = strlen(equacao);
        j = 0; //variavel para controlar a coluna da Matriz dos Coeficientes
        for(n=0;n<=len;n++)
        {
            c = *(equacao+n);

            if(c>=97 && c<=122)
            {
                *(variavel+cont) = c;
                cont++;
            }
            else if (c==32 && !equalsStr(variavel, vazio))
            {
                *(variavel+cont) = '\0';
                insira(lis, variavel); //ESSE INSIRA INSERE POR CIMA DO PRIMEIRO ELEMENTO, PQ??
                printar(lis);
                *(variavel+cont) = *(vazio+cont);
                int l;
                for(l=0; l<cont; l++)
                    *(variavel+l) = *(vazio+l);
                cont = 0;
            }
        }
    }



    for(i=0; i<=sis->lisEqua->qtd; i++)//SEGUNDO FOR PARA EXTRAIR OS COEFICIENTES E RESULTADOS
    {
        equacao = (char*)getElemento(sis->lisEqua,i);

        j = 0; //variavel para controlar a coluna da Matriz dos Coeficientes
        for(n=0;n<=strlen(equacao);n++) //for que vai caracter por caracter da lista
        {
            c = *(equacao+n);


                if(c>=43 && c<=57)
                {
                    if (equalsStr(resultado, vazio)==0 && ehCo != 1 && n==0)
                    {
                        *(resultado+cont) = '\0';
                        sis->linhaResultados[i-1] = strtof(resultado, NULL);
                        *(resultado+cont) = *(vazio+cont);
                        int l;
                        for(l=0; l<cont; l++)
                            *(resultado+l) = *(vazio+l);
                        ehCo = 1;
                        cont = 0;
                    }
                    if(ehCo == 1)
                    {
                        *(coeficiente+cont) = c;
                        cont++;
                    }
                    else
                    {
                        *(resultado+cont) = c;
                        cont++;
                    }
                }
                else if(c>=97 && c<=122)
                {
                    *(variavel+cont) = c;
                    cont++;
                }
                else if(c==61)
                    ehCo = 0;
                else
                {
                    if(ehCo==1)
                    {
                        if(equalsStr(coeficiente, vazio)==0)
                        {
                            *(coeficiente+cont) = '\0';
                            float a = strtof(coeficiente, NULL);
                            sis->matrizCoeficientes[i][j] = a;
                            j++;
                            *(coeficiente+cont) = *(vazio+cont);
                            int l;
                            for(l=0; l<cont; l++)
                                *(coeficiente+l) = *(vazio+l);
                            cont = 0;
                        }
                        *(variavel+cont) = '\0';
                        insira(lis, variavel);
                        *(variavel+cont) = *(vazio+cont);
                        int l;
                        for(l=0; l<cont; l++)
                            *(variavel+l) = *(vazio+l);
                        cont = 0;
                    }

                }

            }

        }

        printar(lis);

}
int main()
{
	FILE* arq;
    Sistema* sis = (Sistema*)malloc(sizeof(Sistema));
    sis->qtdIcog = 0;

    sis->lisEqua = NULL;
    sis->linhaResultados = 0;
    sis->lisIcog = NULL;


   char nome [1000];


    /*printf("Digite o nome do arquivo: ");
    scanf("%s", nome);*/
    nome[0] = 'a';
    nome[1] = '.';
    nome[2] = 't';
    nome[3] = 'x';
    nome[4] = 't';

    nome[5] = '\0';
    extrairCoeficientes(sis, nome);


    return  0;
}


