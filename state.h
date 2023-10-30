#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdlib.h>
#include <stdio.h>
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
#include "queue.h"

#define MAX_TRANSITIONS 3 // Número máximo de transições distintas possíveis em um estado
#define QUEUE_NAME_LENGTH_OFFSET 13 // Posição no frame de Queue.Declare / Basic.Consume onde começa o tamanho do nome da fila
#define ROUTING_KEY_SIZE_POSITION 14 // Posição no frame de Basic.Publish onde começa a routing_key do comando

extern int (*actions[NUM_STATES])(client_thread*, server_data*);
extern state transitions[NUM_STATES][MAX_TRANSITIONS];
extern char* state_name[NUM_STATES];

// Para cada estado STATE, há uma função do_STATE() definindo o comportamento do servidor nesse estado

int do_WAIT_HEADER (client_thread* data, server_data*);

int do_RCVD_HEADER (client_thread* data, server_data*);

int do_WAIT_START_OK (client_thread* data, server_data*);

int do_RCVD_START_OK (client_thread* data, server_data*);

int do_WAIT_TUNE_OK (client_thread* data, server_data*);

int do_WAIT_CONNECTION_OPEN (client_thread* data, server_data*);

int do_RCVD_CONNECTION_OPEN (client_thread* data, server_data*);

int do_WAIT_CHANNEL_OPEN (client_thread* data, server_data*);

int do_RCVD_CHANNEL_OPEN (client_thread* data, server_data*);

int do_WAIT_COMMAND (client_thread* data, server_data*);

int do_RCVD_QUEUE_DECLARE (client_thread* data, server_data*);

int do_RCVD_BASIC_PUBLISH (client_thread* data, server_data*);

int do_RCVD_BASIC_CONSUME (client_thread* data, server_data*);

int do_SUBSCRIBED (client_thread* data, server_data*);

int do_CLIENT_TURN (client_thread* data, server_data*);

int do_WAIT_BASIC_ACK (client_thread* data, server_data*);

int do_WAIT_CHANNEL_CLOSE (client_thread* data, server_data*);

int do_RCVD_CHANNEL_CLOSE (client_thread* data, server_data*);

int do_WAIT_CONNECTION_CLOSE (client_thread* data, server_data*);

int do_RCVD_CONNECTION_CLOSE (client_thread* data, server_data*);

int do_FINAL(client_thread* data, server_data*);

#endif
