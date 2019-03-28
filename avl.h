#define AVLTREE

typedef struct node {
	char * token;
	int frequency;
	struct node * left;
	struct node * right;
} node;

node * search(int freq, node * n);
node * get_max(node * n);
node * get_min(node * n);
node * insert(char * token, int freq, node * n);
