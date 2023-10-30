#include "state.h"
#include "utils.h"
#include "hardcoded.h"

// Array que mapeia estados para ações, ou seja, o comportamento do servidor em cada estado

int (*actions[NUM_STATES])(client_thread*, server_data*) = {
  do_WAIT_HEADER,
  do_RCVD_HEADER, 
  do_WAIT_START_OK,
  do_RCVD_START_OK,
  do_WAIT_TUNE_OK, 
  do_WAIT_CONNECTION_OPEN,
  do_RCVD_CONNECTION_OPEN, 
  do_WAIT_CHANNEL_OPEN, 
  do_RCVD_CHANNEL_OPEN, 
  do_WAIT_COMMAND,
  do_RCVD_QUEUE_DECLARE, 
  do_RCVD_BASIC_PUBLISH,
  do_RCVD_BASIC_CONSUME,
  do_SUBSCRIBED,
  do_CLIENT_TURN,
  do_WAIT_BASIC_ACK,
  do_WAIT_CHANNEL_CLOSE, 
  do_RCVD_CHANNEL_CLOSE, 
  do_WAIT_CONNECTION_CLOSE,
  do_RCVD_CONNECTION_CLOSE,
  do_FINAL
};

// Array de transições. Cada função actions[i] retorna um código que sinaliza para qual estado o servidor deve ir

state transitions[NUM_STATES][MAX_TRANSITIONS] = {
  {RCVD_HEADER, WAIT_HEADER}, // WAIT_HEADER,
  {WAIT_START_OK}, // RCVD_HEADER, 
  {RCVD_START_OK}, // WAIT_START_OK,
  {WAIT_TUNE_OK}, // RCVD_START_OK,
  {WAIT_CONNECTION_OPEN, RCVD_CONNECTION_OPEN}, // WAIT_TUNE_OK, 
  {RCVD_CONNECTION_OPEN}, // WAIT_CONNECTION_OPEN,
  {WAIT_CHANNEL_OPEN}, // RCVD_CONNECTION_OPEN, 
  {RCVD_CHANNEL_OPEN}, // WAIT_CHANNEL_OPEN, 
  {WAIT_COMMAND}, // RCVD_CHANNEL_OPEN, 
  {RCVD_QUEUE_DECLARE, RCVD_BASIC_PUBLISH, RCVD_BASIC_CONSUME}, // WAIT_COMMAND, 
  {WAIT_CHANNEL_CLOSE}, // RCVD_QUEUE_DECLARE, 
  {WAIT_CHANNEL_CLOSE, RCVD_CONNECTION_CLOSE}, // RCVD_BASIC_PUBLISH, 
  {SUBSCRIBED}, // RCVD_BASIC_CONSUME, 
  {CLIENT_TURN}, // SUBSCRIBED, 
  {WAIT_BASIC_ACK, SUBSCRIBED}, // CLIENT_TURN, 
  {SUBSCRIBED}, // WAIT_BASIC_ACK, 
  {RCVD_CHANNEL_CLOSE}, // WAIT_CHANNEL_CLOSE, 
  {WAIT_CONNECTION_CLOSE}, // RCVD_CHANNEL_CLOSE, 
  {RCVD_CONNECTION_CLOSE}, // WAIT_CONNECTION_CLOSE,
  {FINAL}, // RCVD_CONNECTION_CLOSE
  {FINAL}  // FINAL
};

// Mapeamento estado (um inteiro) para nome do estado. Usado para fins de depuração
char* state_name[NUM_STATES] = {
  "WAIT_HEADER",
  "RCVD_HEADER", 
  "WAIT_START_OK",
  "RCVD_START_OK",
  "WAIT_TUNE_OK", 
  "WAIT_CONNECTION_OPEN",
  "RCVD_CONNECTION_OPEN", 
  "WAIT_CHANNEL_OPEN", 
  "RCVD_CHANNEL_OPEN", 
  "WAIT_COMMAND", 
  "RCVD_QUEUE_DECLARE", 
  "RCVD_BASIC_PUBLISH", 
  "RCVD_BASIC_CONSUME", 
  "SUBSCRIBED", 
  "CLIENT_TURN", 
  "WAIT_BASIC_ACK", 
  "WAIT_CHANNEL_CLOSE", 
  "RCVD_CHANNEL_CLOSE", 
  "WAIT_CONNECTION_CLOSE",
  "RCVD_CONNECTION_CLOSE",
  "FINAL"
};

