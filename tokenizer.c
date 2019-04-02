/*********************************
* Aditi Singh and Rumeet Goradia *
* as2811, rug5                   *
* CS214: Section 04              *
**********************************/
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

/* Count number of lines in codebook */
int count_codebook(char * input, int length) {
	int count = 0;
	int i = 0;
	for (i = 0; i < length; ++i) {
		/* Number of lines is equal to number of tabs in properly formatted codebook */
		if (input[i] == '\t') {
			++count;
		}
	}
	return count;
}

/* Return index of given string in array */
int search(char ** arr, int size, char * string) {
	int i = 0;
	for (i = 0; i < size; ++i) {
	if (strcmp(arr[i], string) == 0) {

			return i;
		}
	}
	return -1;
}

/* Parse file for tokens and write corresponding code to .hcz file */
int compress(int fd, char * input, int length, char ** codes, char ** tokens, int size) {
	int i = 0, j = 0;
	int token_length = 0;
	int last_whitespace = 0;
	/* Get indices for whitespace tokens first because they are relatively common */
	int space_index = search(tokens, size, "~)!(@s*#&$^");
	int tab_index = search(tokens, size, "~)!(@t*#&$^");
	int nl_index = search(tokens, size, "~)!(@n*#&$^");
	for (i = 0; i < length; ++i) {
		/* Use previous method for tokenizing input */
		if (input[i] == '\t' || input[i] == '\n' || input[i] == ' ') {
			if (token_length > 0) {
				char * string = (char *)malloc(sizeof(char) * (token_length + 1));
				for (j = 0; j < token_length; ++j) {
					string[j] = input[last_whitespace + j];
				}
				string[token_length] = '\0';
				last_whitespace += token_length + 1;
				token_length = 0;
				/* Write the code that corresponds to the token */
				int index = search(tokens, size, string);
				if (index != -1) {
					write(fd, codes[index], strlen(codes[index]));
				/* If token is not present in codebook, return with error message (should not happen under normal operation) */
				} else {
					printf("Error: No corresponding code for token \"%s\". Exiting.\n");
					return -1;
				}
				free(string);
			} else {
				++last_whitespace;
			}
			/* Also write the code that corresponds to the current whitespace */
			if (input[i] == ' ') {
				write(fd, codes[space_index], strlen(codes[space_index]));
			} else if (input[i] == '\t') {
				write(fd, codes[tab_index], strlen(codes[tab_index]));
			} else {
				write(fd, codes[nl_index], strlen(codes[nl_index]));
			}
		/* If current char is not a separator, just increase token length */
		} else {
			++token_length;
		}
	}
	return 0;
}

/* Read chars from input, and if series of input chars is equal to a code, write the 
 * corresponding token */
void decompress(int fd, char * input, int length, char ** codes, char ** tokens, int size) {
	int i = 0, j = 0;
	int last_break = 0;
	int code_length = 0;
	for (i = 0; i < length; ++i) {
		/* Since no separators, always increase the length of the string */
		++code_length;
		char * code = (char *)malloc(sizeof(char) * (code_length + 1));
		for (j = 0; j < code_length; ++j) {
			code[j] = input[last_break + j];
		}
		code[code_length] = '\0';
		/* Check if code exists within codebook; if yes, then write corresponding token */
		int index = search(codes, size, code);
		if (index >= 0) {
			/* If token is special token for whitespace, write corresponding
 			 * whitespace */	
			if (strcmp(tokens[index], "~)!(@s*#&$^") == 0) {
				write(fd, " ", strlen(" "));
			} else if (strcmp(tokens[index], "~)!(@t*#&$^") == 0) {
				write(fd, "\t", strlen("\t"));
			} else if (strcmp(tokens[index], "~)!(@n*#&$^") == 0) {
				write(fd, "\n", strlen("\n"));
			} else {
				write(fd, tokens[index], strlen(tokens[index]));
			}
			/* Advance the last breakpoint in input and reset length of code */
			last_break += code_length;
			code_length = 0;
		}
		free(code);
	}
}
