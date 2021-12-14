#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  mkfifo(WKP, 0644);
  int from_client = open(WKP, O_RDONLY);
  printf("WKP created\n");

  char *secret_name = calloc(HANDSHAKE_BUFFER_SIZE,1);
  read(from_client,secret_name,HANDSHAKE_BUFFER_SIZE);
  printf("server connected, secret name: %s\n",secret_name);
  remove(WKP);

  *to_client = open(secret_name,O_WRONLY);
  printf("opened private server\n");
  write(*to_client,ACK,strlen(ACK));

  char *message = calloc(HANDSHAKE_BUFFER_SIZE,1);
  read(from_client,message,HANDSHAKE_BUFFER_SIZE);
  printf("reponse received: %s\n",message);

  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

  char *secret_name = calloc(HANDSHAKE_BUFFER_SIZE,1);
  sprintf(secret_name,"%d",getpid());
  mkfifo(secret_name,0644);
  printf("client created secret pipe\n");

  *to_server = open(WKP, O_WRONLY);
  write(*to_server, secret_name,strlen(secret_name));

  char *message = calloc(HANDSHAKE_BUFFER_SIZE,1);
  int from_server = open(secret_name,O_RDONLY);
  read(from_server,message,HANDSHAKE_BUFFER_SIZE);

  printf("client got: %s\n",message);
  remove(secret_name);

  write(*to_server, "success", strlen("success"));

  return from_server;
}
