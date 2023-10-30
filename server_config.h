// Esse arquivo define as macros e estruturas utilizadas pelo servidor AMQP

#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#define MAX_FRAME_SIZE 100000 // Tamanho máximo em bytes dos frames servidor/cliente 
#define MAX_CONSUMER_TAG_SIZE 100 // Tamnho máximo em bytes da consumer_tag de um cliente
#define PROTOCOL_HEADER_SIZE 8
#define FRAME_END 0xce 
#define MAX_NUM_QUEUE 8 // Número máximo de filas que podem ser declaradas, de acordo com o requisito escrito no fórum

#define TRUE 1
#define FALSE 0

#define LISTENQ 1
#define INICIAL_STATE WAIT_HEADER // Estado inicial de qualquer conexão: espera pelo protocol header
#define NUM_STATES 21

#define METHOD_ID_POSITION 10
#define DECLARE_ID 10
#define PUBLISH_ID 40
#define CONSUME_ID 20

#define MAX_CLIENTS 100 // Número máximo de clientes conectados a qualquer instante

#include <pthread.h>

#include "queue.h"

// Estados da maquina de estados do servidor

typedef enum {
  WAIT_HEADER,
  RCVD_HEADER, 
  WAIT_START_OK,
  RCVD_START_OK,
  WAIT_TUNE_OK, 
  WAIT_CONNECTION_OPEN,
  RCVD_CONNECTION_OPEN, 
  WAIT_CHANNEL_OPEN, 
  RCVD_CHANNEL_OPEN, 
  WAIT_COMMAND,
  RCVD_QUEUE_DECLARE, 
  RCVD_BASIC_PUBLISH, 
  RCVD_BASIC_CONSUME,
  SUBSCRIBED,
  CLIENT_TURN,
  WAIT_BASIC_ACK,
  WAIT_CHANNEL_CLOSE, 
  RCVD_CHANNEL_CLOSE, 
  WAIT_CONNECTION_CLOSE,
  RCVD_CONNECTION_CLOSE,
  FINAL
} state;

// Estrutura que guarda informações específicas de cada conexão, que é gerida por uma thread
typedef struct client_thread {
  int connfd; // socket da conexão
  state current_state;
  pthread_t thread_id; // também é a consumer_tag desse cliente
  unsigned char buf[MAX_FRAME_SIZE]; // buffer para guardar dados lidos/escritos do socket
  int bytes_read;
  int client_queue; // índice da fila na qual o cliente está inscrito, se ele for um consumidor
  pthread_mutex_t* server_data_mutex; // Artifício para usar o mutex global dentro das funções de estado
} client_thread;

typedef struct server_data {
  queue* queue_list[MAX_NUM_QUEUE]; // lista de filas
  int queue_list_size;
  queue* client_queue[MAX_NUM_QUEUE]; // lista de filas de clientes conectados em cada fila. Esse array é alinhado com queue_list, ou seja, client_queue[i] guarda a fila de consumidores conectados na fila queue_list[i]
} server_data;

extern server_data _server_data; // Informações globais do servidor compartilhadas por todas as threads 
extern pthread_mutex_t _server_data_mutex; // Mutex para controlar acesso às informações globais do servidor. Toda thread que acessa a estrutura _server_data obtém o esse mutex antes.

#endif

