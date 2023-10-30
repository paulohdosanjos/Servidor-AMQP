// Esse arquivo define algumas funções de propósitos gerais utilizadas pelo servidor. Define como o servidor lê frames, monta frames, etc.

int read_frame (int connfd, unsigned char* buf);

int read_protocol_header (int connfd, unsigned char* buf);

unsigned char* to_longstr(char* value);

unsigned char* to_shortstr(char* value);

int write_short_int(unsigned char* frame, int i, unsigned short int n);

int write_long_int(unsigned char* frame, int i, unsigned long int n);

int write_long_long_int(unsigned char* frame, int i, unsigned long long int n);

int queue_declare_ok(unsigned char* frame, char* _queue_name);

int basic_consume_ok(unsigned char* frame, char* _consumer_tag);

int basic_deliver(unsigned char* frame, unsigned char* msg, int msg_length, char* _consumer_tag, char* _queue_name);

unsigned long int mount_general_frame (unsigned char* dst, unsigned char type, unsigned short int channel, unsigned char* payload, unsigned long int payload_size);

unsigned long int mount_method_frame_payload (unsigned char* dst, unsigned short int class_id, unsigned short int method_id, unsigned char* arguments, unsigned long int arguments_size);
