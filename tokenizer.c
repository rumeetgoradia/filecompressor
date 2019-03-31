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

void populate_arrs(char ** codes, char ** tokens, int size, char * input, int length) {
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
				codes[index] = (char *)malloc(sizeof(string) + 1);
				strncpy(codes[index], string, (sizeof(string)));
			} else {
				tokens[index] = (char *)malloc(sizeof(string) + 1);
				strncpy(tokens[index], string, (sizeof(string)));
				++index;
			}
			free(string);	
		} else {
			++string_length;
		}
	}
}
