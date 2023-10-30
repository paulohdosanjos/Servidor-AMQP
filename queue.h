// Implementação de fila usando lista ligada

#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE_NAME_SIZE 256
#define MAX_MSG_SIZE 1024

/*
 
    first                       last
      A   -->   B   -->   C  --> D
      B.next = C  B.past = A
*/

typedef struct node 
{
  struct node* next;
  struct node* past;
  char message[MAX_MSG_SIZE];
} node;

typedef struct queue 
{
  char name[MAX_QUEUE_NAME_SIZE];
  int size;
  node* first;
  node* last;
} queue;

// Cria nova fila e retorna um ponteiro para ela
queue* create_queue(char* queue_name);

// Empilha mensagem msg na fila q
void enqueue_queue(queue* q, char* msg);

// Desempilha primeira mensagem da fila q e escreve em buf
void dequeue_queue(queue* q, char* buf);

// Deleta fila q
void remove_queue(queue* q);

// Imprime fila
void print_queue(queue* q);

// Escreve em dst a primeira mensagem da fila
void first_queue(queue* q, char* dst);

// Retorna tamanho da fila
int size_queue (queue* q);

#endif

