#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;

  while (1){

    from_client = server_handshake( &to_client );
    printf("awaiting client\n");

    while (from_client){
      printf("client connected\n");
      char line[BUFFER_SIZE];
      read(from_client,line,BUFFER_SIZE);

      int i = 0;
      for (i = 0; i < strlen(line); i++) {
  				if (line[i] >= 97 && line[i] <= 122) line[i] -= 32;
  		}

      write(to_client,line,BUFFER_SIZE);
      from_client = 0;
    }
  }


}
