/* Somador.c */
/* Giovanni Paes */
/* Codigo para o exercicio de um somador com entradas aleatorias e paralelizado com spinlocks */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h> 

//estrutura do lock
struct lock{
    bool held;
};

//variaveis globais de soma e lock
int soma = 0;
struct lock lock;

//tentei nao usar como global mas vi todos os erros possiveis tentando passar uma estrutura que continha
//as tres variaveis anteriores e esse vetor como parametro pras threads
char *vetor = NULL;

//test and set conforme apresentado na aula
bool test_and_set(bool *flag) {
    bool anterior = *flag;
    *flag = 1;
    return anterior;
}

//funcao para adquirir o lock
void acquire(struct lock lock) {
    while(test_and_set(&lock.held));
};

//funcao para liberar o lock
void release(struct lock lock) {
    lock.held = 0;
};

//Funcao para as threads executarem
void *funcaothreads(void *arg){
    int *parametros = (int *) arg;
    for (int i=parametros[2];i<parametros[0];i=i+parametros[1]){
        acquire(lock);
        soma = soma + vetor[i];
        release(lock); 
    };
};

//funcao principal
int main(int argc, char *argv[]){
    //setando as variáveis
    int N,K;

    //setando o valor inicial do lock
    lock.held = 0;

    //definindo os inputs
    printf("Insira o valor de N: ");
    scanf("%d",&N);
    printf("Insira o valor de K: ");
    scanf("%d",&K);


    //setando o vetor de threads
    pthread_t threads[K];

    //Criando e retornando o vetor com os valores aleatorios
    vetor = malloc(sizeof(int)*N);
    srand(time(NULL));
    int temp;
    for(int i=0;i<N;i++){
        vetor[i] = (rand() % 201) - 100;
    };

    //Criando as threads
    clock_t begin = clock();
    for (int i=0;i<K;i++){
        int parametros[3] = {N,K,i};
        int * p = malloc(3 * sizeof(int));
        memcpy(p, parametros, 3*sizeof(int));
        pthread_create(&threads[i],NULL,funcaothreads, p);
    };

    //Esperando a finalizacao das threads
    for (int i=0;i<K;i++){
        pthread_join(threads[i],NULL);
    };
    clock_t end = clock();
    double tempo = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("A soma final e %d\n",soma);
    printf("O tempo total foi de %f\n", tempo);
    return 0;
};
