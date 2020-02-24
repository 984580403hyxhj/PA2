#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "huffman.h"


////////////////////////////
void countChar(char * filename1, char *filename2)
{
	long ascii = 0;
	
	//long arr = malloc(sizeof(long) * 256);
	long arr[256] = {0};
	FILE *fp = fopen(filename1,"r");

	if(fp == NULL)
	{
		printf("fopen fails at countChar");
		return;
	}

	while(ascii != EOF)
	{
		ascii = fgetc(fp);
		arr[ascii]++;
	}
	fclose(fp);

	fp = fopen(filename2,"wb+");
	for(int indx = 0; indx < 256; indx++)
	{
		fwrite(&arr[indx],sizeof(long),1,fp);
	}
	fclose(fp);

	return;

}
///////////////////above output count, all works/////////////



/////////////////////////////
List *buildlist(unsigned char value, int weight) //put weight into list, create tree node with char value
{
	List *p = malloc(sizeof(List));
	p->tree = malloc(sizeof(Node));
	p->tree->weight = weight;
	p->tree->value = value;
	p->tree->left = NULL;
	p->tree->right = NULL;
	p->next = NULL;

	return p; 
}

List *insertlist(List *org, unsigned char value, int weight)
{
	List *p= buildlist(value,weight);
	p->next = org;

	return p;
}

List *findlist(char *filename) //find char and its weight
{
	FILE *fp = fopen(filename, "r");

	List *head = NULL;
	List *temp_list;
	int temp = 0;
	bool ex_check = false;
	while(temp != EOF)
	{
		ex_check = false;
		temp = fgetc(fp);
		//if(temp == ' ') temp = '!'; ///////debug
		if(head == NULL)
		{
			head = insertlist(head, temp,0);
			
		}
		//if head != NULL
		temp_list = head;
		while(temp_list != NULL)
		{
			if(temp_list->tree->value == temp)
			{
				temp_list->tree->weight++;
				ex_check = true;
			}
			if(temp_list->next == NULL && ex_check == false && temp!=EOF)
			{
				head = insertlist(head, temp, 0);
				head->tree->weight++;
			}

			temp_list = temp_list->next;
		}
	}
	fclose(fp);
	return head;
}

void swaptree(List *a, List *b)
{
	Node *temp = a->tree;
	a->tree = b->tree;
	b->tree = temp;
}

List *sortList(List *head)
{
	//int temp = 0;
	List *temp1 = head;
	List *temp2 = head->next;
	while(temp1 != NULL)//first sort weight
	{
		temp2 = temp1->next;
		while(temp2 != NULL)
		{
			if(temp2->tree->weight < temp1->tree->weight)
			{
				swaptree(temp2,temp1);
			}


			temp2 = temp2->next;
		}
		temp1=temp1->next;
	}

	//sort by ascii
	temp1 = head;
	temp2 = head->next;
	while(temp1 != NULL)
	{
		temp2 = temp1->next;
		while(temp2 != NULL)
		{	
			
			if((temp2->tree->value < temp1->tree->value) && (temp1->tree->weight == temp2->tree->weight))
			{
				swaptree(temp2,temp1);
			}


			temp2 = temp2->next;
		}
		temp1=temp1->next;
		//if(temp1 == NULL) printf("early break 2!!!\n");
	}

	return head;
}

int findlength(List *head)
{
	int count = 0;
	List *temp = head;
	while(temp != NULL)
	{
		count++;
		temp = temp->next;
	}
	return count;
}

/////////////////////////////above makes the sorted frequency list////////////////////////////////
//all above is working


/////////////////////////////below makes the huffman tree out of the frequency list///////////////
List *merge(List *a, List *b)
{
	Node *temp1 = a->tree;
	Node *temp2 = b->tree;
	Node *newNode = malloc(sizeof(Node));

	newNode->left = temp1;
	newNode->right = temp2;
	newNode->value = 0;
	newNode->weight = temp1->weight + temp2->weight;

	b->tree = newNode;

	free(a);
	return b;
}

List *moveNode(List *head)
{
	if(head->next == NULL) return head;

	List *temp = head;
	List *finalHead = head->next;
	while(temp->next != NULL && !(temp->next->tree->weight > head->tree->weight))
	{
		temp = temp->next;
	}
	
	//temp->next = head;
	head->next = temp->next;
	temp->next = head;

	return finalHead;
}


Node * buildtree(List *head)
{
	while(head->next != NULL)
	{
		head = merge(head, head->next);
		//head = sortList(head); this is too slow
		head = moveNode(head);
	}

	Node *temp = head->tree;

	free(head);
	return temp;
}

/////////////////////////////above builds huffman tree//////////////////////////
//above all works

////////////////////////////below outputs huffman tree/////////////////////////
void preorder(Node* head, FILE *fp)
{
	if(!head->left && !head->right)
	{
		fprintf(fp, "1%c",head->value);
		return;
	}
	fprintf(fp,"0");
	preorder(head->left,fp);
	preorder(head->right,fp);

}

void treeoutput(Node *head, char *filename)
{
	FILE *fp = fopen(filename,"w+");

	preorder(head,fp);
	fclose(fp);
}



//////////////////////////////below builds huffman code///////////////////////

lib *buildlib(unsigned char value, int length, int* bit) //put weight into list, create tree node with char value
{
	lib *p = malloc(sizeof(lib));
	p->value = value;
	p->length = length;
	p->bit = malloc(sizeof(int) * length);
	p->next = NULL;
	for(int i = 0; i < length; i++)
	{
		//sprintf(&(p->bit)[i], "%d", bit[i]);
		p->bit[i]=bit[i];
	}

	return p; 
}

