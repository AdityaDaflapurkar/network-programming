#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	struct sockaddr_in server_addr;
	struct hostent *remote_server;
	int clientfd;
	char *hostname, *page, *recvbuffer;
	recvbuffer = (char *)malloc(1024 * sizeof(char));
	hostname = "localhost";
	page = "/index.html";

	if((clientfd = socket(AF_INET, SOCK_STREAM, 0))<0) {
		printf("Error -- socket\n");
		exit(1);	
	}

	remote_server = gethostbyname(hostname);

	bcopy((char*)remote_server->h_addr, (char*)&server_addr.sin_addr, remote_server->h_length);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);

	if(connect(clientfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr))<0) {
		printf("Error -- connection\n");
		exit(1);
	}

	char* template = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: HTMLGET 1.0\r\n\r\n";

	if(page[0]=='/')
		page++;
	int requestlen = strlen(template) + strlen(hostname) + strlen(page) - 3;
	char request[requestlen+1];

	sprintf(request, template, page, hostname);

	int total_sent = 0, curr_sent = 0;

	while(total_sent < strlen(request))
	{
		curr_sent += send(clientfd, request + total_sent, strlen(request)-total_sent, 0); 
		if(curr_sent<0)
		{	
			printf("Error -- cant send query\n");
			exit(1);
		}
		total_sent += curr_sent;
	}
	bzero(recvbuffer, sizeof(recvbuffer));
	
	while(recv(clientfd, recvbuffer, sizeof(recvbuffer), 0)>0) {
		printf("%s",recvbuffer);	
		bzero(recvbuffer, sizeof(recvbuffer));
	}
	return 0;
}
