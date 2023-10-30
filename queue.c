#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

// Cria nova fila e retorna um ponteiro para ela

queue* create_queue(char* queue_name)
{
  queue* q = (queue*) malloc(sizeof(queue));
  strcpy(q->name, queue_name);
  q->size = 0;
  q->first = NULL;
  q->last = NULL;
  return q;
}

// Empilha mensagem msg na fila q

void enqueue_queue(queue* q, char* msg)
{
  node* new_node = (node*) malloc(sizeof(node));
  new_node->next = NULL;
  new_node->past = q->last;
  strcpy(new_node->message, msg);

  if(q->size == 0)
    q->first = new_node;
  
  else q->last->next = new_node;

  q->last = new_node;
  q->size += 1;
}

// Desempilha primeira mensagem da fila q e salva em buf

void dequeue_queue(queue* q, char* buf)
{
  if(q->size == 0) return; // Se não tiver elemetos na fila, só retorna. Para verificar se de fato um elemento foi desempilhado, deve-se usar a função size_queue() antes e depois dessa função.

  node* new_first = q->first->next;

  strcpy(buf, q->first->message);
  free(q->first);
  if(new_first != NULL) new_first->past = NULL;

  q->first = new_first;
  q->size--;
}

// Guarda em dst o primeiro elemento da fila. Quem a chama deve se certificar que a fila não está vazia primeiramente.

void first_queue (queue* q, char* dst)
{
  memcpy(dst, q->first->message, strlen(q->first->message));
}

// Retorna tamanho da fila

int size_queue (queue* q) { return q->size; }

// Deleta fila q

void remove_queue(queue* q)
{
  node* x = q->first;
  while(x != NULL)
  {
    node* tmp = x;
    x = x->next;
    free(tmp);
  }
  free(q);
}

// Imprime na saída padrão uma representação da fila

void print_queue(queue* q)
{
  printf("***************\n");
  printf("%s (%d) :\n", q->name, q->size);
  node* x = q->first;
  while(x != NULL)
  {
    puts(x->message);
    x = x->next;
  }
  printf("***************\n");
}
