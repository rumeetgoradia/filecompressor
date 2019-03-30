#include<stdlib.h>
#include<stdio.h>
#include "filecompress.h"

int main() {
	llist_node * head = (llist_node *)malloc(sizeof(llist_node));
	head = NULL;
	int size = 0;
	insert_list(head, "a", 5);
	insert_list(head, "b", 9);
	insert_list(head, "c", 12);
	insert_list(head, "d", 13);
	insert_list(head, "e", 16);
	insert_list(head, "f", 45);
	size = 6;
	print_list(head);
	printf("about to huff\n");
	huffman(size, head);
	return EXIT_SUCCESS;
}
