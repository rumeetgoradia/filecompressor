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
