#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#define SERVER_PORT 9999
#define MSG_LEN 1024

int main() {
	int sockfd, num_chars;
	struct sockaddr_in client_address, server_address;
	socklen_t addrlen = sizeof (struct sockaddr_in);
	char message[MSG_LEN];

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror ("socket");
		return -1;	
	}
	
	bzero (&server_address, addrlen);
	bzero (&client_address, addrlen);

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sockfd, (struct sockaddr *)&server_address, addrlen) < 0) {
		perror ("bind");
		return -1;	
	}

  	num_chars = recvfrom(sockfd, &message, MSG_LEN, 0, (struct sockaddr *)&client_address, &addrlen);
	printf ("Message from client = %s\n", message);

	sendto(sockfd, &message, num_chars, 0, (struct sockaddr *) &client_address, addrlen);
	
	close(sockfd);

	return 0;
}
