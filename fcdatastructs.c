/*********************************
* Aditi Singh and Rumeet Goradia *
* as2811, rug5                   *
* CS214: Section 04              *
**********************************/
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "fcdatastructs.h"

/* Create a node for Huffman tree using parameters */
tree_node * create_node(char * token, unsigned int freq) {	
	tree_node * ptr = (tree_node *)malloc(sizeof(tree_node));
	ptr->left = NULL;
	ptr->right = NULL;
	ptr->token = (char *)malloc(strlen(token) + 1);
	strncpy(ptr->token, token, (strlen(token)));	
	ptr->freq = freq;
	return ptr;
}

/* Initializes minheap tree */
tree * create_tree(unsigned int max_size) {
	tree * ptr = (tree *)malloc(sizeof(tree));
	ptr->size = 0;
	ptr->max_size = max_size;
	ptr->node_arr = (tree_node **)malloc(max_size * sizeof(tree_node *));
	return ptr;
}
/* Utility function that swaps tree_node lists at specific index */
void swap_nodes(tree_node ** first, tree_node ** second) {
	tree_node * temp = *first;
	*first = *second;
	*second = temp;
}

/* Heapify function based on algorithm */
void heapify(tree * root, int index) {
	int base = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;
	if (left < root->size && root->node_arr[left] != NULL && root->node_arr[base] != NULL && root->node_arr[left]->freq < root->node_arr[base]->freq) {
		base = left;
	}
	if (right < root->size && root->node_arr[right] != NULL && root->node_arr[base] != NULL && root->node_arr[right]->freq < root->node_arr[base]->freq) {
		base = right;
	}
	if (root->node_arr[base] != NULL && root->node_arr[index] != NULL && base != index) {
		swap_nodes(&root->node_arr[base], &root->node_arr[index]);
		heapify(root, base);
	}
	return;
}

/* Removes the minimum node in the tree and returns it */
tree_node * remove_min(tree * root) {
	tree_node * ptr = root->node_arr[0];
	int i = 1;
	root->node_arr[0] = root->node_arr[root->size - i];
	--root->size;
	heapify(root, 0);
	return ptr;
}

/* Adds node to the minheap */
void insert_tree_node(tree * root, tree_node * node) {
	++root->size;
	int i = root->size - 1;
	while (i > 0 && root->node_arr[(i - 1)/2] != NULL && node->freq < root->node_arr[(i - 1)/2]->freq) {
		root->node_arr[i] = root->node_arr[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	root->node_arr[i] = node;
}

/* Converts heap into correctly sorted minheap */
void build_tree(tree * root) {
	int i = 0;
	int j = root->size - 1;
	for (i = (j - 1) / 2; i >= 0; --i) {
		heapify(root, i);
	}

}

/* Initializes tree, sets up tree nodes, and organizes tree into minheap */
tree * full_tree(unsigned int size, llist_node * node) {
	tree * root = create_tree(size);
	llist_node * ptr = node;
	int i = 0;
	for (i = 0; i < size; ++i) {
		root->node_arr[i] = create_node(ptr->token, ptr->freq);
		ptr = ptr->next;
		if (ptr == NULL) {
			break;
		}
	}
	root->size = size;
	build_tree(root);
	return root;
}

/* Algorithm to build Huffman tree from minheap */
tree_node * build_huffman(unsigned int size, llist_node * node) {
	tree_node * parent;
	tree_node * left;
	tree_node * right;
	tree * root = full_tree(size, node);
	while (root->size != 1) {
		left = remove_min(root);
		right = remove_min(root);
		int leftfreq = 0, rightfreq = 0;
		if (left != NULL) {
			leftfreq = left->freq;
		}
		if (right != NULL) {
			rightfreq = right->freq;
		}
		/* Enter unique token for nodes that are technically trees */
		char temp[3] = "/\\\0";
		parent = create_node(temp, leftfreq + rightfreq);
		parent->left = left;
		parent->right = right;
		insert_tree_node(root, parent);
	}
	return remove_min(root);
}

/* Sets specific numbers within array and writes them to file once leaf is reached */
void set_codes(tree_node * node, unsigned short code_arr[], int parent, int fd) {
	/* If going left, set current node to 0 and recurse for left node */
	if (node->left != NULL) {
		code_arr[parent] = 0;
		set_codes(node->left, code_arr, parent+1, fd);
	}
	/* If going right, set current index to 1 and recurse for right node */
	if (node->right != NULL) {
		code_arr[parent] = 1;
		set_codes(node->right, code_arr, parent+1, fd);
	}
	/* If reached a leaf, write the values stored previously in array up to parent */
	if (node->left == NULL && node->right == NULL) {
		int i = 0;
		char buf[parent];
		int count = 0;
		/* From 0 up to parent, write each number as char */
		for (i = 0; i < parent; ++i) {
			if (code_arr[i] == 1 || code_arr[i] == 0) {
				buf[i] = code_arr[i] + '0';
				++count;
			} else {
				break;
			}
		}
		/* Then tab, token, new line */
		write(fd, buf, count);
		write(fd, "\t", 1);
		write(fd, node->token, strlen(node->token));
		write(fd, "\n", 1);
	}
}

/* Goes through entire Huffman tree building process */
void huffman(int size, llist_node * root, int fd) {
	tree_node * base = build_huffman(size, root);
	short codes[size];
	set_codes(base, codes, 0, fd);
}
