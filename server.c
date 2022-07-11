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
#define HTTP_HEADER "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n"

int bind_lsock(int lsock) {
  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(PORT);

  return bind(lsock, (struct sockaddr *)&sin, sizeof(sin));
}

void *pthread_ex(void *arg) {
  int i = 0;
  while (i < 10) {
    sleep(1);
    printf("thread.. %d\n", i);
    i++;
  }
  return 0;
}

int main() {

  int lsock, asock, valread;
  char buffer[1024] = {0}, rt_txt[500];

  struct sockaddr_in remote_sin;
  socklen_t remote_sin_len;

  printf("[INFO] The server will listen to port: %d.\n", PORT);

  lsock = socket(AF_INET, SOCK_STREAM, 0);

  if (bind_lsock(lsock) < 0) {
    perror("bind error");
    return -1;
  }

  listen(lsock, 5);

  printf("[INFO] waiting for connection... \n");

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, pthread_ex, NULL);

  while (1) {

    asock = accept(lsock, (struct sockaddr *)&remote_sin, &remote_sin_len);

    if (asock < 0) {
      perror("[ERR] failed to accept.\n");
    }

    // read
    valread = read(asock, buffer, sizeof(buffer));
    printf("=====Received!!=====\n");
    printf("%s\n", buffer);

    // write
    strcpy(rt_txt, HTTP_HEADER);
    strcat(rt_txt, "hello123123123");

    printf("%s", rt_txt);

    printf("\n Size: %lu \n", sizeof(rt_txt));
    write(asock, rt_txt, sizeof(rt_txt));

    close(asock);
  }
}