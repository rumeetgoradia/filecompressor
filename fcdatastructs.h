/********************************* 
* Aditi Singh and Rumeet Goradia *
* as2811, rug5			 *
* CS214: Section 04		 *
**********************************/
typedef struct tree_node {
	char * token;
	unsigned int freq;
	struct tree_node *left, *right;
} tree_node;

typedef struct tree {
	unsigned int size;
	unsigned int max_size;
	tree_node ** node_arr;
} tree;

typedef struct llist_node {
	unsigned int freq;
	char * token;
	struct llist_node * next;
} llist_node;

void huffman(int size, llist_node * node, int fd);