// Servidor bloqueia até receber o protocol header do cliente. Não valida o protocolo.

int do_WAIT_HEADER (client_thread* data, server_data* _server_data)
{
  read_protocol_header(data->connfd, data->buf);

  printf("Protocol Header recebido\n");

  return 0;
}

// Servidor manda um Connection.Start para o cliente. HARDCODED.

int do_RCVD_HEADER (client_thread* data, server_data* _server_data)
{
    write(data->connfd, CONNECTION_START_FRAME, CONNECTION_START_FRAME_SIZE);

    printf("Connection.Start enviado\n");

    return 0;
}

// Servidor bloqueia até receber um Connection.Start-Ok do cliente. Não valida.

int do_WAIT_START_OK (client_thread* data, server_data* _server_data)
{
  read_frame(data->connfd, data->buf);

  printf("Connection.Start-Ok recebido\n");
  return 0;
}

// Servidor manda um Connection.Tune para o cliente. HARDCODED.

int do_RCVD_START_OK (client_thread* data, server_data* _server_data)
{
  write(data->connfd, CONNECTION_TUNE_FRAME, CONNECTION_TUNE_FRAME_SIZE);

  printf("Connection.Tune enviado\n");

  return 0;
}

// Servidor bloqueia até receber um Connection.Tune-Ok do cliente. Não valida. Como logo em seguida ele deverá receber um Connection.Open, é possível que o socket contenha os dois frames correspondentes a esses comandos. Retorna 1 nesse caso e 0 caso contrário. 

int do_WAIT_TUNE_OK (client_thread* data, server_data* _server_data)
{

  int bytes_read = read_frame(data->connfd, data->buf);

  printf("Connection.Tune-Ok recebido\n");

  int count = 0; // Quantos frames amqp foram enviados pelo cliente ?
  for(int i = 0; i < bytes_read; i++) if(data->buf[i] == FRAME_END) count++; 

  if(count == 2) 
    printf("Connection.Open recebido\n");

  return count - 1;
}

// Servidor bloqueia até receber um Connection.Open do cliente. Não valida.

int do_WAIT_CONNECTION_OPEN (client_thread* data, server_data* _server_data)
{
  int bytes_read = read_frame(data->connfd, data->buf);

  return 0;
}

// Servidor envia Connection.Open-Ok para o cliente. HARDCODED.

int do_RCVD_CONNECTION_OPEN (client_thread* data, server_data* _server_data)
{
  write(data->connfd, CONNECTION_OPEN_OK_FRAME, CONNECTION_OPEN_OK_FRAME_SIZE);

  printf("Connection.Open-Ok enviado\n");

  return 0;
}

// Servidor bloqueia até receber um Channel.Open do cliente. Não valida.

int do_WAIT_CHANNEL_OPEN (client_thread* data, server_data* _server_data)
{
  int bytes_read = read_frame(data->connfd, data->buf);

  printf("Channel.Open recebido\n");

  return 0;
}

// Servidor envia Channel.Open-Ok para o cliente. HARDCODED.

int do_RCVD_CHANNEL_OPEN (client_thread* data, server_data* _server_data)
{
  write(data->connfd, CHANNEL_OPEN_OK_FRAME, CHANNEL_OPEN_OK_FRAME_SIZE);

  printf("Channel.Open-Ok enviado\n");

  return 0;
}

// Nesse estado, o servidor bloqueia até receber um declare_queue, publish ou consume. Não trata outros comandos. 

int do_WAIT_COMMAND (client_thread* data, server_data* _server_data) 
{
  int bytes_read = read_frame(data->connfd, data->buf);
  data->bytes_read = bytes_read;

  // Extrai método do frame
  unsigned short int method_id = data->buf[METHOD_ID_POSITION]; 

  switch (method_id) 
  {
    case DECLARE_ID:
      printf("Queue.Declare recebido\n");
      return 0;
      break;

    case PUBLISH_ID:
      printf("Basic.Publish recebido\n");
      return 1;
      break;

    case CONSUME_ID:
      printf("Basic.Consume recebido\n");
      return 2;
      break;

    default:
      return 3; // O servidor deve morrer aqui. Não há tratamento de erros nesse EP
      break;
  }
}

