#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE 1000

int main(int argc, char** argv) {
  int server_sockfd;      // socket for server side 
  int client_sockfd;      // socket for client side
  int len;
  struct sockaddr_in my_addr;     // struct of server network address
  struct sockaddr_in remote_addr; // struct of client network address 
  int sin_size;
  char buf[BUFFSIZE];  // buffer zone size
  memset(&my_addr, 0, sizeof(my_addr));    // data initialization.
  my_addr.sin_family = AF_INET;           // set IP communication
  my_addr.sin_addr.s_addr = INADDR_ANY;   // server IP addr. allow connect to any addr of local
  my_addr.sin_port = htons(8000);         // server side port 

  // create socket in server side. 
  if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "create server socket is error.\n");
    return 1;
  }

  // bind socket to server network addr 
  if (bind(server_sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) < 0) {
    fprintf(stderr, "bind server socket is error.\n");
    return 1;
  }

  // listen connect request, size of listen queue is equals to 5 
  listen(server_sockfd, 5);

  sin_size = sizeof(struct sockaddr_in);

  // wait request connection receiving of client side 
  if ((client_sockfd = accept(server_sockfd, (struct sockaddr*)&remote_addr, &sin_size)) < 0) {
    fprintf(stderr, "accept client connect is error.\n");
    return 1;
  }
  fprintf(stdout, "accept client: %s\n", inet_ntoa(remote_addr.sin_addr));
  len = send(client_sockfd, "welcome to my server\n", 21, 0);  // send message to client 

  // accept client data and send it to client. 
  while((len = recv(client_sockfd, buf, BUFFSIZE, 0)) > 0) {
    buf[len] = "\0";
    printf("while recv message: %s\n", buf);
    if (send(client_sockfd, buf, len, 0) < 0) {
      fprintf(stderr, "server send error.\n");
      return 1;
    }
  }
  close(client_sockfd);
  close(server_sockfd);
  return 0;
}
