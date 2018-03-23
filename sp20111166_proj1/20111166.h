#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_token 5


typedef struct _Node {
	int op_code;
	char *content;
	struct _Node *next;	//check Node *pNext
} Node;


typedef struct _List {
	Node *head;
	Node *tail;
} List;
/////////linked-list


void copy_str(char *from, char **to);
void list_init(List *list);
int list_insert(List *list, char *str, int op_code);
int hash_function(char *s);
void make_hashTable(List **hashTable);
void print_list(List *list);
void print_hashTable(List **hashTable);
///////////data_structure



int command(List *history_list, List *hash_table);
int tokenizer();
int shell();

