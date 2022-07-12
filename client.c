#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_endian.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 5050

int main() {

  int lsock, con_status;
  lsock = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr("127.0.0.1");
  sin.sin_port = htons(PORT);

  con_status = connect(lsock, (struct sockaddr*) &sin, sizeof(sin));
  char data[100] = "Hello Client";
  char buffer[1024] = {0};

  write(lsock, data, sizeof(data));
  read(lsock, buffer, sizeof(buffer));

  printf("Received!!!\n");
  printf("%s\n", buffer);
  close(lsock);

  return 0;
}