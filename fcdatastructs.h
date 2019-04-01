/********************************* 
* Aditi Singh and Rumeet Goradia *
* as2811, rug5			 *
* CS214: Section 04		 *
**********************************/
/* Using a minheap structure: tree_nodes are the individual values of the minheap and the tree
 * is the minheap. Functions to optimize and arrange each of these are in fcdatastructs.c */
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
