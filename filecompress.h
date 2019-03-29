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
	/*Aditi put your node definition here*/
} llist_node;

/*node * createNode(char * token, unsigned int freq);
tree * createTree(unsigned int cap);
void insert(node * ptr; tree * root);
void buildTree(tree * root);
tree * createHeap(, unsigned int size);
node * buildHuffman(node, unsigned int size);*/
void huffman(unsigned int size, llist_node * node);
