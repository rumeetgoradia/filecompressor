#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<limits.h>
#include "fcdatastructs.h"

llist_node * head = NULL;

void insert_list(char * token) {
	llist_node * temp;
	llist_node * ptr;
	temp = (llist_node *)malloc(sizeof(llist_node));
	if (temp == NULL) {
		printf("Insufficient memory.\n");
		return;
	}
	temp->freq = 1;
	temp->token = token;
//	printf("token: %s\n", token);
	/*Case 1: New List*/
	if (head == NULL) {
//		printf("entering first case\n");
		head = temp;
		temp->next = NULL;
//		print_list(head);
		return;
	}
//	printf("going to case 2\n");	
	if (strcmp(token, (head->token)) == 0) {
//		printf("got past comparison\n");
		++head->freq;
//		print_list(head);
		return;
	}
//	printf("going to case 3\n");
	if (temp->freq <= head->freq) {
		temp->next = head;
		head = temp;
//		printf("inserting %s\n", temp->token);
//		print_list(head);
		return;
	}
	ptr = head;
//	printf("entering while\n");
	while (ptr->next != NULL) {
		if(strcmp(ptr->next->token, token) == 0) {
			++ptr->next->freq;
//			print_list(head);
			return;
		}
		if (temp->freq <= ptr->next->freq) {
			temp->next = ptr->next;
			ptr->next = temp;
//			printf("inserting %s", temp->token);
//			print_list(head);
			return;
		}
		ptr = ptr->next;
	}
	ptr->next = temp;
	temp->next = NULL;
	return;
}

unsigned int tokenize(char * input) {
	if (input == NULL || strlen(input) == 0) {
		return 0;
	}
	char * token;
	int i = 0, j = 0;
	int pos_last_sep = 0;
	int token_len = 0;
	int length = strlen(input);
	int last_was_sep = 1;
	int count = 0;	
//	printf("input string: %s\n", input);
	for (i = 0; i < length; i++) {
		if (input[i] != '\t' && input[i] != '\n' && input[i] != ' ') {
			++token_len;
			last_was_sep = 0;	
		} else if (last_was_sep == 0) {
			token = (char *)malloc(sizeof(char) * (token_len + 1));
			if (token == NULL) {
				printf("Insufficient memory.\n");
				return EXIT_FAILURE;
			}		
			int freq = 0;
			for (j = 0; j < token_len; ++j) {
//				printf("Enter inner for loop\n");
				token[j] = input[pos_last_sep + j];
			}
			token[token_len] = '\0';
			pos_last_sep += token_len + 1;
//			printf("Inserting\n");
			insert_list(token);
//			printf("inserted\n");
			++count;
			last_was_sep = 1;
			token_len = 0;
		}
		else {
			++pos_last_sep;
		}
	}
//	printf("finished for loop in tokenize\n");
	if (token_len > 0) {
		token = (char *)malloc(sizeof(char) * (token_len + 1));
		if (token == NULL) {
			printf("Insufficient memory.\n");
			return EXIT_FAILURE;
		}
		for (j = 0; j < token_len; ++j) {
			token[j] = input[pos_last_sep + j];
		}
		token[token_len] = '\0';
		insert_list(token);
		++count;
	}
	free(token);
	return count;
}

int main(int argc, char ** argv) {
	/*Open relevant files.*/
	int fd_codebook = open("./HuffmanCodebook.hcz", O_CREAT | O_RDWR, 0644);
	char * prefix = (char *)malloc(sizeof(char) * (3+strlen(argv[1])));
	prefix[0] = '.';
	prefix[1] = '/';
	char * file = strcat(prefix, argv[1]);
	int fd_file = open(file, O_RDONLY);



	/*Store entire file into one string that will be tokenized later.*/
	char * temp = malloc(sizeof(char)*INT_MAX);
	int total_length = read(fd_file, temp, INT_MAX);
	char * input = malloc(sizeof(char)*total_length + 1);
	strcpy(input, temp);
	free(temp);
	printf("finished getting input string\n");
	
	/*Additional setup*/
	write(fd_codebook,"\\\n", 2);
	
	
	unsigned int size = tokenize(input);
	printf("finished tokenizing\n");
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
	close(fd_file);
	free(head);
	return EXIT_SUCCESS;
}
