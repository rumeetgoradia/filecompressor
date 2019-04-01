/*********************************
* Aditi Singh and Rumeet Goradia *
* as2811, rug5                   *
* CS214: Section 04              *
**********************************/
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<limits.h>
#include<dirent.h>
#include"fcdatastructs.h"
#include"tokenizer.h"

/* Head of linked list of tokens */
llist_node * head = NULL;

/* Insert tokens into linked list or update their frequencies */
int insert_list(char * token) {
	llist_node * temp;
	llist_node * ptr;
	temp = (llist_node *)malloc(sizeof(llist_node));
	if (temp == NULL) {
		printf("Insufficient memory.\n");
		return -1;
	}
	temp->freq = 1;
	temp->token = (char *)malloc(strlen(token) + 1);
	strncpy(temp->token, token, (strlen(token)));
	/* Case 1: New list */
	if (head == NULL) {
		head = temp;
		temp->next = NULL;
		return 1;
	}
	/* Case 2: Only one node */
	if (strcmp(token, (head->token)) == 0) {
		/* If token already in list, just update its frequency */
		++head->freq;
		return 0;
	}
	if (temp->freq <= head->freq && head->next == NULL) {
		/* Generally inputting in ascending frequency order */
		temp->next = head;
		head = temp;
		return 1;
	}
	/* Case 3: Token already exists somewhere in list of multiple nodes */
	ptr = head;
	while (ptr->next != NULL) {
		if(strcmp(ptr->next->token, token) == 0) {
			++ptr->next->freq;
			return 0;
		}
		ptr = ptr->next;
	}
	/* Case 4: Put token at end */
	ptr->next = temp;
	temp->next = NULL;
	return 1;
}

/* Separate input into tokens and return how many unique tokens were registerd */
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
	/* Traverse through entire input string, keeping track of unique tokens with count */
	for (i = 0; i < length; i++) {
		/* If current char is not a delimiter, increase the length of the token */
		if (input[i] != '\t' && input[i] != '\n' && input[i] != ' ') {
			++token_len;
			last_was_sep = 0;
			current_sep = 0;
		/* If current char is a delimiter and previous char wasn't, create token */
		} else if (last_was_sep == 0) {
			char * token = (char *)malloc(sizeof(char) * (token_len + 1));
			if (token == NULL) {
				printf("Error: Insufficient memory.\n");
				return -1;
			}
			int freq = 0;
			for (j = 0; j < token_len; ++j) {
				token[j] = input[pos_last_sep + j];
			}
			token[token_len] = '\0';
			/* Change index of most recent delimiter for next token creation */
			pos_last_sep += token_len + 1;
			int inc = insert_list(token);
			if (inc == -1) {
				return -1;
			}
			count += inc;
			last_was_sep = 1;
			token_len = 0;
			current_sep = 1;
		/* If current char is a delimiter and previous char was also, do not create token 
 		 * and update position of last delimiter */ 
		} else {
			++pos_last_sep;
			current_sep = 1;
		}
		/* If current char is a delimiter, enter it into linked list of tokens */
		if (current_sep == 1) {
			int inc = 0;
			if(input[i] == ' ') {
				inc = insert_list("~)!(@s*#&$^");
				if (inc == -1) {
					return -1;
				}
			} else if (input[i] == '\t') {
				inc = insert_list("~)!(@t*#&$^");
				if (inc == -1) {
					return -1;
				}
			} else {
				inc = insert_list("~)!(@n*#&$^");
				if (inc == -1) {
					return -1;
				}
			}
			count += inc;
		}
	}
	/* If final char isn't a delimiter, create a new token out of remaining chars */
	if (token_len > 0) {
		char * token = (char *)malloc(sizeof(char) * (token_len + 1));
		if (token == NULL) {
			printf("Error: Insufficient memory.\n");
			return -1;
		}
		for (j = 0; j < token_len; ++j) {
			token[j] = input[pos_last_sep + j];
		}
		token[token_len] = '\0';
		int inc = insert_list(token);
		if (inc == -1) {
			return -1;
		}
		count += inc;
	}
	return count;
}

