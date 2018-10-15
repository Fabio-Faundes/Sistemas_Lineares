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

void freeNo (No* no) {
    if(no -> prox == NULL){
        free(no);
    }
    else{
        No* aux = no -> prox;
        freeNo(aux);
    }
}

void freeLista (Lista* lis){
    //freeNo(lis -> inicio);
    //freeNo(lis -> ult);
    free(lis -> inicio);
    free(lis -> ult);
    free(lis);
}

void freeSistema(Sistema* sis) {
    freeLista(sis -> lisIcog);
    freeLista(sis -> lisEqua);
    freeMatriz((void**)sis -> matrizCoeficientes, sis -> qtdIcog);
    free(sis -> linhaResultados);
    free(sis);
}

//inicia uma lista de string
Lista* initListaStr (){
    Lista* lis = (Lista*)malloc(sizeof(Lista));
    lis->inicio = NULL;
    lis->ult = NULL;
    lis->qtd = 0;
    lis -> equals = (int(*)(void*,void*))&equalsStr;
    lis->toString = (char*(*)(void*))&toStringStr;

    return lis;
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
    if(determinanteC == 0)
        return NULL;

    for(i = 0; i < sis -> qtdIcog; i++){
        aux = matrizIcognita(sis -> matrizCoeficientes, sis -> linhaResultados, i+1, sis -> qtdIcog);
        determinanteIcog = det(aux, sis->qtdIcog);
        freeMatriz((void**)aux, sis -> qtdIcog);//Lembrar de descartar o que não for mais usada;
        ret[i] = determinanteIcog/determinanteC;
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

    if(resolucao == NULL){
        printf("Sistema sem solucao definida, eh SPI ou SI");
        return;
    }

    printf("\nSolucao: ");
    for(i = 0; i < sis -> qtdIcog; i++){
        char * a = (char*)getElemento(lis, i);
        printf("%s = %.3f", a, resolucao[i]);

        if(i + 1 < sis -> qtdIcog)
            printf(", ");
    }

    printf(".\n");

}

char* lerArquivo(FILE* arq){
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

Lista* separarEquacoes(char* nome, Sistema* sis){
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

void extrairCoeficientes(Sistema* sis, char* nome){
    //Separando equacoes para facilitar o processo;
    sis->lisEqua = separarEquacoes(nome, sis);
    sis -> qtdIcog = sis -> lisEqua -> qtd;

    //INICIALIZACAO DA LISTA DAS VARIAVEIS
    Lista* lis = initListaStr();

    int i, j, n,
        inseriu = 0, // variavel boolean para saber foi inserido um item em uma lista
        posVariavel = 0, //variavel para contNolar quantas variaveis ja foram inseridas na lista
        cont = 0, //Contador reponsável por inserir os caracteres nas variáveis
        contN = 0, //Contador responsavel por inserir caracteres nos coeficientes
        ehCo = 1;//variável para contNolar se um numero é ou nao coeficiente

    char c;//Caracter lido da equação
    char* equacao = (char*)malloc(sizeof(char)*1024); //variavel para guardar uma das equacoes da lista de cada vez;
    char* resultado = (char*)malloc(sizeof(char)*100); //variavel para guardar o valor de um resultado;
    char* coeficiente = (char*)malloc(sizeof(char)*100); //variavel para guardar o valor de um coeficiente;
    char* variavelCmp = (char*)malloc(sizeof(char)*100); //variavel para guardar temporariamente o nome de uma icognita;

    //Vetor de strings utilizado para formar a lista de icognitas do sitema;
    char** variavel = (char**)malloc(sizeof(char*) * sis -> qtdIcog);
    for(i = 0; i < sis -> qtdIcog; i++){
        variavel[i] = (char*)malloc(sizeof(char)*100);
        variavel[i][0] = '\0';
    }

    //Inicializacao da matriz de coeficientes do sistema que será montado;
    sis->matrizCoeficientes = (float**)malloc(sis->lisEqua->qtd * sizeof(float*));
    for(i = 0; i < sis->lisEqua->qtd; i++)
       sis->matrizCoeficientes[i] = (float*)malloc(sis->lisEqua->qtd * sizeof(float));


     //PRIMEIRO FOR PARA DESCOBRIR O NOME DAS ICOGNITAS
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
    cont = 0;

    //Inicializando os componentes que serao usados no sistema
    sis -> linhaResultados = (float*)malloc(sizeof(float)*sis -> qtdIcog);
    sis -> matrizCoeficientes = (float**)malloc(sizeof(float*)*sis -> qtdIcog);
    for(i = 0; i < sis -> qtdIcog; i++)
        sis -> matrizCoeficientes[i] = (float*)malloc(sizeof(float) * sis -> qtdIcog);


    //SEGUNDO FOR PARA EXTRAIR OS COEFICIENTES E RESULTADOS
    for(i = 0; i < sis->qtdIcog; i++)
    {
        equacao = (char*)getElemento(sis->lisEqua,i);
        j = 0;
        ehCo = 1;
        for(n = 0; n < strlen(equacao); n++) //for que vai caracter por caracter da lista
        {
            c = equacao[n];

                //Se o caracter for um numero de coeficiente
                if(c >= 43 && c <= 57 && ehCo)
                {
                    coeficiente[contN] = c;
                    contN++;

                    //se o proximo for uma letra, termina a string do coeficiente
                    if(equacao[n+1]>=97){
                        coeficiente[contN] = '\0';
                        contN = 0;
                    }
                }
                //se o caracter for um numero do resultado
                else if(c >= 43 && c <= 57 && !ehCo)
                {
                    resultado[contN] = c;
                    contN++;
                }
                //Se for o ultimo caracter da equacao, le e insere o resultado
                if(n==strlen(equacao)-1)
                {
                    resultado[contN] = '\0';
                    sis->linhaResultados[i] = strtof(resultado, NULL);
                    contN = 0;

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
                    if(ehCo && cont)
                    {
                        variavelCmp[cont] = '\0';

                        j = getPos(lis, variavelCmp);
                        sis->matrizCoeficientes[i][j] = strtof(coeficiente, NULL);
                        cont = 0;
                        contN = 0;
                        variavelCmp = (char*)malloc(sizeof(char)*100);
                    }
                }
            }
        }

        free(equacao);
        free(coeficiente);
        free(resultado);
        free(variavelCmp);
}

void instrucoes () {
    printf("============================== INSTRUCOES ================================\n");
    printf("Para solucionar um sistem de equacoes lineares usando o programa, voce precisa \nusar um arquivo de texto. \n");
    printf("Escreva o sistema que deseje solucionar, colocando na primeira linha do \narquivo o numero de icognitas/equacoes nele.\n");
    printf("Atencao! O sistema nao podera ser solucionado caso seja SPI ou SI. \n");
    printf("Ao digitar o nome do arquivo, escreva seu endereco completo e coloque \".txt\" no final\n");
}

void solucionador () {
    Sistema sis;
    sis.lisEqua = NULL;
    sis.lisIcog = NULL;
    sis.matrizCoeficientes = NULL;
    sis.qtdIcog = 0;

    char* nome = (char*)malloc(sizeof(char)*100);
    printf("Digite o nome do arquivo: ");
    scanf("%s", nome);

    nome[strlen(nome)] = '\0';


    extrairCoeficientes(&sis, nome);
    printf("Sistema: ");
    printarSistema(&sis);
    printarResultado(&sis);

    freeSistema(&sis);
}

void creditos () {
    printf("\n------------- CREDITOS --------------------\n");
    printf("Autores: Fabio Faundes e Nicholas Patapoff.\n");
    printf("Apoio: Nenhum.\n");
    printf("Agradecimentos: Professor André Carvalho - Cotuca.");
    printf("\n\n\t Viva PD17, PD TERRO\n");
}

int main()
{
    int acabou = 0;
    int escolha =  -1;

    printf("\n========= Bem vindo ao solucionador de Sistemas Lineares!! ============\n\n");
    do{
        printf("O que voce deseja fazer?\n");
        printf("1 - Instrucoes;\n");
        printf("2 - Solucionar um sistema;\n");
        printf("3 - Creditos;\n");
        scanf("%d", &escolha);

        switch (escolha){
            case 1: system("cls"); instrucoes(); break;
            case 2: system("cls"); solucionador(); break;
            case 3: system("cls"); creditos(); break;
            default: break;
        }

        printf("\nSair do programa?(0/1)\n");
        scanf("%d", &acabou);
        system("cls");
    }while(!acabou);

    return  0;
}
