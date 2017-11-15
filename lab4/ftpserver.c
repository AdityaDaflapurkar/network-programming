#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main() {
	int serverfd;
	char *buffer;
	size_t buf_siz = 1024;
	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket-- ");	
	}
	
	struct sockaddr_in serveraddr, clientaddr;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(21);
	socklen_t addrlen = sizeof(serveraddr);

	if(bind(serverfd, (struct sockaddr *)&serveraddr, addrlen) < 0) {
		perror("bind-- ");	
	}

	if(listen(serverfd, 20)<0) {
		perror("listen-- ");
	}

	if(accept(serverfd, (struct sockaddr *)&clientaddr, &addrlen)<0) {
		perror("accept-- ");
	}
	
	buffer = (char *)malloc(1024*sizeof(char));
	if(recv(serverfd, &buffer, buf_siz, 0) > 0) {
		printf("%s", buffer);
	}
	return 0;
}
