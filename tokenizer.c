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
	int token_length = 0;
	int last_whitespace = 0;
	int space_index = search(tokens, size, "~)!(@s*#&$^");
	int tab_index = search(tokens, size, "~)!(@t*#&$^");
	int nl_index = search(tokens, size, "~)!(@n*#&$^");
	for (i = 0; i < length; ++i) {
		if (input[i] == '\t' || input[i] == '\n' || input[i] == ' ') {
			if (token_length > 0) {
				char * string = (char *)malloc(sizeof(char) * (token_length + 1));
				for (j = 0; j < token_length; ++j) {
					string[j] = input[last_whitespace + j];
				}
				string[token_length] = '\0';
				last_whitespace += token_length + 1;
				token_length = 0;
				write(fd, codes[search(tokens, size, string)], strlen(codes[search(tokens, size, string)]));
				free(string);
			} else {
				++last_whitespace;
			}
			if (input[i] == ' ') {
				write(fd, codes[space_index], strlen(codes[space_index]));
			} else if (input[i] == '\t') {
				write(fd, codes[tab_index], strlen(codes[tab_index]));
			} else {
				write(fd, codes[nl_index], strlen(codes[nl_index]));
			}
		} else {
			++token_length;
		}
	}
}

void decompress(int fd, char * input, int length, char ** codes, char ** tokens, int size) {
	int i = 0, j = 0;
	int last_break = 0;
	int code_length = 0;
	for (i = 0; i < length; ++i) {
		++code_length;
		char * code = (char *)malloc(sizeof(char) * (code_length + 1));
		for (j = 0; j < code_length; ++j) {
			code[j] = input[last_break + j];
		}
		code[code_length] = '\0';
		int index = search(codes, size, code);
		if (index >= 0) {
			if (strcmp(tokens[index], "~)!(@s*#&$^") == 0) {
				write(fd, " ", strlen(" "));
			} else if (strcmp(tokens[index], "~)!(@t*#&$^") == 0) {
				write(fd, "\t", strlen("\t"));
			} else if (strcmp(tokens[index], "~)!(@n*#&$^") == 0) {
				write(fd, "\n", strlen("\n"));
			} else {
				write(fd, tokens[index], strlen(tokens[index]));
			}
			last_break += code_length;
			code_length = 0;
		}
		free(code);
	}
}
