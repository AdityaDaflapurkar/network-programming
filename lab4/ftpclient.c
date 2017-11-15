#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#define MAX_LEN 1024

int main() {
	int clientfd;
	struct sockaddr_in serveraddr;
	
	if(socket(AF_INET, SOCK_STREAM, 0)<0) {
		perror("socket --");
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(21); 
	if(inet_aton("127.0.0.1", &serveraddr.sin_addr.s_addr)==0) {
		perror("address--");
		exit(errno);
	}

	if(connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))!=0) {
		perror("connect--");	
	}

	char *request;
	request = "Hello World\n";
	send(clientfd, request, sizeof(request), 0);

	return 0;	
}