// Servidor recebe solicitação de criação de fila e envia Queue.Declare-Ok para o cliente. Se o servidor está nesse estado, os dados da fila estão em data->buf devido ao comando Queue.Declare enviado pelo cliente. Caso alguma fila com esse nome já exista, o servidor continua.

int do_RCVD_QUEUE_DECLARE (client_thread* data, server_data* _server_data)
{
  // Extrai nome da fila 
  int queue_name_size = data->buf[QUEUE_NAME_LENGTH_OFFSET];
  char queue_name[MAX_QUEUE_NAME_SIZE];
  memcpy(queue_name, data->buf + QUEUE_NAME_LENGTH_OFFSET + 1, queue_name_size);
  queue_name[queue_name_size] = 0;

  // Verifica se a fila já existe no servidor. Como teremos no máximo 8 filas a qualquer instante, é aceitável procurar pela fila pelo seu nome na lista de filas do servidor

  pthread_mutex_lock(data->server_data_mutex);
  int is_present = FALSE;
  for(int i = 0; i < _server_data->queue_list_size; i++) 
    if(strcmp(_server_data->queue_list[i]->name, queue_name) == 0) is_present = TRUE;
  
  if(!is_present)
  {
    queue* q = create_queue(queue_name); // Aloca nova fila vazia com nome queue_name
    _server_data->queue_list_size++;
    _server_data->queue_list[_server_data->queue_list_size-1] = q;

    printf("nova fila criada, número de filas = %d\n", _server_data->queue_list_size);
  }
  pthread_mutex_unlock(data->server_data_mutex);
  
  // Envia Queue.Declare-Ok para o cliente

  int n = queue_declare_ok(data->buf, queue_name);
  write(data->connfd, data->buf, n);

  printf("Queue.Declare-Ok enviado\n");

  return 0;
}

// Servidor recebeu um Basic.Publish. Escreve mensagem recebida na fila corresponed. Pode ter recebido um Channel.Close também. Não trata erros do tipo, fila inexistente, etc. Outro ponto importante é que aqui assumimos que a mensagem será enviada pelo servidor em somente um Content Body. Isso é verdade para o limite do tamanho das mensagens estabelecido. Mas um programa mais robusto deveria verificar isso.

int do_RCVD_BASIC_PUBLISH (client_thread* data, server_data* _server_data)
{
  int n;
  char* exchange_name = "foo"; // Não usamos exchanges. Se o cliente não especificar nenhum exchange, esse campo será 0x00 no frame. Estamos pressupondo isso aqui.

  // Extrai nome da fila 
  char queue_name[MAX_QUEUE_NAME_SIZE];
  int queue_name_size = data->buf[ROUTING_KEY_SIZE_POSITION];
  memcpy(queue_name, data->buf + ROUTING_KEY_SIZE_POSITION + 1, queue_name_size);
  queue_name[queue_name_size] = 0;

  printf("nome da fila extraído : ");
  puts(queue_name);

  // Extrai mensagem

  int count = 0;

  // Pula o Method frame e o Content header, o restante é o Content body
  int i;
  for(i = 0; i < data->bytes_read && count < 2; i++) 
    if(data->buf[i] == FRAME_END) count++;   
  
  i+=3; // i agora aponta para o começo do campo length do Content body

  // Extrai tamanho do Content body

  int content_body_length = 0;
  content_body_length += data->buf[i++] << 6;
  content_body_length += data->buf[i++] << 4;
  content_body_length += data->buf[i++] << 2;
  content_body_length += data->buf[i++];

  // Recupera mensagem
  char msg[MAX_MSG_SIZE];
  memcpy(msg, data->buf + i, content_body_length);
  msg[content_body_length] = 0;

  // Procura pela fila na lista de filas do servidor
  pthread_mutex_lock(data->server_data_mutex);
  for(i = 0; i < _server_data->queue_list_size; i++)
    if(strcmp(_server_data->queue_list[i]->name, queue_name) == 0) break;

  enqueue_queue(_server_data->queue_list[i], msg); // Empilha mensagem

  printf("Mensagem publicada na fila %s\n", _server_data->queue_list[i]->name);
  pthread_mutex_unlock(data->server_data_mutex);

  // Agora, vamos verificar se o Channel.Close foi lido junto no socket. Supomos que o Basic.Publish veio em 3 frames, confome descrito em cima da assinatura dessa função
  count = 0;
  for(i = 0; i < data->bytes_read; i++)
    if(data->buf[i] == FRAME_END) count++;   

  if(count == 4) // Leu Channel.Close junto
   return 1;
  else return 0;
}

