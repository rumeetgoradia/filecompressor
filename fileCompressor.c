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

llist_node * head = NULL;

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
	if (temp->freq >= head->freq && head->next == NULL) {
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
				printf("Error: Insufficient memory.\n");
				return -1;
			}
			int freq = 0;
			for (j = 0; j < token_len; ++j) {
				token[j] = input[pos_last_sep + j];
			}
			token[token_len] = '\0';
			pos_last_sep += token_len + 1;
			int inc = insert_list(token);
			if (inc == -1) {
				return -1;
			}
			count += inc;
			last_was_sep = 1;
			token_len = 0;
			current_sep = 1;
		}
		else {
			++pos_last_sep;
			current_sep = 1;
		}
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

void populate_arrs(char ** codes, char ** tokens, char * input, int length) {
	int i = 0, j = 0;
	int last_whitespace = 0;
	int string_length = 0;
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

int recursive_function(int fd, char * file, char flag, char ** codes, char ** tokens, int size) {
	DIR * dr;
	struct dirent * de;
	int sum = 0;
	if (!(dr = opendir(file))) {
		return 0;
	}
	while ((de = readdir(dr)) != NULL) {
		char new_path[1024];
		snprintf(new_path, sizeof(new_path), "%s/%s", file, de->d_name);
		if (de->d_type == DT_DIR) {	
			if (strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0) {
				continue;
			}
			sum += recursive_function(fd, new_path, flag, codes, tokens, size);
		} else {
			int fd_file = open(new_path, O_RDONLY);
			char * temp = (char *)malloc(sizeof(char) * INT_MAX);
			int total_length = read(fd_file, temp, INT_MAX);
			char * input = (char *)malloc(sizeof(char) * (total_length + 1));
			strcpy(input, temp); 
			free(temp);
			if (flag == 'b') {
				sum += tokenize(input);
			} else if (flag == 'c') {
				char * hczfile = strcat(new_path, ".hcz");
				int fd_hcz = open(hczfile, O_WRONLY | O_CREAT , 0644);
				compress(fd_hcz, input, total_length, codes, tokens, size);
				close(fd_hcz);
			} else if (flag == 'd') {
				char * test = (char *)malloc(sizeof(char) * 5);
				test = new_path + strlen(new_path) - 4;
				if (strcmp(test, ".hcz") != 0) {
					continue;
				}
				char * resfile = new_path;
				resfile[strlen(new_path) - 4] = '\0';	
				int fd_res = open(resfile, O_WRONLY | O_CREAT , 0644);
				decompress(fd_res, input, total_length, codes, tokens, size);
				close(fd_res);
			}
		}
	}
	closedir(dr);
	return sum;
}

int main(int argc, char ** argv) {
	if (argc < 3) {
		printf("Error: Not enough arguments. Please try again.\n");
		return EXIT_FAILURE;
	} else if (argc > 5) {
		printf("Error: Too many arguments. Please try again.\n");
		return EXIT_FAILURE;
	}
	int recursive = 0;
	int is_file = 1;
	if (strcmp("-R",argv[1]) == 0 || strcmp("-R",argv[2]) == 0) {
		recursive = 1;
	}
	char * file = argv[3 + (recursive - 1)];
/*	char * path = (char *)malloc(strlen(file) + 1);
	strncpy(path, file, strlen(file)); */
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
	if (recursive) {
		struct stat pstat;
		if (stat(file, &pstat) != 0) {
			printf("Error: You lack permissions to the file or path specified, or it doesn't exist. Please try again.\n");
			return EXIT_FAILURE;
		}
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
			if (!path_check) {
				printf("Warning: You specified a file, not a path. Results may be undefined.\n");
			}
		}
//		printf("%s\n", path);
	}
	if (is_file) {
		int fd_file = open(file, O_RDONLY);
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
//			printf("building\n");
			int fd_codebook = open("./HuffmanCodebook", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//			printf("%s\n", input);
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
		}
		if (flag == 'c' || flag == 'd') {
			if (argc > (5 + recursive - 1)) {
				printf("Error: Too many arguments. Please try again.\n");
				return EXIT_FAILURE;
			} else if (argc < (5 + recursive - 1)) {
				printf("Error: Not enough arguments. Please try again.\n");
				return EXIT_FAILURE;
			}
			int fd_codebook = open(argv[argc - 1], O_RDWR);
			temp = (char *)malloc(sizeof(char) * INT_MAX);
			int codebook_length = read(fd_codebook, temp, INT_MAX);
			char * codebook_input = (char *)malloc(sizeof(char) * (codebook_length + 1));
			strcpy(codebook_input, temp);
			free(temp);
			int size = count_codebook(codebook_input, codebook_length);

			char ** codes = (char **)malloc(sizeof(char *) * size);
			char ** tokens = (char **)malloc(sizeof(char *) * size);
			populate_arrs(codes, tokens, codebook_input, codebook_length);
/*			int i = 0;
			for (i = 0; i < size; ++i) {
				printf("%s\t%s\n", codes[i], tokens[i]);
			}*/
			if (flag == 'c') {
				char * hczfile = strcat(file, ".hcz");
				int fd_hcz = open(hczfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
/*				int i = 0;
	                       	for (i = 0; i < size; ++i) {
					printf("%s\t%s\n", codes[i], tokens[i]);
				} */
				compress(fd_hcz, input, total_length, codes, tokens, size);
				close(fd_hcz);
			} else {
				char * test = (char *)malloc(sizeof(char) * 5);
				test = file + strlen(file) - 4;
				if (strcmp(test, ".hcz") != 0) {
					printf("Error: Decoding requires a .hcz file.\n");
					return EXIT_FAILURE;
				}
				char * resfile = file;
				resfile[strlen(file) - 4] = '\0';	
				int fd_res = open(resfile, O_WRONLY | O_CREAT , 0644);
				decompress(fd_res, input, total_length, codes, tokens, size);
				close(fd_res);
			}
			close(fd_codebook);
		}
		close(fd_file);
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
			int size = recursive_function(fd_codebook, file, flag, NULL, NULL, 0);
			if (size > 1) {
				huffman(size, head, fd_codebook);	
			} else if (size == 1) {
				write(fd_codebook,"0\t",2);
				write(fd_codebook, head->token, strlen(head->token));
				write(fd_codebook,"\n", 1);
			}	
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
			char * temp = (char *)malloc(sizeof(char) * INT_MAX);
			int codebook_length = read(fd_codebook, temp, INT_MAX);
			char * codebook_input = (char *)malloc(sizeof(char) * (codebook_length + 1));
			strcpy(codebook_input, temp);
			free(temp);
			int size = count_codebook(codebook_input, codebook_length);
			char ** codes = (char **)malloc(sizeof(char *) * size);
			char ** tokens = (char **)malloc(sizeof(char *) * size);
			populate_arrs(codes, tokens, codebook_input, codebook_length);
			recursive_function(0, file, flag, codes, tokens, size);
			free(codes);
			free(tokens);
			close(fd_codebook);
		}
	}
	free(head);
	return EXIT_SUCCESS;
}
