#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
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
	if (temp->freq <= head->freq) {
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
		if (temp->freq <= ptr->next->freq) {
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
	int fd = open("./HuffmanCodebook.hcz", O_CREAT | O_RDWR, 0644);
	write(fd,"\\\n", 2);
	insert_list("a", 5);
	insert_list("<space>",5);
	insert_list("dog", 5);
	insert_list("cat", 5);
	insert_list("button", 5);
	insert_list("tab", 5);
	insert_list("ball", 5);
	insert_list("and", 5);
	size = 8;
//	print_list(head);
//	printf("about to huff\n");
	huffman(size, head, fd);
	write(fd,"\n",1);
	close(fd);
	return EXIT_SUCCESS;
}