lib *insertlib(lib *org, unsigned char value, int length, int* bit)
{
	lib *p= buildlib(value,length,bit);
	p->next = org;

	return p;
}


void write_tree(FILE *fp, Node *head, int *arr, int index, lib **lib_head)
{
	if(head->left == NULL && head->right == NULL)
	{
		fprintf(fp, "%c:", head->value);
		*lib_head = insertlib(*lib_head, head->value, index, arr);
		for(int i = 0; i < index; i++)
		{
			fprintf(fp,"%d", arr[i]);
		}
		fprintf(fp,"\n");
		return;
	}

	if(head->left)
	{
		arr[index] = 0;
		write_tree(fp,head->left,arr,index+1, lib_head);
	}

	if(head->right)
	{
		arr[index] = 1;
		write_tree(fp,head->right,arr,index+1, lib_head);
	}

}

lib *huffman_code(char *filename, Node *head, int size)
{
	FILE *fp = fopen(filename,"w+");
	int *arr = malloc(sizeof(int) * (size+1));
	//lib **lib_head = NULL;
	lib **lib_head = malloc(sizeof(int*));
	*lib_head = NULL;
	//*lib_head = malloc(sizeof(int*));
	//lib_head = &(*lirb_head);
	write_tree(fp, head, arr, 0, lib_head);
	lib *temp = *lib_head;
	free(lib_head);
	free(arr);

	fclose(fp);
	return temp;

}

//////////////////////above outputs huffman codes///////////
//above all works//

//////////////////////below outputs compressed file///////

lib *findlib(lib *head, unsigned char read)
{
	lib *temp = head;
	while(temp->value != read)
	{
		temp = temp->next;
	}
	return temp;
}

void huffman_compress(char *f_input, char *f_output, char *treeinput, lib *head)
{
	FILE* input = fopen(f_input, "r");
	FILE* output = fopen(f_output, "wb+");

	

/////////output total char//////

////////////////////////

	///find total count of char in code to final total bytes 
	lib *temp = head;
	long count = 0;
	while(temp!=NULL)
	{
		count++;
		temp = temp->next;
	}
	
	//printf("\n\n%ld\n",count);
	fwrite(&count,sizeof(long),1,output);


	///output total num of char///
	fseek(input,0,SEEK_END);
	long size = ftell(input);
	size = size/sizeof(char);
	printf("size : %ld\n", size);
	fseek(input,0,SEEK_SET);
	
	/////////////////

////////////////below output total number of char in tree, total char in input file//////////////
	FILE *fp_tree = fopen(treeinput, "r");
	fseek(fp_tree,0,SEEK_END);
	long totalchar_tree = ftell(fp_tree);
	totalchar_tree = totalchar_tree / sizeof(char);
	totalchar_tree = totalchar_tree - count;
	totalchar_tree = totalchar_tree/8 + (totalchar_tree%8 != 0) + count;
	fwrite(&totalchar_tree,sizeof(long),1,output);
	printf("totalchar_tree: %ld\n", totalchar_tree);
	fwrite(&size,sizeof(long),1,output);
	fseek(fp_tree,0,SEEK_SET);


//////////////below output tree////////////

	char a = fgetc(fp_tree);
	unsigned char read = a;
	unsigned char write = 0;
	int curlength = 1;
	unsigned char mask0 = 0;
	unsigned char mask1 = 1;
	while(a != EOF)
	{
		//printf("|%c|, ",a);
		printf("|%c|\n",read);
		if(a == '0')
		{
			//printf("22222\n");
			write = write | (mask0 << (curlength-1));
			curlength++;
			if(curlength > 8)
			{
				fwrite(&write,sizeof(char),1,output);
				curlength = 1;
				write = 0;
			}
		}
		else if(a == '1')
		{
			//printf("33333\n");
			write = write | (mask1 << (curlength-1));
			curlength++;
			if(curlength > 8)
			{
				fwrite(&write,sizeof(char),1,output);
				curlength = 1;
				write = 0;
			}
			a = fgetc(fp_tree);
			printf("|%c|\n", a);
			read = a;
			for(int k = 0; k< 8; k++)
			{
				write = write | (((read >> k) & mask1) << (curlength-1));
				curlength++;
				if(curlength > 8)
				{
					fwrite(&write,sizeof(char),1,output);
					curlength = 1;
					write = 0;
				}
			}
		}
		a = fgetc(fp_tree);
		read = a;
		if(a == EOF) break;
	}
	if(curlength != 1) fwrite(&write,sizeof(char),1,output);
	fclose(fp_tree);


	///////////output compressed file////////
	

	write = 0;
	read = 0; //read from file
	curlength = 1;
	temp = NULL;
	int j = 0;
	int totalchar = 0;
	for(int i = 0; i < size; i++)
	{
		read = fgetc(input);
		//printf("\n|%c|\n", read);
		temp = findlib(head,read);
		//printf("%d, %d\n", j, temp->length );
		for(; j < temp->length; j++)
		{
			//printf("%d",temp->bit[j] );
			//write |= (mask & ((temp->bit)[j] << (8-curlength)));
			write = write | (temp->bit[j]<<(curlength-1));
			//printf("\nwrite = %d\n", write);
			curlength++;
			if(curlength > 8)
			{
				//printf("\nwrite, output: %d\n", write);
				fwrite(&write,sizeof(char),1,output);
				totalchar++;
				curlength = 1;
				write = 0;
			}
		}
		j = 0;
	}
	if(curlength != 1) fwrite(&write,sizeof(char),1,output);
	//printf("\n%d\n", totalchar);
	fclose(input);
	fclose(output);
}


























