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


#define N_BYTES 1024
char *ROOT;
int serverfd, clientfd,fd;
void startServer(char *);
void respond();

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

	if (listen(serverfd, 100000) != 0)
	{
		perror("listen");
		exit(1);
	}
}

void respond(){
	char message[99999], *request_line[3], send_buffer[N_BYTES], path[99999];
	int bytes_read;
	memset((void *)message, 0, 99999 );
	if(recv(clientfd, message, 99999, 0)<0){
		fprintf(stderr,("recv()\n"));
	} else {
		printf("%s", message);
		request_line[0] = strtok (message, " \t\n");
		if(strncmp(request_line[0],"GET\0",4)==0) {
			request_line[1] = strtok (NULL, " \t");
			request_line[2] = strtok (NULL, " \t\n");
			if ( strncmp( request_line[2], "HTTP/1.0", 8)!=0 && strncmp( request_line[2], "HTTP/1.1", 8)!=0 ) {
				
				write(clientfd, "HTTP/1.0 400 Bad Request\n", 25);

			}else {
				strcpy(path, ROOT);
				strcpy(&path[strlen(ROOT)], request_line[1]);
				printf("file: %s\n", path);

				if((fd = open(path, O_RDONLY)) != -1) {
					send(clientfd, "HTTP/1.0 200 OK\n\n", 17, 0);
					
					while((bytes_read = read(fd, send_buffer, N_BYTES))>0) {		
						write(clientfd, send_buffer, bytes_read);						
					}
				}else {   
						write(clientfd, "HTTP/1.0 404 Not Found\n", 23);
				}
			}		
		}

		shutdown (clientfd, SHUT_RDWR);
		close(clientfd);
	}
}

int main(int argc, char *argv) {
	struct sockaddr_in clientaddr;
	socklen_t addrlen;
	char *port;
	port = "8080";
	ROOT = "/home/aditya/Downloads/network-programming/lab3";
	start_server(port);
	printf("server started\n");
	
	addrlen = sizeof(clientaddr);
	while(1) {
		clientfd = accept(serverfd, (struct sockaddr *) &clientaddr, &addrlen);
		respond();
				
	}
	return 0;
}
