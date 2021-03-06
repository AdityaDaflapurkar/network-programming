#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#define MSG_LEN 1024

int main() {
	int sockfd, clientfd, n, sock_length;
	struct sockaddr_un unix_server_addr, unix_client_addr;
	char message[MSG_LEN];
	const char path_unix[] = "/Downloads";

	if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0))<0) {
		perror ("socket");
		return -1;
	}


	bzero (&unix_server_addr, sizeof (unix_server_addr));
	sock_length = sizeof (unix_server_addr);
    bzero (&unix_client_addr, sizeof (unix_client_addr));

	unix_server_addr.sun_family = AF_LOCAL;
	unlink (path_unix);
	strncpy (unix_server_addr.sun_path, path_unix, sizeof (unix_server_addr.sun_path)-1);

	if (bind (sockfd, (struct sockaddr *) &unix_server_addr, sizeof (struct sockaddr_un)) < 0) {
		perror ("bind");
		return -1;
	}	

	listen (sockfd, 5);

	clientfd = accept (sockfd, (struct sockaddr *) &unix_client_addr, &sock_length);
    if (clientfd < 0) {
		perror ("accept");
		return -1;
	}
   
	n = recv (clientfd, &message, MSG_LEN, 0);
	printf ("server: message from client = %s\n", message);
 	n = send (clientfd, &message, MSG_LEN, 0);

	shutdown (clientfd, SHUT_RDWR);
	shutdown (sockfd, SHUT_RDWR);

	return 0;
}
