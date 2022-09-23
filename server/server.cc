#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <bits/stdc++.h>
#include <pthread.h>
#include "../myqueue.h"

#define PORT 8080
#define BUFFSIZE 4096
#define SERVER_BACKLOG 100
#define SOCKETERROR (-1)
#define THREAD_POOL_SIZE 1

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void * handle_connection(void * clientSocket);
int check(int exp, const char *msg);
void * thread_function(void * arg);

int main(int argc, char const* argv[])
{
	int serverSocket, clientSocket, addrSize;
	SA_IN serverAddr, clientAddr;

	//first create pool of threads
	for(int i = 0; i < THREAD_POOL_SIZE; i++) {
		pthread_create(&thread_pool[i], NULL, thread_function, NULL);
	}

	//create socket, addr strct, bind, and listen
	check((serverSocket = socket(AF_INET, SOCK_STREAM, 0)),
		"Failed to create the socket"
	);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	check(bind(serverSocket, (SA*)&serverAddr, sizeof(serverAddr)),
		"Binding failed!"
	);

	check(listen(serverSocket, SERVER_BACKLOG),
		"Listen Failed!"
	);

	while(true) {
		printf("Waiting for connections ...\n");

		addrSize = sizeof(SA_IN);
		check(clientSocket = accept(serverSocket, (SA*)&clientAddr, (socklen_t*)&addrSize),
			"Accept failed"
		);
		printf("Connected!");

		int * pclient = (int*)malloc(sizeof(int));
		*pclient = clientSocket;
		pthread_mutex_lock(&mutex);
		enqueue(pclient);
		std::cout<<"enqueued "<<(*pclient)<<" size: " << size()<< "\n";
		pthread_mutex_unlock(&mutex);
		// handle_connection(pclient);

	}

	close(clientSocket);
	shutdown(serverSocket, SHUT_RDWR);

	return 0;
}

void * handle_connection(void * p_clientSocket) {
	int clientSocket = *((int*)p_clientSocket);
	free(p_clientSocket);
	char buffer[BUFFSIZE];
	read(clientSocket, buffer, 1024);

	int number = atoi(buffer);
	number = number * number;
	std::cout<<number<<" will be sent\n";

	char n_char[BUFFSIZE];
	int len = strlen(n_char);
	std::sprintf(n_char, "%d", number);
	send(clientSocket, n_char, len, 0);

	close(clientSocket);
	return NULL;
}

int check(int exp, const char * msg) {
	if(exp == SOCKETERROR) {
		printf("%s\n", msg);
		exit(exp);
	}
	return exp;
}

void * thread_function(void * arg) {
	while(true) {
		int *pclientA;
		pthread_mutex_lock(&mutex);
		pclientA = dequeue();
		pthread_mutex_unlock(&mutex);
		if(pclientA != NULL) {
			int temp = *pclientA;

			std::cout<<"handle_connection will be called "<< temp << "\n"; 
			handle_connection(pclientA);
			std::cout<<"handle_connection was called "<< temp << "\n"; 
		}
	}
}