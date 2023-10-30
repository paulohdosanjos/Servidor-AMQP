#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "server_config.h"
#include "state.h"
#include "utils.h"
#include "hardcoded.h"

server_data _server_data; // Informações globais do servidor compartilhadas por todas as threads 
pthread_mutex_t _server_data_mutex; // Mutex para controlar acesso às informações globais do servidor. Toda thread que acessa a estrutura _server_data obtém o esse mutex antes.

// Para cada novo cliente conectado é criada uma thread para cuidar da conexão. Cada thread executa essa função.

void *handle_client(void* __client_thread) 
{
  client_thread* _client_thread = (client_thread*) __client_thread;
  int n;
  printf("entrei na main da thread %lu\n", _client_thread->thread_id);

  _client_thread->current_state = INICIAL_STATE;
  _client_thread->server_data_mutex = &_server_data_mutex;

  // Simula a maquina de estados

  while(_client_thread->current_state != FINAL)
  {
    n = (*actions[_client_thread->current_state])(_client_thread, &_server_data);
    //printf("CODE : %d\n", n);
    _client_thread->current_state = transitions[_client_thread->current_state][n];
    //printf("ESTADO : %s\n", state_name[_client_thread->current_state]);
  }
  
  // Trecho para depuração
  // ****************************************************
  pthread_mutex_lock(&_server_data_mutex);
  printf("Filas no servidor:\n");
  printf("num filas = %d\n", _server_data.queue_list_size);
  for(int i = 0; i < _server_data.queue_list_size; i++)
    print_queue(_server_data.queue_list[i]);
  pthread_mutex_unlock(&_server_data_mutex);
  // ****************************************************

  close(_client_thread->connfd);
  printf("[Uma conexão fechada]\n");
  return NULL;
}

int main (int argc, char **argv) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
   
    if (argc != 2) {
        fprintf(stderr,"Uso: %s <Porta>\n",argv[0]);
        fprintf(stderr,"Vai rodar um servidor de echo na porta <Porta> TCP\n");
        exit(1);
    }

   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket :(\n");
        exit(2);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(atoi(argv[1]));

    int option = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); // Para não precisar esperar o TIME_WAIT

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind :(\n");
        exit(3);
    }

   if (listen(listenfd, LISTENQ) == -1) {
        perror("listen :(\n");
        exit(4);
    }

    _server_data = *((server_data *) malloc(sizeof(server_data)));
    pthread_mutex_init(&_server_data_mutex, NULL);
    printf("Estrutura de informações globais do servidor alocada com sucesso\n");

    printf("[Servidor no ar. Aguardando conexões na porta %s]\n",argv[1]);
    printf("[Para finalizar, pressione CTRL+c ou rode um kill ou killall]\n");

    for ( ; ; ) {
      
      if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
          perror("accept :(\n");
          exit(5);
      }

      printf("[Uma conexão aberta]\n");

      // Cria um thread para gerir a conexão com o novo cliente conectado

      client_thread* _client_thread = (client_thread*) malloc(sizeof(client_thread));
      _client_thread->connfd = connfd;
      pthread_create(&_client_thread->thread_id, NULL, handle_client,_client_thread);
  }

  exit(0);
}
