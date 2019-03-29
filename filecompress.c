#include<string.h>
#include<stdlib.h>
#include "filecompress.h"

node * createNode(char * token, unsigned int freq) {
	node * ptr = (node *)malloc(sizeof(node));
	ptr->left = NULL;
	ptr->right = NULL;
	ptr->token = token;
	ptr->freq = freq;
	return ptr;
}

tree * createTree(unsigned int max_size) {
	tree * ptr = (tree *)malloc(sizeof(tree));
	ptr->size = 0;
	ptr->max_size = max_size;
	ptr->arr = (node **)malloc(max_size * sizeof(node *));
	return root;
}
