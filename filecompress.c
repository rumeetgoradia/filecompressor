#include<string.h>
#include<stdlib.h>
#include "filecompress.h"

tree_node * create_node(char * token, unsigned int freq) {
	tree_node * ptr = (tree_node *)malloc(sizeof(tree_node));
	ptr->left = NULL;
	ptr->right = NULL;
	ptr->token = token;
	ptr->freq = freq;
	return ptr;
}

tree * create_tree(unsigned int max_size) {
	tree * ptr = (tree *)malloc(sizeof(tree));
	ptr->size = 0;
	ptr->max_size = max_size;
	ptr->arr = (tree_node **)malloc(max_size * sizeof(tree_node *));
	return root;
}

void heapify(tree * root, int index) {
	int base = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;
	if (left < root->size && root->node_arr[left]->freq < root->node_arr[base]->freq) {
		base = left;
	}
	if (right < root->size && root->node_arr[right]->freq < root->node_arr[base]->freq) {
		base = right;
	}
	if (base != index) {
		tree_node * temp = root->(node_arr + base);
		root->(node_arr + base) = root->(node_arr + index);
		root->(node_arr + index) = temp;
		heapify(root, base);
	}
}

node * remove_min(tree * root) {
	tree_node * ptr = root->node_arr[0];
	root->node_arr[0] = root->node_arr[root->size - 1];
	root->size = root->size - 1;
	heapify(root, 0);
	return ptr;
}

void insert_tree_node(tree * root, tree_node * node) {
	++(root->size);
	int i = root->size - 1;
	while (i > 0 && node->freq < tree->node_arr[(i - 1)/2]->freq) {
		tree->node_arr[i] = tree->node_arr[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	tree->node_arr[i] = node;
}

void build_tree(tree * root) {
	int i = 0;
	int j = root->size - 1;
	for (i = (j - 1) / 2; i >= 0; --i) {
		heapify(tree, i);	
	}
}

tree * full_tree(llist_node * node, unsigned int size) {
	tree * root = create_tree(size);
	int i = 0;
	for (i = 0; i < size; ++i) {
		tree->node_arr[i] = create_node(node->token,node->freq);
		/*need to edit based on Aditi's implementation*/
	}
	root->size = size
	build_tree(root);
	return root;
}

tree * build_huffman(llist node * node, unsigned int size) {
	tree_node * parent;
	tree_node * left;
	tree_node * right;
	tree * root = full_tree(node, size);
	while (root->size != 1) {
		left = remove_min(root);
		right = remove_min(root);
		parent = create_node("/\\", left->freq + right->freq);
		parent->left = left;
		parent->right = right;
		insert_tree_node(root, parent);
	}
	return remove_min(root);
}

void set_codes(tree_node * node, unsigned int code_arr[], int parent) {
	if (node->left != NULL) {
		code_arr[parent] = 0;
		set_codes(node->left, code_arr, parent+1);
	}
	if (node->right != NULL) {
		code_arr[parent] = 1;
		set_codes(node->right, code_arr, parent+1);
	} 
	if (node->left == NULL && node->right == NULL) {
		printf("%s ", node->token);
		int = 0;
		for (i = 0; i < parent; ++i) {
			printf("%d", code_arr[i]);
		}
		printf("\n");
	}
}

void huffman(unsigned int size, llist_node * node) {
	tree_node * base = build_huffman(node, size);
	int codes[size];
	set_codes(base, codes, size);
}