/* Parse through codebook's input for the individual codes and corresponding tokens */
void populate_arrs(char ** codes, char ** tokens, char * input, int length) {
	int i = 0, j = 0;
	int last_whitespace = 0;
	int string_length = 0;
	int index = 0;
	for (i = 0; i < length - 1; ++i) {
		/* Nearly identical process to tokenize() */
		if (input[i] == '\t' || input[i] == '\n') {
			char * string = (char *)malloc(sizeof(char) * (string_length + 1));
			for (j = 0; j < string_length; ++j) {
				string[j] = input[last_whitespace + j];
			}
			string[string_length] = '\0';
			last_whitespace += string_length + 1;
			string_length = 0;
			if (input[i] == '\t') {
				/* Codebook will always have codes before the tabs
 				 * and tokens after. */
				codes[index] = (char *)malloc(strlen(string) + 1);
				strncpy(codes[index], string, (strlen(string)));
			} else {
				tokens[index] = (char *)malloc(strlen(string) + 1);
				strncpy(tokens[index], string, (strlen(string)));
				++index;
			}
			free(string);
		/* If current char is not a delimiter, just increase the length of the string */
		} else {
			++string_length;
		}
	}
}

/* One recursive function to handle all recursive operations */
int recursive_function(int fd, char * file, char flag, char ** codes, char ** tokens, int size) {
	DIR * dr;
	struct dirent * de;
	int sum = 0;
	if (!(dr = opendir(file))) {
		printf("Error: Could not open %s. Please try again.\n", file);
		return -1;
	}
	/* Traverse through directory while it still has files or subdirectories */
	while ((de = readdir(dr)) != NULL) {
		/* Concatenate current file / directory name to current path to create the
		 * current file's or directory's actual path */
		char new_path[1024];
		snprintf(new_path, sizeof(new_path), "%s/%s", file, de->d_name);
		/* If it's a directory, recurse */
		if (de->d_type == DT_DIR) {	
			/* Check to make sure no recursion is occuring on the parent directory
 			 * or the current directory */ 
			if (strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0) {
				continue;
			}
			sum += recursive_function(fd, new_path, flag, codes, tokens, size);
		/* If it's a file, get the file contents in a string and perform the correct
		 * operation on it. */
		} else {
			int fd_file = open(new_path, O_RDONLY);
			char * temp = (char *)malloc(sizeof(char) * INT_MAX);
			int total_length = read(fd_file, temp, INT_MAX);
			char * input = (char *)malloc(sizeof(char) * (total_length + 1));
			strcpy(input, temp); 
			free(temp);
			if (flag == 'b') {
				/* Sum counts the number of unique tokens throughout the files 
 				 * in the current directory */
				sum += tokenize(input);
			} else if (flag == 'c') {
				char * hczfile = strcat(new_path, ".hcz");
				int fd_hcz = open(hczfile, O_WRONLY | O_CREAT , 0644);
				if (fd_hcz < 0) {
					printf("Error: Cannot create or open %s. Skipping compression of %s.\n", hczfile, new_path);
					continue;
				}
				compress(fd_hcz, input, total_length, codes, tokens, size);
				close(fd_hcz);
			} else if (flag == 'd') {
				char * test = (char *)malloc(sizeof(char) * 5);
				test = new_path + strlen(new_path) - 4;
				/* Only operate on file if it's a .hcz file */
				if (strcmp(test, ".hcz") != 0) {
					continue;
				}
				char * resfile = new_path;
				resfile[strlen(new_path) - 4] = '\0';	
				int fd_res = open(resfile, O_WRONLY | O_CREAT , 0644);
				if (fd_res < 0) {
					printf("Error: Cannot create or open %s. Skipping decompression of %s.\n", resfile, new_path);
					continue;
				}
				decompress(fd_res, input, total_length, codes, tokens, size);
				close(fd_res);
			}
		}
	}
	closedir(dr);
	return sum;
}

