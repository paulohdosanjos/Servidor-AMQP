// Esse arquivo define alguns frames constantes enviados pelo servidor. As funções que mandam os frames correspondentes usam esses valores.

extern const unsigned char CONNECTION_START_FRAME[];
extern const unsigned long int CONNECTION_START_FRAME_SIZE;

extern const unsigned char CONNECTION_TUNE_FRAME[];
extern const unsigned long int CONNECTION_TUNE_FRAME_SIZE;

extern const unsigned char CONNECTION_OPEN_OK_FRAME[];
extern const unsigned long int CONNECTION_OPEN_OK_FRAME_SIZE;

extern const unsigned char CONNECTION_CLOSE_OK_FRAME[];
extern const unsigned long int CONNECTION_CLOSE_OK_FRAME_SIZE;

extern const unsigned char CHANNEL_OPEN_OK_FRAME[];
extern const unsigned long int CHANNEL_OPEN_OK_FRAME_SIZE;

extern const unsigned char CHANNEL_CLOSE_OK_FRAME[];
extern const unsigned long int CHANNEL_CLOSE_OK_FRAME_SIZE;
