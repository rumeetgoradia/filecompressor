/*********************************
* Aditi Singh and Rumeet Goradia *
* as2811, rug5                   *
* CS214: Section 04              *
**********************************/
int count_codebook(char * input, int length);
int compress(int fd, char * input, int length, char ** codes, char ** tokens, int size);
void decompress(int fd, char * input, int length, char ** codes, char ** tokens, int size);
