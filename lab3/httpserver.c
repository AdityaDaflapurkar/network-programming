#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>



#define MSG_LEN 1024
char *ROOT;
int serverfd;
void startServer(char *);

void start_server(char *port) {
	struct addrinfo hints, *res, *c_addr;
	memset(&hints,0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if(getaddrinfo(NULL, port, &hints, &res) != 0) {
		perror("getaddrinfo");
		exit(1);	
	}

	for(c_addr = res; res!=NULL; res = res->ai_next) {
		if((serverfd = socket(c_addr->ai_family, c_addr->ai_socktype, 0))<0){
			continue;		
		}
		if (bind(serverfd, c_addr->ai_addr, c_addr->ai_addrlen) == 0) break;
	}

	if (c_addr==NULL)
	{
		perror ("socket() or bind()");
		exit(1);
	}

	freeaddrinfo(res);

	if (listen(serverfd, 10) != 0)
	{
		perror("listen");
		exit(1);
	}
}

int main(int argc, char *argv) {
	char port[6] = "8080";
	ROOT = (char *)malloc(6*sizeof(char));
	ROOT = "localhost";
	start_server(port);
	printf("server started");
}
