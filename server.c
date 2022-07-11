#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
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

int bind_lsock(int lsock) {
  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(PORT);

  return bind(lsock, (struct sockaddr *)&sin, sizeof(sin));
}

int main() {

  int lsock, asock, valread;
  char buffer[1024] = {0}, rt_txt[500];
  char *rt_txt_p;

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
    strcpy(rt_txt_p, "HTTP/1.1 200 OK\r\n");
    strcat(rt_txt_p, "Content-Type: text/plain\r\n");
    strcat(rt_txt_p, "\r\n");
    strcat(rt_txt_p, "hello123123123");

    char test_text[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nhello123123123";
    printf("%d", *rt_txt_p);

    printf("\n Size: %d \n", sizeof(*rt_txt_p));
    write(asock, rt_txt_p, sizeof(*rt_txt_p));

    close(asock);
  }
}