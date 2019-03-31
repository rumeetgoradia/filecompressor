#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<limits.h>
#include "fcdatastructs.h"
#include "tokenizer.h"

llist_node * head = NULL;

int insert_list(char * token) {
//	printf("inserting %s\n", token);
	llist_node * temp;
	llist_node * ptr;
	temp = (llist_node *)malloc(sizeof(llist_node));
	if (temp == NULL) {
		printf("Insufficient memory.\n");
		return 0;
	}
	temp->freq = 1;
	temp->token = (char *)malloc(strlen(token) + 1);
	strncpy(temp->token, token, (strlen(token)));
	/*Case 1: New List*/
	if (head == NULL) {
		head = temp;
		temp->next = NULL;
//		printf("put in case 1");
		return 1;
	}
	if (strcmp(token, (head->token)) == 0) {
		++head->freq;
//		printf("put in case 2");
		return 0;
	}
	if (temp->freq >= head->freq && head->next == NULL) {
		temp->next = head;
		head = temp;
//		printf("put in case 3");
		return 1;
	}
	ptr = head;
	while (ptr->next != NULL) {
		if(strcmp(ptr->next->token, token) == 0) {
			++ptr->next->freq;
//			printf("put in case 4");
			return 0;
		}
/*		if (temp->freq <= ptr->next->freq) {
			temp->next = ptr->next;
			ptr->next = temp;
			return 1;
		}*/
		ptr = ptr->next;
	}
	ptr->next = temp;
	temp->next = NULL;
//	printf("Put at end\n");
	return 1;
}

unsigned int tokenize(char * input) {
	if (input == NULL || strlen(input) == 0) {
		return 0;
	}
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
			char * token = (char *)malloc(sizeof(char) * (token_len + 1));
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
			free(token);
		}
		else {
			++pos_last_sep;
			current_sep = 1;
		}
		if (current_sep == 1) {
			if(input[i] == ' ') {
				count += insert_list("~)!(@s*#&$^");
			} else if (input[i] == '\t') {
				count += insert_list("~)!(@t*#&$^");
			} else {
//				printf("inserting new line\n");
				count += insert_list("~)!(@n*#&$^");
			}
		}
	}
	if (token_len > 0) {
		char * token = (char *)malloc(sizeof(char) * (token_len + 1));
		if (token == NULL) {
			printf("Insufficient memory.\n");
			return EXIT_FAILURE;
		}
		for (j = 0; j < token_len; ++j) {
			token[j] = input[pos_last_sep + j];
		}
		token[token_len] = '\0';
		count += insert_list(token);
		free(token);
	}
	return count - 1;
}

void populate_arrs(char ** codes, char ** tokens, char * input, int length) {
	int i = 0, j = 0;
	int last_whitespace = 0;
	int string_length;
	int index = 0;
	for (i = 0; i < length - 1; ++i) {
		if (input[i] == '\t' || input[i] == '\n') {
			char * string = (char *)malloc(sizeof(char) * (string_length + 1));
			for (j = 0; j < string_length; ++j) {
				string[j] = input[last_whitespace + j];
			}
			string[string_length] = '\0';
			last_whitespace += string_length + 1;
			string_length = 0;
			if (input[i] == '\t') {
				codes[index] = (char *)malloc(strlen(string) + 1);
				strncpy(codes[index], string, (strlen(string)));
			} else {
				tokens[index] = (char *)malloc(strlen(string) + 1);
				strncpy(tokens[index], string, (strlen(string)));
				++index;
			}
			free(string);	
		} else {
			++string_length;
		}
	}
}

int main(int argc, char ** argv) {
	int recursive = 0;
	if (strcmp("-R",argv[1]) == 0 && strcmp("-R",argv[2]) == 0) {
		recursive = 1;
	}
	char * file = argv[3 + (recursive - 1)];
	char flag = ' ';
	if (recursive) {
		if (strcmp("-R", argv[1]) == 0) {
			flag = argv[2][1];
		} else {
			flag = argv[1][1];
		}
	} else {
		flag = argv[1][1];
	}
	int fd_file = open(file, O_RDONLY);
	char * temp = (char *)malloc(sizeof(char) * INT_MAX);
	int total_length = read(fd_file, temp, INT_MAX);
	char * input = (char *)malloc(sizeof(char) * (total_length + 1));
	strcpy(input, temp);
//	printf("%s\n%d\n", input, total_length);
	free(temp);
	int fd_codebook = open("./HuffmanCodebook", O_RDWR | O_CREAT | O_APPEND, 0644);
	if (flag == 'b') {
		int size = tokenize(input) + 1;
		if (size > 1) {
			huffman(size, head, fd_codebook);	
		} else if (size == 1) {
			write(fd_codebook,"0\t",2);
			write(fd_codebook, head->token, strlen(head->token));
			write(fd_codebook,"\n", 1);
		}
		write(fd_codebook,"\n",1);
	}
	if (flag == 'c' || flag == 'd') {
		temp = (char *)malloc(sizeof(char) * INT_MAX);
		int codebook_length = read(fd_codebook, temp, INT_MAX);
		char * codebook_input = (char *)malloc(sizeof(char) * (codebook_length + 1));
		strcpy(codebook_input, temp);
		free(temp);
		int size = count_codebook(codebook_input, codebook_length);
		char ** codes = (char **)malloc(sizeof(char *) * size);
		char ** tokens = (char **)malloc(sizeof(char *) * size);
		populate_arrs(codes, tokens, codebook_input, codebook_length);
/*		int i = 0;
		for (i = 0; i < size; ++i) {
			printf("%s\t%s\n", codes[i], tokens[i]);
		}*/
		if (flag == 'c') {
			char * hczfile = strcat(file, ".hcz");
			int fd_hcz = open(hczfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
			compress(fd_hcz, input, total_length, codes, tokens, size);
			close(fd_hcz);
		} else {
			file[total_length - 4] = '\0';
//			char * resfile = file;
			char * resfile = "result.txt";
			int fd_res = open(resfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
			decompress(fd_res, input, total_length, codes, tokens, size);
			close(fd_res);
		}
	}
	close(fd_codebook);
	close(fd_file);
	free(head);
	return EXIT_SUCCESS;
}
