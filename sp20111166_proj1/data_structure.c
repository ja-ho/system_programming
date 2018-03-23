#include "20111166.h"

void copy_str(char *from, char **to)
{
	int i;
	int length = strlen(from);
	*to = (char *)malloc(sizeof(char)*length+1);
	strncpy(*to, from, length);
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
	
	while(temp->next!=NULL) {
		printf("%d	%s", num++, temp->content);
		printf("\n");
		temp = temp->next;
	}
	printf("%d %s\n", num++, temp->content);	//print tail
}


int hash_function(char *s)
{
	int i;
	int g = 31;
	int hash=0;
	int length = strlen(s);
	for(i=0; i < length; i++) {
		hash = g * hash + s[i];
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
		list_insert(hashTable[hash], mnemonic, op_code);
	}
	
	fclose(fp);
	return;
}

void print_opTable(List **op_table)	//need to fix
{
	int i;
	Node *temp;
	for(i=0; i<20; i++) {
		temp = op_table[i]->head;
		if(temp == NULL) {
			continue;
		}
		while(temp->next != NULL) {
			printf("%x %s %d\n", temp->op_code, temp->content, i);
			temp = temp->next;
		}
		if(op_table[i]->head != NULL) {
			printf("%x %s\n", temp->op_code, temp->content);
		}
	}
}
