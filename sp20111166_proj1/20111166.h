#include <stdin.h>
#include <stdlib.h>
#include <string.h>


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


void copy_str(char *from, char *to);
void list_init(List *list);
int list_insert(List *list, char *str, int op_code);
int hash_function(char *s);
void make_hashTable(List *hashTable);
