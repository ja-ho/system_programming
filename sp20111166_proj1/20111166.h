#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

/*정의되는 상수*/
#define MAX_TOKEN 6
#define MAX_STR 256
#define MEM_ROW 16
#define MEM_COL 65536

/*정의되는 구조체*/
typedef struct _Node {
	int op_code;
	char *content;
	struct _Node *next;
} Node;


typedef struct _List {
	Node *head;
	Node *tail;
} List;
/////////linked-list

typedef enum {
	out_of_boundary,
	wrong_input,
	not_found,
	not_number,
	not_allocate,
} error_num;
///////error enum type

/*함수 원형*/
void list_init(List *list);
int list_insert(List *list, char *str, int op_code);
int hash_function(char *s);
void make_hashTable(List **hashTable);
void print_list(List *list);
void print_op_table(List **hashTable);
void free_list(List *list);
void free_hashTable(List **hashTable);
///////////data_structure



int tokenizer(char *str, char **token);
int count_token_with_comma(char *str);
int command(unsigned char memory[][MEM_ROW], List *history_list, List **op_table, char **token, int token_number, char *history_temp);
/////////////shell


void copy_str(char *from, char **to);
int dir(void);
int find_opcode(List **opTable, char *mnemonic);
int handle_error(error_num num);
//////etc_function



int memory_dump(unsigned char memory[][MEM_ROW], int *address);
int memory_start_dump(unsigned char memory[][MEM_ROW], int start, int *address);
int memory_range_dump(unsigned char memory[][MEM_ROW], int start, int end, int *address);
int memory_edit(unsigned char memory[][MEM_ROW], int address, int value);
int memory_fill(unsigned char memory[][MEM_ROW], int start, int end, int value);
void memory_reset(unsigned char memory[][MEM_ROW]);
///memory_management
