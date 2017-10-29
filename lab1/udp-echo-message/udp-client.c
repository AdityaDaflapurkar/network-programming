#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 9999
#define MSG_LEN 1024

int main() {
	int sockfd, num_chars;
	struct sockaddr_in client_address, server_address;
	int addrlen = sizeof (struct sockaddr_in);
	char message[MSG_LEN];

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror ("socket");
		return -1;	
	}
	
	bzero (&server_address, addrlen);
	bzero (&client_address, addrlen);

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	inet_pton (AF_INET, SERVER_ADDRESS , &(server_address.sin_addr));

	scanf ("%[^\n]", message);
  	num_chars = sendto(sockfd, &message, strlen(message) + 1, 0, (struct sockaddr *) &server_address, addrlen);
	bzero (&message, MSG_LEN*sizeof (char));
	recvfrom(sockfd, &message, num_chars, 0, (struct sockaddr *)&server_address, &addrlen);
	printf ("Echo from server = %s\n", message);

	close(sockfd);

	return 0;
}
