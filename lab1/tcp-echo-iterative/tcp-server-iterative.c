#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define SERVER_PORT 9999
#define MAX_LEN 1024

int main() {
	int sockfd;
	struct sockaddr_in self;
	
	/* Create a socket for server */
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0) {
		perror("Socket");
		exit(errno);
	}
	
	/* Address definition */
	bzero(&self,sizeof(self));	
	self.sin_family = AF_INET;
	self.sin_port = htons(SERVER_PORT); 
	self.sin_addr.s_addr = INADDR_ANY;
	
	/* Bind server socket to address */
	if(bind(sockfd, (struct sockaddr *)&self, sizeof(self))) {
		perror("socket --bind");
		exit(errno);
	}
	
	/* Add listen feature to server socket*/ 
	if(listen(sockfd, 20)!=0) {
		perror("socket --listen");
		exit(errno);
	}
	
	/* Create a socket for client */
	int clientfd;
	struct sockaddr_in client_addr;
	int client_addr_len = sizeof(client_addr); 
	
	while(1) {
		/* Wait for incoming connections */	
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

		/* Receive data from client */
		char input_string[MAX_LEN], response[MAX_LEN];
		recv(clientfd, input_string, sizeof(input_string), 0);
		strcpy(response,input_string);
		
		/* Send Response To Client */
		send(clientfd, response, sizeof(response), 0);
		
		/* Close socket file descriptors */
		close(clientfd);
	}

	close(sockfd);
	

	return 0;
}
