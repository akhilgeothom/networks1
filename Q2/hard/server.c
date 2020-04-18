#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


#define BUFSIZE 1024
#define N 10
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// struct SockInfo{
	int clntSock[N];
	struct sockaddr_in clntAddr[N];
	char clntIpAddr[N][INET_ADDRSTRLEN];
	int active[N]={0};
// }sockInfo;

static const int MAXPENDING = 5; // Maximum outstanding connection requests

void *socketHelper(void *arg){
	int recvLen;
	char buffer[BUFSIZE];
	int i= *((int *)arg);
	i--;
	printf("%d\n",i );
	memset(buffer, 0, BUFSIZE);
    while ((recvLen = recv(clntSock[i], buffer, BUFSIZE - 1, 0)) > 0) {
        // printf("\nto server: ");
        // buffer[recvLen] = '\n';
        int clientNum = buffer[recvLen-1] -'A';
        buffer[--recvLen] = '\n';
        
        fputs(buffer,stdout);
        
	    if(active[clientNum]==1){
	        ssize_t sentLen = send(clntSock[clientNum], buffer, recvLen, 0);
			if (sentLen < 0) {
				perror("send() failed");
				exit(-1);
			} 
			else if (sentLen != recvLen) {
				perror("send(): sent unexpected number of bytes");
				exit(-1);
			}
		}
		else{
			printf("Invalid clientNum\n");
		}
    }
    printf("closing socket %d\n", i);
	pthread_exit(NULL);

    // return NULL;
}

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

	int i=0;
	pthread_t tid[N];
	while(i<N) {
		// Wait for a client to connect
		socklen_t clntAddrLen = sizeof(clntAddr[0]);
		clntSock[i] = accept(servSock, (struct sockaddr *) &clntAddr[i], &clntAddrLen);
		if (clntSock[i] < 0) {
			perror("accept() failed");
			exit(-1);
		}
		if (inet_ntop(AF_INET, &clntAddr[i].sin_addr.s_addr, 
				clntIpAddr[i], sizeof(clntIpAddr)) != NULL) {
			printf("----\nHandling client%d %s %d\n", i, 
					clntIpAddr[i], ntohs(clntAddr[i].sin_port));
		} else {
			puts("----\nUnable to get client IP Address");
		}

		if( pthread_create(&tid[i], NULL, socketHelper, &i) != 0 ){
			printf("Failed to create thread\n");
		}
		active[i] =1;
		i++;
	}

	i = 0;
	while(i < N)
	{
	  pthread_join(tid[i++],NULL);
	}

	for(int i=0;i<N;i++) {
		close(clntSock[i]);
	}

	printf("End of Program\n");

}
