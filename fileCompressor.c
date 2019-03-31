#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<limits.h>
#include "fcdatastructs.h"

llist_node * head = NULL;

int insert_list(char * token) {
	llist_node * temp;
	llist_node * ptr;
	temp = (llist_node *)malloc(sizeof(llist_node));
	if (temp == NULL) {
		printf("Insufficient memory.\n");
		return 0;
	}
	temp->freq = 1;
	temp->token = token;
	/*Case 1: New List*/
	if (head == NULL) {
		head = temp;
		temp->next = NULL;
		return 1;
	}
	if (strcmp(token, (head->token)) == 0) {
		++head->freq;
		return 0;
	}
	if (temp->freq <= head->freq && head->next == NULL) {
		temp->next = head;
		head = temp;
		return 1;
	}
	ptr = head;
	while (ptr->next != NULL) {
		if(strcmp(ptr->next->token, token) == 0) {
			++ptr->next->freq;
			return 0;
		}
		if (temp->freq <= ptr->next->freq) {
			temp->next = ptr->next;
			ptr->next = temp;
			return 1;
		}
		ptr = ptr->next;
	}
	ptr->next = temp;
	temp->next = NULL;
	return 1;
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
	int current_sep = 1;
	int count = 0;	
	for (i = 0; i < length; i++) {
		if (input[i] != '\t' && input[i] != '\n' && input[i] != ' ') {
			++token_len;
			last_was_sep = 0;
			current_sep = 0;
		} else if (last_was_sep == 0) {
			token = (char *)malloc(sizeof(char) * (token_len + 1));
			if (token == NULL) {
				printf("Insufficient memory.\n");
				return EXIT_FAILURE;
			}		
			int freq = 0;
			for (j = 0; j < token_len; ++j) {
				token[j] = input[pos_last_sep + j];
			}
			token[token_len] = '\0';
			pos_last_sep += token_len + 1;
			count += insert_list(token);
			last_was_sep = 1;
			token_len = 0;
			current_sep = 1;
		}
		else {
			++pos_last_sep;
			current_sep = 1;
		}
		if (current_sep == 1) {
			if(input[i] == ' ') {
				count += insert_list(" ");
			} else if (input[i] == '\t') {
				count += insert_list("\\t");
			} else {
				count += insert_list("\\n");
			}
		}
	}

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
		count += insert_list(token);
	}
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
	
	/*Additional setup*/
	write(fd_codebook,"\\\n", 2);
		
	int size = tokenize(input);
	if (size > 1) {
		huffman(size, head, fd_codebook);	
	} else if (size == 1) {
		write(fd_codebook,"0\t",2);
		write(fd_codebook, head->token, strlen(head->token));
		write(fd_codebook,"\n", 1);
	}
	write(fd_codebook,"\n",1);
	close(fd_codebook);
	close(fd_file);
	free(head);
	return EXIT_SUCCESS;
}
