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

	struct sockaddr_in clnt1Addr;
	socklen_t clnt1AddrLen = sizeof(clnt1Addr);

	// Wait for a client to connect
	int clnt1Sock = accept(servSock, (struct sockaddr *) &clnt1Addr, &clnt1AddrLen);
	if (clnt1Sock < 0) {
		perror("accept() failed");
		exit(-1);
	}

	char clnt1IpAddr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &clnt1Addr.sin_addr.s_addr, 
			clnt1IpAddr, sizeof(clnt1IpAddr)) != NULL) {
		printf("----\nHandling client1 %s %d\n", 
				clnt1IpAddr, ntohs(clnt1Addr.sin_port));
	} else {
		puts("----\nUnable to get client1 IP Address");
	}
	struct sockaddr_in clnt2Addr;
	socklen_t clnt2AddrLen = sizeof(clnt2Addr);

	// Wait for a client to connect
	int clnt2Sock = accept(servSock, (struct sockaddr *) &clnt2Addr, &clnt2AddrLen);
	if (clnt2Sock < 0) {
		perror("accept() failed");
		exit(-1);
	}

	char clnt2IpAddr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &clnt2Addr.sin_addr.s_addr, 
			clnt2IpAddr, sizeof(clnt2IpAddr)) != NULL) {
		printf("----\nHandling client2 %s %d\n", 
				clnt2IpAddr, ntohs(clnt2Addr.sin_port));
	} else {
		puts("----\nUnable to get client2 IP Address");
	}
	int pid;
	ssize_t recvLen = 0;
	
    // This thread handles messages from client1
	if((pid = fork()) == 0) {
		char buffer[BUFSIZE];
		memset(buffer, 0, BUFSIZE);
        while ((recvLen = recv(clnt1Sock, buffer, BUFSIZE - 1, 0)) > 0) {
            // printf("\n1 to 2: ");
            buffer[recvLen] = '\n';
            // fputs(buffer,stdout);
			// Send string to client
            ssize_t sentLen = send(clnt2Sock, buffer, recvLen, 0);
    		if (sentLen < 0) {
    			perror("send() failed");
    			exit(-1);
    		} 
    		else if (sentLen != recvLen) {
    			perror("send(): sent unexpected number of bytes");
    			exit(-1);
    		}
        }
        exit(0);
    }
    // handles messages from client2
    else{
    	char buffer[BUFSIZE];
		memset(buffer, 0, BUFSIZE);
        while ((recvLen = recv(clnt2Sock, buffer, BUFSIZE - 1, 0)) > 0) {
            // printf("\n2 to 1: ");
            buffer[recvLen] = '\n';
            // fputs(buffer,stdout);
			// Send string to client
            ssize_t sentLen = send(clnt1Sock, buffer, recvLen, 0);
    		if (sentLen < 0) {
    			perror("send() failed");
    			exit(-1);
    		} 
    		else if (sentLen != recvLen) {
    			perror("send(): sent unexpected number of bytes");
    			exit(-1);
    		}
        }
        exit(0);
    }

	close(clnt1Sock);
	close(clnt2Sock);

	printf("End of Program\n");

}
