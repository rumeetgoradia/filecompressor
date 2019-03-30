#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "filecompress.h"

llist_node * head = NULL;

void insert_list(char * token, unsigned int freq) {
	llist_node * temp;
	llist_node * ptr;
	temp = (llist_node *)malloc(sizeof(llist_node));
	if (temp == NULL) {
		printf("Insufficient memory.\n");
		return;
	}
	temp->freq = freq;
	temp->token = (char *)malloc((sizeof(char) * strlen(token)) + 1);
	strcpy(temp->token, token);
	if (head == NULL) {
		head = temp;
		head->next = NULL;
//		printf("inserting %s\n in first place", temp->token);
		return;
	}
	if (strcmp(head->token, temp->token) == 0) {
		return;
	}
	if (temp->freq < head->freq) {
		temp->next = head;
		head = temp;
//		printf("inserting %s\n", temp->token);
		return;
	}
	ptr = head;
	while (ptr->next != NULL) {
		if(strcmp(ptr->next->token, temp->token) == 0) {
			return;
		}
		if (temp->freq < ptr->next->freq) {
			temp->next = ptr->next;
			ptr->next = temp;
//			printf("inserting %s\n", temp->token);
			return;
		}
		ptr = ptr->next;
	}
	ptr->next = temp;
	temp->next = NULL;
//	printf("inserting %s\n", temp->token);
	return;
}

int main() {
	int size = 0;
	insert_list("a", 5);
	insert_list("b", 9);
	insert_list("c", 12);
	insert_list("d", 13);
	insert_list("e", 16);
	insert_list("f", 45);
	size = 6;
	print_list(head);
	printf("about to huff\n");
	huffman(size,head);
	return EXIT_SUCCESS;
}
