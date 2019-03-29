#define AVLTREE

typedef struct node {
	char * token;
	int frequency;
	struct node * left;
	struct node * right;
} node;

node * search(int freq, char * token, node * n, node * parent);
node * get_max(node * n);
node * get_min(node * n);
node * insert(int freq, char * token, node * n);
