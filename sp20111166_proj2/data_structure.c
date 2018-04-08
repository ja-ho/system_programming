#include "20111166.h"

#define SYM_SIZE 40
#define OP_SIZE 20




/*---------------------*/
/*함수 : list_init*/
/*목적 : list를 초기화한다*/
/*리턴값: 없음*/
/*---------------------*/
void list_init(List *list)
{
	list->head = NULL;
	list->tail = NULL;
}



/*---------------------*/
/*함수 : list_insert*/
/*목적 : list에 노드를 추가한다*/
/*리턴값: 0 = 정상, -1: error */
/*---------------------*/
int list_insert(List *list, char *str, int value, opcode_format format)
{
	Node *temp = (Node *)malloc(sizeof(Node));
	
	if(temp == NULL) {
		return handle_error(not_allocate);
	}

	copy_str(str, &temp->name);
	temp->value = value;
	temp->next = NULL;
	temp->prev = NULL;
	temp->format = format;
	if(list->head == NULL) {
		list->head = temp;
		list->tail = temp;
	} else {
		list->tail->next = temp;
		temp->prev = list->tail;
		list->tail = temp;
	}
	return 0;
}

/*---------------------*/
/*함수 : print_list*/
/*목적 : list를 출력한다*/
/*리턴값: 없음*/
/*---------------------*/
void print_list(List *list)
{
	Node *temp;
	int num;
	temp = list->head;
	num = 1;

	if(temp == NULL) return;
	
	while(temp->next!=NULL) {
		printf("%d %s\n", num++, temp->name);
		temp = temp->next;
	}
	printf("%d %s\n", num++, temp->name);	//print tail
	return;
}


/*---------------------*/
/*함수 : hash_function*/
/*목적 : hash값을 생성*/
/*리턴값: hash*/
/*---------------------*/
int hash_function(char *s, int hash_number)
{
	int i;
	const int g = 31;						//hash constant.
	int hash=0;						//hash index
	int length = strlen(s);
	for(i=0; i < length; i++) {
		hash = g * hash + s[i];		//add each char digit. and multiply by g. so it doensn't allow any permutation overlap
		hash = hash%hash_number;
	}
	return hash;
}


/*---------------------*/
/*함수 : init_Table*/
/*목적 : hashtable을 초기화한다*/
/*리턴값: 없*/
/*----------음-----------*/
void init_table(List **op_table, List **sym_table)
{
	int i;
	for(i=0; i<OP_SIZE; i++) {
		op_table[i] = (List *)malloc(sizeof(List));
		list_init(op_table[i]);
	}
	for(i=0; i<SYM_SIZE; i++) {
		sym_table[i] = (List *)malloc(sizeof(List));
		list_init(sym_table[i]);
	}
	return;
}

/*---------------------*/
/*함수 : make_hashTable*/
/*목적 : opcode table을 구성한다*/
/*리턴값: 없*/
/*----------음-----------*/
void make_hashTable(List **hashTable)
{
	FILE *fp;
	unsigned int op_code;
	char mnemonic[8];
	char format[5];
	fp = fopen("opcode.txt", "r");
	
	if(fp == NULL) {
		printf("file can not open\n");
		return;
	}
	while(fscanf(fp, "%x %s %s", &op_code, mnemonic, format) != EOF) {
		int hash = hash_function(mnemonic, 20);				//insert to the hashtable by hash_index which is from hash_function
		if(!strcmp(format, "1")) {
			list_insert(hashTable[hash], mnemonic, op_code, format_1);		
		} else if (!strcmp(format, "2")) {
			list_insert(hashTable[hash], mnemonic, op_code, format_2);
		} else if (!strcmp(format, "3/4")) {
			list_insert(hashTable[hash], mnemonic, op_code, format_3_4);
		}
	}
	
	fclose(fp);
	return;
}

/*---------------------*/
/*함수 : print_op_table*/
/*목적 : optable을 출력한다*/
/*리턴값: 없음*/
/*---------------------*/
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
			printf("%d : [%s, %02X] ->", i, temp->name, temp->value);
		} else {					//case2: no next node
			printf("%d : [%s, %02X]\n", i, temp->name, temp->value);
			continue;
		}
		temp = temp->next;
		while(temp->next != NULL) {
			printf(" [%s, %02X] ->", temp->name, temp->value);
			temp = temp->next;
		}
		if(op_table[i]->head != NULL) {	//print last node
			printf(" [%s, %02X]\n", temp->name, temp->value);
		}
	}
	return;
}

/*---------------------*/
/*함수 : print_sym_table*/
/*목적 : symtable을 내림차순으로 출력한다*/
/*리턴값: 없음*/
/*---------------------*/
void print_sym_table(List **sym_table)
{
	int i, idx = 0;
	Node *all_temp[100] = {0};
	Node *temp;

	for(i=0; i<40; i++) {
		for(temp = sym_table[i]->head; temp != NULL; temp = temp->next) {
			all_temp[idx] = (Node *)malloc(sizeof(Node));
			all_temp[idx]->value = temp->value;
			all_temp[idx]->name = (char *)malloc(sizeof(char)*10);
			strcpy(all_temp[idx]->name, temp->name);
			all_temp[idx]->format = temp->format;
			all_temp[idx]->next = NULL; all_temp[idx++]->prev = NULL;
		}
	}
	if(idx == 0) {
		printf("there is no symbol in the symbol table\n");
		return;
	}

	qsort(all_temp, idx, sizeof(Node*), sym_comp_func);

	for(i=0; i<idx-1; i++) {
		printf("\t%s\t%04X\n", all_temp[i]->name, all_temp[i]->value);	
	}
	printf("\t%s\t%04X", all_temp[i]->name, all_temp[i]->value);
	
	for(i=0; i<idx; i++) {
		free(all_temp[i]->name);
		free(all_temp[i]);
	}
	return;
}
/*---------------------*/
/*함수 : sym_comp_func*/
/*목적 : sym_table을 qsort하기 위한 compare 함*/
/*리턴값: strcmp 결과값*/
/*---------------------*/
int sym_comp_func(const void *a, const void *b)
{
	Node *tempA = *(Node **)a;
	Node *tempB = *(Node **)b;
	return strcmp(tempA->name, tempB->name);
}



/*---------------------*/
/*함수 :free_list*/
/*목적 : list 할당된 메모리를 해제한다*/
/*리턴값: 없음*/
/*---------------------*/
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
		free(temp->name);
		list->head = list->head->next;
		free(temp);
	}
	free(list);
	return;
}


/*---------------------*/
/*함수 : free_hashTable*/
/*목적 : hashtable 할당된 메모리를 해제한다.*/
/*리턴값: 없음*/
/*---------------------*/
void free_hashTable(List **hashTable, int length)
{
	int i;
	for(i=0; i<length; i++) {
		free_list(hashTable[i]);
	}
	return;
}
/*---------------------*/
/*함수 : table_initialize*/
/*목적 : hash_table을 length크기만큼 초기화한다.*/
/*리턴값: 없음*/
/*---------------------*/
void table_initialize(List **hash_table, int length)
{
	int i;
	Node *temp, *free_node;
	for(i=0; i<length; i++) {
		temp = hash_table[i]->head;
		while(temp != NULL) {
			free_node = temp;
			temp = temp->next;
			free(free_node);
		}
		list_init(hash_table[i]);
	}
}


