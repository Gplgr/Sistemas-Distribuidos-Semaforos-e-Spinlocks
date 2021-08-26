#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

//Definindo a variavel global de contador pro limite de execucoes e os semaforos
int impressaominha;
sem_t empty,full,mutex;

//Funcao que gera numero aleatorio entre 1 e 10000000
int gerador(){
    return (rand()%9999999)+1;
};

//Funcao que verifica se um numero e primo e printa
//testa todos os valores ate a raiz do valor original; se nenhuma bater, e primo
//existem divisores entre a raiz e a metade do valor, mas que so ocorrem caso algum inferior ocorra
void printaprimo(int valorrecebido){
    char *primo = malloc(256);
    //usando o strcpy pra facilitar mexer com strings
    strcpy(primo,"e primo");
    if (valorrecebido==1) strcpy(primo,"nao e primo");
    for (int j=2;(double)j <= sqrt(valorrecebido); j++){
        if (valorrecebido%j == 0){
            strcpy(primo,"Nao e primo");
        };
    };
    printf("Em relacao a %d: %s\n", valorrecebido,primo);
    free(primo);
};

//Funcao produtor
void* produtor(void* arg){
    int *querovaga = (int *) arg;
    int N = sizeof(querovaga);
    int escrevinhador;
    while(impressaominha<10000){
        //Gera o numero de forma independente
        escrevinhador = gerador();
        sem_wait(&empty);
        sem_wait(&mutex);
        int value;
        sem_getvalue(&empty,&value);
        //verifica onde tem espaco livre e salva no vetor
        for (int i = 0; i < N; ++i){
            if (querovaga[i] == 0){
                querovaga[i] = escrevinhador;
                escrevinhador = 0;
                break;
            };
        };
        if (impressaominha >= 9999){
            sem_post(&mutex);
            sem_post(&full);
            sem_post(&empty);
            break;
        };
        sem_post(&mutex);
        sem_post(&full);
    };
};

//Funcao consumidor
void* consumidor(void* arg){
    int *queronumero = (int *) arg;
    int leitao;
    int N = sizeof(queronumero);
    while(impressaominha<10000){
        sem_wait(&full);
        sem_wait(&mutex);
        for (int i = 0; i < N; ++i){
            if (queronumero[i] != 0){
                leitao = queronumero[i];
                queronumero[i] = 0;
                break;
            };
        };
        //avisa que tem mais um sendo lido e impresso
        impressaominha++;
        sem_post(&mutex);
        sem_post(&empty);
        printaprimo(leitao);
        leitao = 0;
    };
};


int main (int argc, char *argv[]) {
    //definindo os inputs
    srand(time(NULL));
    int N,Np,Nc;
    printf("Insira o valor de N: ");
    scanf("%d",&N);
    printf("Insira o valor de Np: ");
    scanf("%d",&Np);  
    printf("Insira o valor de Nc: ");
    scanf("%d",&Nc);
    //Inicializando o vetor geral
    int memoria[N];
    //Por algum motivo nao estava passando 0 pros valores do vetor nem o valor direto nem com memset
    //entao foi feita de forma iterativa
    for (int i=0;i<N;i++){
        memoria[i] = 0;
    };
    //Inicializando os semaforos
    sem_init(&empty, 0, N);
    sem_init(&full, 0, N);
    sem_init(&mutex, 0, 1);

    //Setando o valor inicial de empty em N
    for (int i = 0; i < N; ++i){
        sem_post(&empty);
    };

    clock_t begin = clock();
    //Criando as threads
    //Criando as produtoras
    pthread_t produtores[Np];
    for (int i=0;i<Np;i++){
        pthread_create(&produtores[i],NULL,produtor, (void*)memoria);
    };

    //Criando as consumidoras
        pthread_t consumidores[Nc];
    for (int i=0;i<Nc;i++){
        pthread_create(&consumidores[i],NULL,consumidor, (void*)memoria);
    };

    //Esperando a finalizacao das threads
    for (int i=0;i<Np;i++){
        pthread_join(produtores[i],NULL);
    };
    for (int i=0;i<Nc;i++){
        pthread_join(consumidores[i],NULL);
    };
    clock_t end = clock();    
    double tempo = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("O tempo total foi de %f\n", tempo);
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
};