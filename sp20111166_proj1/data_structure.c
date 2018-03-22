#include "20111166.h"

void copy_str(char *from, char *to)
{
	int i;
	int length = strlen(from);
	to = (char *)malloc(sizeof(char)+1);
	
	for(i=0; i<length; i++) {
		strncpy(to, from, length);
	}
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

	copy_str(str, temp->content);
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
	Node *temp;
	unsigned int op_code;
	char mnemonic[8];
	char format[5];
	int i;
	fp = fopen("opcode.txt", "r");
	
	if(fp == NULL) {
		printf("file can not open\n");
		return;
	}
	
	hashTable = (List **)malloc(sizeof(List*)*20);
	for(i=0; i<20; i++) {
		hashTable[i] = (List *)malloc(sizeof(List));
		hashTable[i]->head = NULL;
		hashTable[i]->tail = NULL;
	}

	while(fscanf(fp, "%x %s %s", &op_code, mnemonic, format) != EOF) {
		int hash = hash_function(mnemonic);
		list_insert(hashTable[hash], mnemonic, op_code);
	}

	fclose(fp);
	return;
}
