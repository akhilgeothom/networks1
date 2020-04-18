#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


#define BUFSIZE 1024

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main(int argc, char ** argv) {

	if (argc != 2) {
		perror("<server port>");
		exit(-1);
	}

	in_port_t servPort = atoi(argv[1]); // Local port

	// create socket for incoming connections
	int servSock;
	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket() failed");
		exit(-1);
	}

	// Set local parameters
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(servPort);

	// Bind to the local address
	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind() failed");
		exit(-1);
	}

	// Listen to the client
	if (listen(servSock, MAXPENDING) < 0) {
		perror("listen() failed");
		exit(-1);
	}

	// Server Loop

	struct sockaddr_in clntAddr;
	socklen_t clntAddrLen = sizeof(clntAddr);

	// Wait for a client to connect
	int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
	if (clntSock < 0) {
		perror("accept() failed");
		exit(-1);
	}

	char clntIpAddr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, 
			clntIpAddr, sizeof(clntIpAddr)) != NULL) {
		printf("----\nHandling client %s %d\n", 
				clntIpAddr, ntohs(clntAddr.sin_port));
	} else {
		puts("----\nUnable to get client IP Address");
	}
	int pid;
	ssize_t recvLen = 0;
	
    // thread to receive messages from client 
	if((pid = fork()) == 0) {
		char buffer[BUFSIZE];
		memset(buffer, 0, BUFSIZE);
        while ((recvLen = recv(clntSock, buffer, BUFSIZE - 1, 0)) > 0) {
            printf("\nMessage Received: ");
            buffer[recvLen] = '\n';
            fputs(buffer,stdout);
            //An extra breaking condition can be added here (to terminate the child process)            
        }
        exit(0);
    }
    // thread to send messages to client 
    else{
    	char message[BUFSIZE] = "";
		memset(message, 0, BUFSIZE);

		while(1) {
    		printf("\nEnter a message: ");
    	    fgets(message, BUFSIZE, stdin);
			// Send string to server
    	    ssize_t sentLen = send(clntSock, message, strlen(message), 0);
    		if (sentLen < 0) {
    			perror("send() failed");
    			exit(-1);
    		} 
    		else if (sentLen != strlen(message)) {
    			perror("send(): sent unexpected number of bytes");
    			exit(-1);
    		}
    	}
    }

	close(clntSock);
	// printf("End of Server Loop\n");
	

	printf("End of Program\n");

}