int main(int argc, char ** argv) {
	/* Minimum number of arguments is 3, and maximum is 5 */
	if (argc < 3) {
		printf("Error: Not enough arguments. Please try again.\n");
		return EXIT_FAILURE;
	} else if (argc > 5) {
		printf("Error: Too many arguments. Please try again.\n");
		return EXIT_FAILURE;
	}
	int recursive = 0;
	int is_file = 1;
	/* Check if recursive flag is set */
	if (strcmp("-R",argv[1]) == 0 || strcmp("-R",argv[2]) == 0) {
		recursive = 1;
	}
	char * file = argv[3 + (recursive - 1)];
	char flag = ' ';
	/* Check which flag is set, or break if no correct one is set */
	if (recursive) {
		if (strcmp("-R", argv[1]) == 0) {
			flag = argv[2][1];
		} else {
			flag = argv[1][1];
		}
	} else {
		flag = argv[1][1];
	}
	if (flag != 'b' && flag != 'c' && flag != 'd') {
		printf("Error: Invalid flag(s). Please try again with -b, -c, or -d.\n");
		return EXIT_FAILURE;
	}
	if (recursive) {
		struct stat pstat;
		if (stat(file, &pstat) != 0) {
			printf("Error: You lack permissions to the file or path specified, or it doesn't exist. Please try again.\n");
			return EXIT_FAILURE;
		}
		/* Check if user inputted a directory or a file */
		is_file = S_ISREG(pstat.st_mode);
		if (is_file) {
			int i = i;
			int path_check = 0;
			for (i = 0; i < strlen(file); ++i) {
				if (file[i] == '/') {
					path_check = 1;
					break;
				}
		
			}
			/* Issue warning if user entered a file but without a path */
			if (!path_check) {
				printf("Warning: You specified a file, not a path. Continuing operation.\n");
			}
		}
		/* Get rid of trailing '/' if present */
		if (file[strlen(file) - 1] == '/') {
			file[strlen(file) - 1] = '\0';
		}
	}
	/* Recursively operating on a file is essentially the same as no recursion */
	if (is_file) {
		int fd_file = open(file, O_RDONLY);
		if (fd_file < 0) {
			printf("Error: Could not open file \"%s\". Please try again.\n", file);
			return EXIT_FAILURE;
		}
		/* Since length of file contents is unknown, give it maximum space first and then
 		 * copy non-garbage contents into properly sized string */  
		char * temp = (char *)malloc(sizeof(char) * INT_MAX);
		int total_length = read(fd_file, temp, INT_MAX);
		char * input = (char *)malloc(sizeof(char) * (total_length + 1));
		strcpy(input, temp);
		free(temp);
		if (flag == 'b') {
			if (argc > (4 + recursive - 1)) {
				printf("Error: Too many arguments. Please try again.\n");
				return EXIT_FAILURE;
			} else if (argc < (4 + recursive - 1)) {
				printf("Error: Not enough arguments. Please try again.\n");
				return EXIT_FAILURE;
			}
			int fd_codebook = open("./HuffmanCodebook", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_codebook < 0) {
				printf("Error: Could not open or create HuffmanCodebook. Please try again.\n");
				return EXIT_FAILURE;
			}
			/* Size holds the number of unique tokens */
			int size = tokenize(input);
			if (size > 0) {
				huffman(size, head, fd_codebook);
			} 
/*			else if (size == 1) {
				write(fd_codebook,"0\t",2);
				write(fd_codebook, head->token, strlen(head->token));
				write(fd_codebook,"\n", 1);
			} */
			write(fd_codebook,"\n",1);
			close(fd_codebook);
		}
		if (flag == 'c' || flag == 'd') {
			if (argc > (5 + recursive - 1)) {
				printf("Error: Too many arguments. Please try again.\n");
				return EXIT_FAILURE;
			} else if (argc < (5 + recursive - 1)) {
				printf("Error: Not enough arguments. Please try again.\n");
				return EXIT_FAILURE;
			}
			int fd_codebook = open(argv[argc - 1], O_RDONLY);
			if (fd_codebook < 0) {
				printf("Error: Could not open file \"%s\". Please try again.\n", argv[argc - 1]);
				return EXIT_FAILURE;
			}
			/* Get all contents of file all in one string using previous method */
			temp = (char *)malloc(sizeof(char) * INT_MAX);
			int codebook_length = read(fd_codebook, temp, INT_MAX);
			char * codebook_input = (char *)malloc(sizeof(char) * (codebook_length + 1));
			strcpy(codebook_input, temp);
			free(temp);
			/* Get total number of tokens in codebook */
			int size = count_codebook(codebook_input, codebook_length);
			char ** codes = (char **)malloc(sizeof(char *) * size);
			char ** tokens = (char **)malloc(sizeof(char *) * size);
			/* Put tokens and codes from codebook into respective arrays */
			populate_arrs(codes, tokens, codebook_input, codebook_length);
			if (flag == 'c') {
				/* Create new file name with .hcz extension */
				char * hczfile = strcat(file, ".hcz");
				int fd_hcz = open(hczfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (fd_hcz < 1) {
					printf("Error: Could not open or create file \"%s\". Please try again.\n", hczfile);
					return EXIT_FAILURE;
				}
				compress(fd_hcz, input, total_length, codes, tokens, size);
				close(fd_hcz);
			} else {
				char * test = (char *)malloc(sizeof(char) * 5);
				test = file + strlen(file) - 4;
				/* Ensure that file is an .hcz file */
				if (strcmp(test, ".hcz") != 0) {
					printf("Error: Decoding requires a .hcz file.\n");
					return EXIT_FAILURE;
				}
				/* Cut off last four chars to get rid of .hcz and make new file */
				char * resfile = file;
				resfile[strlen(file) - 4] = '\0';	
				int fd_res = open(resfile, O_WRONLY | O_CREAT , 0644);
				if (fd_res < 1) {
					printf("Error: Could not open or create file \"%s\". Please try again.\n", resfile);
					return EXIT_FAILURE;
				}
				decompress(fd_res, input, total_length, codes, tokens, size);
				close(fd_res);
			}
			close(fd_codebook);
		}
		close(fd_file);
	/* Separate operation for recursion of a directory */
	} else {
		if (flag == 'b') {
			if (argc < 4) {
				printf("Error: Not enough arguments. Please try again.\n");
				return EXIT_FAILURE;
			} else if (argc > 4) {
				printf("Error: Too many arguments. Please try again.\n");
				return EXIT_FAILURE;
			}
			int fd_codebook = open("./HuffmanCodebook", O_WRONLY | O_CREAT | O_TRUNC, 0644);	
			if (fd_codebook < 0) {
				printf("Error: Could not create or open HuffmanCodebook. Please try again.\n");
				return EXIT_FAILURE;
			}
			/* Perform recursive function and get number of unique tokens */
			int size = recursive_function(fd_codebook, file, flag, NULL, NULL, 0);
			/* Create a single HuffmanCodebook for all files */
			if (size > 0) {
				huffman(size, head, fd_codebook);	
			} 
/*			else if (size == 1) {
				write(fd_codebook,"0\t",2);
				write(fd_codebook, head->token, strlen(head->token));
				write(fd_codebook,"\n", 1);
			}	
*/
			write(fd_codebook,"\n",1);
			close(fd_codebook);
		} else if (flag == 'c' || flag == 'd') {
			if (argc < 5) {
				printf("Error: Not enough arguments. Please try again.\n");
				return EXIT_FAILURE;
			} else if (argc > 5) {
				printf("Error: Too many arguments. Please try again.\n");
				return EXIT_FAILURE;
			}
			int fd_codebook = open(argv[argc - 1], O_RDONLY);
			if (fd_codebook < 0) {
				printf("Error: Could not open file \"%s\". Please try again.\n", argv[argc - 1]);
				return EXIT_FAILURE;
			}
			/* Get contents of codebook into one string */
			char * temp = (char *)malloc(sizeof(char) * INT_MAX);
			int codebook_length = read(fd_codebook, temp, INT_MAX);
			char * codebook_input = (char *)malloc(sizeof(char) * (codebook_length + 1));
			strcpy(codebook_input, temp);
			free(temp);
			/* Get total number of tokens in codebook */
			int size = count_codebook(codebook_input, codebook_length);
			char ** codes = (char **)malloc(sizeof(char *) * size);
			char ** tokens = (char **)malloc(sizeof(char *) * size);
			/* Put tokens and codes into respective arrays */
			populate_arrs(codes, tokens, codebook_input, codebook_length);
			/* Both compression and decompression can use the same recursive_function call */
			recursive_function(0, file, flag, codes, tokens, size);
			free(codes);
			free(tokens);
			close(fd_codebook);
		}
	}
	free(head);
	return EXIT_SUCCESS;
}
