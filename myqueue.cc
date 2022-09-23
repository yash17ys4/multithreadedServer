#include "myqueue.h"
#include <stdlib.h>
#include <stdio.h>

node_t * head = NULL;
node_t * tail = NULL;

void enqueue(int *client_socket) {
	node_t * newnode = (node_t *)malloc(sizeof(node_t));
	newnode->client_socket = client_socket;
	newnode->next = NULL;
	if(tail == NULL) {
		head = newnode;
	} else {
		tail->next = newnode;
	}
	tail = newnode;
	// printf("NQ1 %d", *(head->client_socket));
	printf("NQ2 %d", *(tail->client_socket));
}

int * dequeue() {
	if(head == NULL) {
		return NULL;
	} else {
		int *result = head->client_socket;
		node_t* temp = head;
		head = head->next;
		if(head == NULL) {
			tail == NULL;
		}
		free(temp);
		return result;
	}
}

int size() {
	if(head == NULL) return 0;
	int count = 0;
	node_t* iter = head;
	while(iter != tail) {
		iter = iter->next;
		count++;
	}
	return count+1;
}