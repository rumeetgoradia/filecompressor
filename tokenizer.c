#include<string.h>
#include<stdio.h>
#include<stdlib.h>

int count_codebook(char * input, int length) {
	int count = 0;
	int i = 0;
	for (i = 0; i < length; ++i) {
		if (input[i] == '\t') {
			++count;
		}
	}
	return count;
}

int search(char ** arr, int size, char * string) {
	int i = 0;
	for (i = 0; i < size; ++i) {	
		if (strcmp(arr[i], string) == 0) {
			return i;
		}
	}
	return -1;
}

void compress(int fd, char * input, int length, char ** codes, char ** tokens, int size) {
	int i = 0, j = 0;
	int token_length;
	int last_whitespace = 0;
	int space_index = search(tokens, size, " ");
	int nl_index = search(tokens, size, "\\n");
	for (i = 0; i < length; ++i) {
		if (input[i] == '\t' || input[i] == '\n' || input[i] == ' ') {
			char * string = (char *)malloc(sizeof(char) * (token_length + 1));
			for (j = 0; j < token_length; ++j) {
				string[j] = input[last_whitespace + j];
			}
			string[token_length] = '\0';
			last_whitespace += token_length + 1;
			token_length = 0;
			write(fd, codes[search(tokens, size, string)], sizeof(codes[search(tokens, size, string)]));
			free(string);
			if (input[i] == ' ') {
				write(fd, codes[space_index], sizeof(codes[space_index]));
			} else if (input[i] == '\n') {
				write(fd, codes[nl_index], sizeof(codes[nl_index]));
			}
		} else {
			++token_length;
		}
	}
}
