#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFSIZE 1024

int main(int argc, char **argv) {

	if (argc != 4) {
		perror("<Server Address> <Server Port> <Echo Word>");
		exit(-1);
	}
	
	clock_t now, later;

	char *servIP = argv[1];
	char *echoString = argv[3];
	
	// Set port number as given by user or as default 12345
	// in_port_t servPort = (argc == 3) ? atoi(argv[2]) : 12345;
	
	// Set port number as user specifies
	in_port_t servPort = atoi(argv[2]);
	
	//Creat a socket
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("socket() failed");
		exit(-1);
	}
	
	// Set the server address
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;

	
	int err = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
	if (err <= 0) {
		perror("inet_pton() failed");
		exit(-1);
	}
	servAddr.sin_port = htons(servPort);
	
	// Connect to server
	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("connect() failed");
		exit(-1);
	}
	
	size_t echoStringLen = strlen(echoString);
	
	char parity='\0';
	for(int i=0;i<echoStringLen;i++){
		parity += echoString[i];
	}
	echoString[echoStringLen] = parity;

	// parity = parity % 128;
	// char *parity_str = &parity;
	// char *toSent = strcat(parity_str,echoString);
	// int senLen = strlen(toSent);
	
	// printf("parity = %c\n",parity);
	// printf("toSent = %s\n",toSent);
	// printf("senLen = %ld", strlen(toSent));
	now = clock();
	// Send string to server
	ssize_t sentLen = send(sockfd, echoString, echoStringLen+1, 0);
	if (sentLen < 0) {
		perror("send() failed");
		exit(-1);
	} else if (sentLen != echoStringLen+1) {
		perror("send(): sent unexpected number of bytes");
		exit(-1);
	}

	// Receive string from server
	fputs("Received: ", stdout);
	unsigned int totalRecvLen = 0;

	while (totalRecvLen < echoStringLen) {
		char buffer[BUFSIZE];
		memset(buffer, 0, BUFSIZE);
		ssize_t recvLen = recv(sockfd, buffer, BUFSIZE - 1, 0);
		if (recvLen < 0) {
			perror("recv() failed");
			exit(-1);
		} else if (recvLen == 0) {
			perror("recv() connection closed prematurely");
			exit(-1);
		}
	
		totalRecvLen += recvLen;
		buffer[recvLen] = '\n';
		fputs(buffer, stdout);	
	}
	later = clock();

	double ping = (later-now)*1000.0/CLOCKS_PER_SEC/2;

	printf("ping=%fm s\n",ping);
	
	close(sockfd);
	exit(0);
}