// Servidor inscreve cliente na fila requerida e envia Basic.Consume-Ok. Não verifica se a fila de fato existe. 
int do_RCVD_BASIC_CONSUME (client_thread* data, server_data* _server_data)
{
  // Extrai nome da fila a partir do pacote do cliente Basic.Consume
  char queue_name[MAX_QUEUE_NAME_SIZE];
  int queue_name_size = data->buf[QUEUE_NAME_LENGTH_OFFSET];
  memcpy(queue_name, data->buf + QUEUE_NAME_LENGTH_OFFSET + 1, queue_name_size);
  queue_name[queue_name_size] = 0;

  printf("Nome da fila extraído : ");
  puts(queue_name);

  // Inscreve cliente nessa fila. Primeiramente, acha índice dessa fila na lista de filas do servidor. Não verifica se o cliente já está inscrito na fila. Se um cliente for conectado mais de uma vez na mesma fila, o efeito é que ele receberá mais um "quanta" no round robin, e ele poderá receber duas mensagens na sua vez de consumir. Entretanto, esse comportamento não é o comportamento padrão de um servidor AMQP, portanto, isso não deve ser feito pelo cliente utilizando esse servidor simplificado.

  pthread_mutex_lock(data->server_data_mutex);
  int i;
  for(i = 0; i < _server_data->queue_list_size; i++)
    if(strcmp(_server_data->queue_list[i]->name, queue_name) == 0) break;

  data->client_queue = i; // Guarda em qual fila cliente está inscrito

  if(_server_data->client_queue[i] == NULL) // Ainda não foi criada a lista de clientes dessa fila. Primeiro cliente conectado
  {
    _server_data->client_queue[i] = create_queue("foo");
    printf("não tinha ninguém inscrito nessa fila ainda, fila de clientes alocada\n");
  }

  char consumer_tag[MAX_CONSUMER_TAG_SIZE];
  sprintf(consumer_tag, "%lu", data->thread_id); // A consumer tag do cliente vai ser o id da thread que gerencia a conexão

  printf("a consumer_tag desse cliente é %s\n", consumer_tag);

  enqueue_queue(_server_data->client_queue[i], consumer_tag);

  printf("cliente inscrito na fila %d de nome %s\n", i, _server_data->queue_list[i]->name);
  pthread_mutex_unlock(data->server_data_mutex);

  // Envia o Basic.Consume-Ok
  int n;
   
  n = basic_consume_ok(data->buf, consumer_tag);
  write(data->connfd, data->buf, n);

  printf("Basic.Consume-Ok enviado\n");

  return 0; 
}

// Servidor verifica se é a vez do cliente de consumir uma mensagem na fila em que ele está inscrito. Fica em loop verificando essa condição até que ela seja verdadeira e o servidor transite para o estado CLIENT_TURN, onde o cliente efetivamente receberá a mensagem

