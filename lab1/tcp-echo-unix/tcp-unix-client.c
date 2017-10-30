#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#define MSG_LEN 1024

int main() {
	int sockfd, n; 
	socklen_t addrlen;
	struct sockaddr_un unix_server_addr, unix_client_addr;
	char message[MSG_LEN];
	const char path_unix[] = "/Downloads";

	if((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0))<0) {
		perror ("socket");
		return -1;
	}


	addrlen = sizeof (struct sockaddr_un);
	bzero (&unix_server_addr, sizeof (unix_server_addr));	
	bzero (&unix_client_addr, sizeof (unix_client_addr));

	unix_server_addr.sun_family = AF_LOCAL;
	unlink (path_unix);
	strncpy (unix_server_addr.sun_path, path_unix, sizeof (unix_server_addr.sun_path)-1);

	if ( connect (sockfd, (struct sockaddr *) &unix_server_addr, addrlen) < 0) {
       perror ("connect");
       return -1;
	}
	
	/* read a line */
 	printf ("client: message to server = ");
	scanf ("%[^\n]", message);

	n = send (sockfd, message, strlen (message) + 1, 0);
	bzero (message, MSG_LEN * sizeof(char));
	recv (sockfd, message, MSG_LEN, 0);
	printf ("client: echo from server = %s\n", message);

	shutdown (sockfd, SHUT_RDWR);

	return 0;
}
