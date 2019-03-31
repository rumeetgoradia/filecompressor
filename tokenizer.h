int count_codebook(char * input, int length);
void populate_arrs(char ** codes, char ** tokens, char * input, int length);
void compress(int fd, int length, char ** codes, char ** tokens);
