#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

typedef struct _Node{
	unsigned char value;
	int weight;
	struct _Node* left;
	struct _Node* right;
}Node;

//list with treenodes
typedef struct _List {

	Node *tree;
	struct _List *next;

} List;

typedef struct _lib{
	unsigned char value;
	int length;
	int *bit;
	struct _lib *next;

} lib;

void treeoutput(Node *head, char *filename);

void countChar(char * filename1, char *filename2);

int findlength(List *head);

List *findlist(char *filename);

List *sortList(List *head);

Node * buildtree(List *head);

lib * huffman_code(char *filename, Node *head, int size);

void huffman_compress(char *f_input, char *f_output, char *treeinput, lib *head);

#endif
