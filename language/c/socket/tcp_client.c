#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE 1000

int main(int argc, char** argv) {
  int client_sockfd;
  int len;
  struct sockaddr_in  remote_addr;  // server side network address struct 
  char buf[BUFFSIZE];               // buffer zone 
  memset(&remote_addr, 0, sizeof(remote_addr));   // data initialization. 
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // server side ip addr 
  remote_addr.sin_port = htons(8000);     // server side port 

  // create client socket 
  if ((client_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "create client socket error.\n");
    return 1;
  }

  // connect socket to server network addr. 
  if (connect(client_sockfd, (struct sockaddr*)&remote_addr, sizeof(struct sockaddr)) < 0) {
    fprintf(stderr, "connect socket to server network addr error.\n");
    return 1;
  }
  printf("connected to server\n");
  len = recv(client_sockfd, buf, BUFFSIZE, 0);  // recv server side message 
  buf[len] = '\0';
  printf("print server msg: %s\n", buf);

  // send info and recv it at loop mode. 
  while(1) {
    printf("Enter string to send:");
    scanf("%s", buf);
    if (!strcmp(buf, "q")) break;
    len = send(client_sockfd, buf, strlen(buf), 0);
    len = recv(client_sockfd, buf, BUFFSIZE, 0);
    buf[len] = '\0';
    printf("receiving: %s\n", buf);
  }
  close(client_sockfd);   // close socket
  return 0;
}
