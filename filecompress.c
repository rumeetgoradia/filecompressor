#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "filecompress.h"

void print_list(llist_node * root) {
	llist_node * ptr = root;
	printf("printing list\n");
	for (ptr = root; ptr != NULL; ptr = ptr->next) {
		printf("%s : %d\n", ptr->token, ptr->freq);
	}
}

tree_node * create_node(char * token, unsigned int freq) {
	printf("enter create_node\n");
	tree_node * ptr = (tree_node *)malloc(sizeof(tree_node));
	ptr->left = NULL;
	ptr->right = NULL;
	printf("create nodes left and right\n");
	ptr->token = (char *)malloc((sizeof(char) * strlen(token)) + 1);
	strcpy(ptr->token, token);
	printf("got past token\n");
	ptr->freq = freq;
	return ptr;
}

tree * create_tree(unsigned int max_size) {
	tree * ptr = (tree *)malloc(sizeof(tree));
	ptr->size = 0;
	ptr->max_size = max_size;
	ptr->node_arr = (tree_node **)malloc(max_size * sizeof(tree_node *));
	return ptr;
}

void swap_nodes(tree_node ** first, tree_node ** second) {
	tree_node * temp = *first;
	*first = *second;
	*second = temp;
}

void heapify(tree * root, int index) {
//	printf("start heap\n");
	int base = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;
//	printf("heapify compares\n");
	if (left < root->size && root->node_arr[left] != NULL && root->node_arr[base] != NULL && root->node_arr[left]->freq < root->node_arr[base]->freq) {
		base = left;
	}
	if (right < root->size && root->node_arr[right] != NULL && root->node_arr[base] != NULL && root->node_arr[right]->freq < root->node_arr[base]->freq) {
		base = right;
	}
//	printf("base: %d, index: %d\n", base, index);
	if (root->node_arr[base] != NULL && root->node_arr[index] != NULL && base != index) {
//		printf("replacing\n");
		swap_nodes(&root->node_arr[base], &root->node_arr[index]);
		heapify(root, base);
	}
//	printf("past if statement\n");
	return;
}

tree_node * remove_min(tree * root) {
	tree_node * ptr = root->node_arr[0];
//	printf("got node_arr[0]\n");
	int i = 1;
	root->node_arr[0] = root->node_arr[root->size - i];		
//	printf("got the other final\n");
	root->size = root->size - 1;
//	printf("remove_min heapify starting\n");
	heapify(root, 0);
	return ptr;
}

void insert_tree_node(tree * root, tree_node * node) {
	++root->size;
	int i = root->size - 1;
	while (i > 0 && (node->freq < root->node_arr[(i - 1)/2]->freq)) {
		root->node_arr[i] = root->node_arr[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	root->node_arr[i] = node;
}

void build_tree(tree * root) {
	int i = 0;
//	printf("size: %d\n", root->size);
	int j = root->size - 1;
	for (i = (j - 1) / 2; i >= 0; --i) {
		heapify(root, i);
//		printf("i: %d\n", i);	
	}
//	printf("done build-tree\n");
}

tree * full_tree(unsigned int size, llist_node * node) {
	tree * root = create_tree(size);
	printf("got past create_tree\n");
	llist_node * ptr = node;
	int i = 0;
	for (i = 0; i < size; ++i) {
		root->node_arr[i] = create_node(ptr->token, ptr->freq);
		printf("got past create_node\n");
		ptr = ptr->next;
		if (ptr == NULL) {
			break;
		}
	}
	printf("got past for loop in create_tree\n");
	root->size = size;
	build_tree(root);
	printf("got past build tree\n");
	return root;
}

tree_node * build_huffman(unsigned int size, llist_node * node) {
	tree_node * parent;
	tree_node * left;
	tree_node * right;
	tree * root = full_tree(size, node);
	printf("got past full_tree\n");
	while (root->size != 1) {
		printf("enter while loop\n");
		left = remove_min(root);
		right = remove_min(root);
		printf("right left done\n");
		char temp[6] = "/////";		
		if (left != NULL && right != NULL) {
			parent = create_node(temp, left->freq + right->freq);
			printf("parent created\n");
			parent->left = left;
			parent->right = right;
			printf("parent done\n");
			insert_tree_node(root, parent);
			printf("parent inserted\n");
		}
	}
	printf("got past build_huffman\n");
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
		int i = 0;
		for (i = 0; i < parent; ++i) {
			printf("%d", code_arr[i]);
		}
		printf("\n");
	}
}

void huffman(unsigned int size, llist_node * root) {
	tree_node * base = build_huffman(size, root);
	printf("got past building huffman\n");
	int codes[size];
	set_codes(base, codes, 0);
}

