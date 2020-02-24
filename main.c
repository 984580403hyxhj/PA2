#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "huffman.h"

void freelib(lib *head)
{
    if(head == NULL) return;

    lib *temp = head->next;
    while(1)
    {
        free(head->bit);
        free(head);
        head = temp;
        if(temp == NULL) return;
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
    if(argc != 6)
    {
        printf("wrong argc\n");
        return EXIT_FAILURE;
    }

	List *head = NULL;
	head = findlist(argv[1]);
    if(head == NULL) 
    {
        printf("exit fail\n");
        return EXIT_FAILURE;
    }


    int size = findlength(head);
	head = sortList(head);
	Node *topNode = buildtree(head);


	//printf("%d\n", topNode->weight);
	//print2D(topNode);

    //output count
    int total = countChar(argv[1], argv[2]);
    if(total == -1)
    {
        return EXIT_FAILURE;
    }


    //output tree
    treeoutput(topNode, argv[3], total);
    //output code
    lib *lib_head;
    lib_head = huffman_code(argv[4], topNode, size, total);
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
