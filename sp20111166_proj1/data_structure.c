#include "20111166.h"

void copy_str(char *from, char **to)
{
	int length = strlen(from);
	*to = (char *)malloc(sizeof(char)*length+1);
	strncpy(*to, from, length+1);
}


void list_init(List *list)
{
	list->head = NULL;
	list->tail = NULL;
}



int list_insert(List *list, char *str, int op_code)
{
	Node *temp = (Node *)malloc(sizeof(Node));
	
	if(temp == NULL) {
		printf("memory allocation fail\n");
		return 0;
	}

	copy_str(str, &temp->content);
	temp->op_code = op_code;
	temp->next = NULL;

	if(list->head == NULL) {
		list->head = temp;
		list->tail = temp;
	} else {
		list->tail->next = temp;
		list->tail = temp;
	}
	return 0;
}

void print_list(List *list)
{
	Node *temp;
	int num;
	temp = list->head;
	num = 1;

	if(temp == NULL) return;
	
	while(temp->next!=NULL) {
		printf("%d %s\n", num++, temp->content);
		temp = temp->next;
	}
	printf("%d %s\n", num++, temp->content);	//print tail
	return;
}


int hash_function(char *s)
{
	int i;
	int g = 31;						//hash constant.
	int hash=0;						//hash index
	int length = strlen(s);
	for(i=0; i < length; i++) {
		hash = g * hash + s[i];		//add each char digit. and multiply by g. so it doensn't allow any permutation overlap
		hash = hash%20;
	}
	return hash;
}

void make_hashTable(List **hashTable)
{
	FILE *fp;
	unsigned int op_code;
	char mnemonic[8];
	char format[5];
	int i;
	fp = fopen("opcode.txt", "r");
	
	if(fp == NULL) {
		printf("file can not open\n");
		return;
	}
	for(i=0; i<20; i++) {
		hashTable[i] = (List *)malloc(sizeof(List));
		list_init(hashTable[i]);
	}
	while(fscanf(fp, "%x %s %s", &op_code, mnemonic, format) != EOF) {
		int hash = hash_function(mnemonic);
		list_insert(hashTable[hash], mnemonic, op_code);			//insert to the hashtable by hash_index which is from hash_function
	}
	
	fclose(fp);
	return;
}

void print_op_table(List **op_table)	//print table
{
	int i;
	Node *temp;
	for(i=0; i<20; i++) {
		temp = op_table[i]->head;
		if(temp == NULL) {		//nothing in the list
			printf("%d : empty\n", i);
			continue;
		}
		if(temp->next != NULL) {	//case1: theres' next node
			printf("%d : [%s, %02X] ->", i, temp->content, temp->op_code);
		} else {					//case2: no next node
			printf("%d : [%s, %02X]\n", i, temp->content, temp->op_code);
			continue;
		}
		temp = temp->next;
		while(temp->next != NULL) {
			printf(" [%s, %02X] ->", temp->content, temp->op_code);
			temp = temp->next;
		}
		if(op_table[i]->head != NULL) {	//print last node
			printf(" [%s, %02X]\n", temp->content, temp->op_code);
		}
	}
}

void free_list(List *list)	//free list
{
	if(list == NULL) return;
	if(list->head == NULL) {	//there's nothing in the list
		free(list);
		return;
	}
	Node *temp;
	temp = list->head;
	while(temp->next != NULL) {		//free content, node
		temp = list->head;
		free(temp->content);
		list->head = list->head->next;
		free(temp);
	}
	free(list);
	return;
}


void free_hashTable(List **hashTable)
{
	int i;
	for(i=0; i<20; i++) {
		free_list(hashTable[i]);
	}
	return;
}
