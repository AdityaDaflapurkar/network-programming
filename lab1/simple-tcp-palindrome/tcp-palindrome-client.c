#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#define SERVER_ADDR "127.0.0.1"
#define MAX_LEN 1024

int main() {
	int sockfd;
	struct sockaddr_in server_addr;
	
	/* Create a socket for client*/
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0) {
		perror("Socket");
		exit(errno);
	}
	
	/* Define address for server */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9999);
	if(inet_aton(SERVER_ADDR, &server_addr.sin_addr.s_addr)==0) {
		perror("Address");
		exit(errno);
	}

	/* Connect client socket to server address */
	if(connect(sockfd, (struct sockaddr *)&server_addr,sizeof(server_addr))!=0) {
		perror("Connect");	
		exit(errno);
	}

	/* Send data and receive response */
	char query_string[MAX_LEN], response[MAX_LEN];
	scanf("%[^\n]s",query_string);
	send(sockfd, query_string, sizeof(query_string), 0);
	recv(sockfd, response, sizeof(response), 0);
	printf("%s\n",response);

	/* Close socket file descriptor */
	close(sockfd);
	
	return 0;
}
