#include<string.h>
#include "avl.h"

node * search(int freq, node * n) {
	if(n == NULL) {
		return NULL;
	}
	if(freq < n->data) {
		return search(freq, n->left);
	} else if(freq > n->data) {
		return find(freq, n->right);
	} else {
	        return n;
	}
}
