#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "huffman.h"


void print2DUtil(Node *root, int space) 
{ 
    // Base case 
    int COUNT = 10;
    if (root == NULL) 
        return; 
  
    // Increase distance between levels 
    space += COUNT; 
  
    // Process right child first 
    print2DUtil(root->right, space); 
  
    // Print current node after space 
    // count 
    printf("\n"); 
    for (int i = COUNT; i < space; i++) 
        printf(" "); 
    printf("|%c|,%d\n", root->value, root->weight); 
  
    // Process left child 
    print2DUtil(root->left, space); 
} 
  
// Wrapper over print2DUtil() 
void print2D(Node *root) 
{ 
   // Pass initial space count as 0 
   print2DUtil(root, 0); 
}

int main(int argc, char** argv)
{
	List *head = NULL;
	head = findlist(argv[1]);
    int size = findlength(head);
	head = sortList(head);
	Node *topNode = buildtree(head);


	//printf("%d\n", topNode->weight);
	//print2D(topNode);

    //output count
    countChar(argv[1], argv[2]);

    //output tree
    treeoutput(topNode, argv[3]);
    //output code
    lib *lib_head;
    lib_head = huffman_code(argv[4], topNode, size);
    /*lib *libtemp = lib_head; ///////print lib
    while(libtemp != NULL)
    {
        //printf("ssssss\n");
        printf("\n%c. %d \n", libtemp->value, libtemp->length);
        for(int i = 0; i <(libtemp->length); i++)
        {
            printf("%d", libtemp->bit[i]);
        }
        libtemp = libtemp->next;
    }
    //printf("\n");*/
    huffman_compress(argv[1], argv[5], argv[3], lib_head);

	

	return EXIT_SUCCESS;
}