int do_SUBSCRIBED(client_thread *data, server_data* _server_data)
{
  while(1)
  {
    pthread_mutex_lock(data->server_data_mutex);
    queue* q = _server_data->queue_list[data->client_queue];
    //printf("fila que esse cliente está inscrito : %s\n", q->name);

    char next_client_to_consume[MAX_MSG_SIZE]; // Consumer tag do próximo cliente a consumir na fila q
    memset(next_client_to_consume, 0, MAX_MSG_SIZE);
    queue* q_client = _server_data->client_queue[data->client_queue];
    first_queue(q_client, next_client_to_consume);
    pthread_mutex_unlock(data->server_data_mutex);

    //printf("o próximo cliente a consumir nessa fila é : %s\n", buf);
    char client_consumer_tag[MAX_MSG_SIZE];
    memset(client_consumer_tag, 0, MAX_MSG_SIZE);
    sprintf(client_consumer_tag, "%lu", data->thread_id);
    if(strcmp(next_client_to_consume, client_consumer_tag) == 0) // Vez do cliente :)
    {
      //printf("é sua vez :)\n");
      break;
    }
  }

  return 0;
}

// Vez do cliente de consumir uma mensagem da sua fila. Servidor manda um Basic.Deliver contendo a mensagem e faz o round robin.

int do_CLIENT_TURN(client_thread *data, server_data* _server_data)
{
  char client_consumer_tag[MAX_CONSUMER_TAG_SIZE];
  sprintf(client_consumer_tag, "%lu", data->thread_id); // A consumer tag do cliente é o id da thread que gerencia a conexão

  pthread_mutex_lock(data->server_data_mutex);
  queue* q = _server_data->queue_list[data->client_queue];   // Fila que o cliente está inscrito
 
  // Verifica se há novas mensagens na fila. Caso não tenha, volta para o estado SUBSCRIBED

  if(size_queue(q) == 0) 
  {
    pthread_mutex_unlock(data->server_data_mutex);
    return 1;  
  }
  // Desempilha mensagem da fila
  
  char msg[MAX_MSG_SIZE];
  dequeue_queue(_server_data->queue_list[data->client_queue], msg);

  // Envia o Basic.Deliver com a mensagem
  int n = basic_deliver(data->buf, (unsigned char*) msg, strlen(msg), client_consumer_tag, q->name);
  write(data->connfd, data->buf, n);

  printf("Basic.Deliver enviado\n");

  // Faz o round robin da fila. Basicamente, desempilha e empilha o cliente na fila de clientes da lista.
  queue* q_client = _server_data->client_queue[data->client_queue]; // Fila de clientes da fila que o cliente está inscrito
  char buf[MAX_MSG_SIZE];
  dequeue_queue(q_client, buf);
  enqueue_queue(q_client, client_consumer_tag);
  pthread_mutex_unlock(data->server_data_mutex);

  return 0;
}

// Servidor bloqueia até receber um Basic.Ack do cliente. Não valida.
int do_WAIT_BASIC_ACK (client_thread* data, server_data* _server_data)
{
  int bytes_read = read_frame(data->connfd, data->buf);

  printf("Basic.Ack recebido\n");

  return 0;
}

// Servidor bloqueia até receber um Channel.Close do cliente. Não valida.
int do_WAIT_CHANNEL_CLOSE (client_thread* data, server_data* _server_data)
{
  int bytes_read = read_frame(data->connfd, data->buf);

  printf("Channel.Close recebido\n");

  return 0;
}

// Servidor manda Channel.Close-Ok para o cliente. HARDCODED.
int do_RCVD_CHANNEL_CLOSE (client_thread* data, server_data* _server_data)
{
  write(data->connfd, CHANNEL_CLOSE_OK_FRAME, CHANNEL_CLOSE_OK_FRAME_SIZE);

  printf("Channel.Close-Ok enviado\n");

  return 0;
}

// Servidor bloqueia até receber um Connection.Close do cliente. Não valida.
int do_WAIT_CONNECTION_CLOSE (client_thread* data, server_data* _server_data)
{
  int bytes_read = read_frame(data->connfd, data->buf);
  
  printf("Connection.Close recebido\n");

  return 0;
}

// Servidor manda Connection.Close-Ok para o cliente. HARDCODED.
int do_RCVD_CONNECTION_CLOSE (client_thread* data, server_data* _server_data)
{
  write(data->connfd, CONNECTION_CLOSE_OK_FRAME, CONNECTION_CLOSE_OK_FRAME_SIZE);

  printf("Connection.Close-Ok enviado\n");

  return 0;
}

int do_FINAL (client_thread* data, server_data* _server_data)
{
  return 0;
}
