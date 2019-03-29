#include<string.h>
#include "avl.h"

node * search(int freq, char * token, node * n, node * parent) {
	if(n == NULL) {
		return NULL;
	}
	if(freq < n->data) {
		return search(freq, token, n->left, n);
	} else if(freq > n->data) {
		return find(freq, token, n->right, n);
	} else if (strcmp(token,n->token) == 0)
	        return n;
	} else if (parent->left != NULL && strcmp(token,parent->left->token) == 0) {
		return parent->left;
	} else if (parent->right != NULL && strcmp(token,parent->right->token) == 0) {
		return parent->right;
	}
	return NULL;
}


