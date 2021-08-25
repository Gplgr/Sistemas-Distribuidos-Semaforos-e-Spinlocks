# Semáforos e Spinlocks
Códigos em C para execuções e testes de desempenho em relação ao tempo com existência de spinlocks e com o uso de semáforos.

## Semáforos
Implementa um sistema produtor-consumidor, cujo comunicação é feita através de um vetor de tamanho N inserido pelo usuário, assim como os valores Np e Nc, 
que respectivamente se referem ao número de threads produtoras e o número de threads consumidoras. As threads produtoras criam um número aleatório entre 1
e 10000000 enquanto as threads consumidoras dizem se esse número é primo ou não. Esse código executa até que a 10000ª execução do lado consumidor seja feita.

## Spinlocks
Corresponde a um sistema multithreaded com a soma de N valores aleatórios entre -100 e 100, com K threads cuja concorrência é controlada por um lock com test-and-set.

### Instalação
Enquanto o arquivo makefile não for empurrado, no diretório do arquivo C alvo:
```sh
gcc PCSem.c -lpthread -lrt -o semaforo -lm
```
seguido de 
```sh
./semaforo
```
ou
```sh
gcc Somador.c -lpthread -o spinlocks
```
seguido de
```sh
./spinlocks
```
