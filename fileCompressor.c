#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<limits.h>
#include "fcdatastructs.h"

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

int main(int argc, char ** argv) {
	/*Open relevant files.*/
	int fd_codebook = open("./HuffmanCodebook.hcz", O_CREAT | O_RDWR, 0644);
	char * file = strcat("./", argv[1]);
	int fd_file = open(file, O_RDONLY);

	/*Store entire file into one string that will be tokenized later.*/
	char * temp = malloc(sizeof(char)*INT_MAX);
	int total_length = read(fd_file, temp, INT_MAX);
	char * input = malloc(sizeof(char)*total_length + 1);
	strcpy(input, temp);
	free(temp);
	
	/*Additional setup*/
	write(fd_codebook,"\\\n", 2);
	int i = 0;
	
	unsigned int size = 3;
	/*Test code*/
/*	insert_list("a", 5);
	insert_list("<space>",5);
	insert_list("dog", 5);
	insert_list("cat", 5);
	insert_list("button", 5);
	insert_list("tab", 5);
	insert_list("ball", 5);
	insert_list("and", 5);
	size = 8; 
//	print_list(head);
//	printf("about to huff\n"); */
	huffman(size, head, fd_codebook);
	write(fd_codebook,"\n",1);
	close(fd_codebook);
	return EXIT_SUCCESS;
}
