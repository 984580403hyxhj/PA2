#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "huffman.h"

void freelib(lib *head)
{
    lib *temp = head->next;
    while(1)
    {
        free(head->bit);
        free(head);
        head = temp;
        temp=temp->next;
        if(temp == NULL)
        {
            free(head->bit);
            free(head);
            break;
        } 
    }
}

void freetree(Node *head)
{
    if(head == NULL) return;

    freetree(head->left);
    freetree(head->right);

    free(head);
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

	freelib(lib_head);
    freetree(topNode);

	return EXIT_SUCCESS;
}
