#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

/*정의되는 상수*/
#define MAX_TOKEN 6
#define MAX_STR 256
#define MEM_ROW 16
#define MEM_COL 65536

/*정의되는 구조체*/


typedef enum {
	not_format,
	format_1,
	format_2,
	format_3_4,
} opcode_format;


typedef struct _Node {
	int value;
	char *name;
	opcode_format format;
	struct _Node *next;
	struct _Node *prev;
} Node;


typedef struct _List {
	Node *head;
	Node *tail;
} List;
/////////linked-list

typedef enum {
	out_of_boundary,	//memory boundary 
	wrong_input,		//wrong input
	not_found,			//not_found_mnemonic
	not_number,			//need char input
	not_allocate,		//memory allocation fail
	not_found_file,		//not_found_file
	can_t_open_file,	//can't open file
	exist_symbol,		//symbol is already exist
	wrong_statement,	//wrong statement
} error_num;
///////error enum type





/*함수 원형*/
void list_init(List *list);
void init_table(List **op_table, List **sym_table);
int list_insert(List *list, char *str, int value, opcode_format format);
int hash_function(char *s, int hash_number);
void make_hashTable(List **hashTable);
void print_list(List *list);
void print_op_table(List **op_table);
void print_sym_table(List **sym_table);
int sym_comp_func(const void *a, const void *b);
void free_list(List *list);
void free_hashTable(List **hashTable, int length);
void table_initialize(List **hash_table, int length);
///////////data_structure



int tokenizer(char *str, char **token);
int count_token_with_comma(char *str);
int command(unsigned char memory[][MEM_ROW], List *history_list, List **op_table, List **sym_table, char **token, int token_number, char *history_temp);
/////////////shell


void copy_str(char *from, char **to);
int dir(void);
int type_file(char *fileName);
int find_opcode(List **opTable, char *mnemonic, opcode_format *format);
int find_symbol(List **sym_table, char *label);
int handle_error(error_num num);
//////etc_function



int memory_dump(unsigned char memory[][MEM_ROW], int *address);
int memory_start_dump(unsigned char memory[][MEM_ROW], int start, int *address);
int memory_range_dump(unsigned char memory[][MEM_ROW], int start, int end, int *address);
int memory_edit(unsigned char memory[][MEM_ROW], int address, int value);
int memory_fill(unsigned char memory[][MEM_ROW], int start, int end, int value);
void memory_reset(unsigned char memory[][MEM_ROW]);
///memory_management


int overall_assemble(char *fileName, List **op_table, List **sym_table);
int read_statement(char *buffer, List **op_table, char **token, bool *is_symbol, int pass);
int token_mapping(char **token, char *label, char *opcode, char *operand_1, char *operand_2, int token_count, bool *is_symbol);
int token_mapping_2(char **token, int *line_num, unsigned int *address, char *label, char *opcode, char *operand_1, char *operand_2, int token_count, bool *is_symbol);
int assemble_pass_one(char *mid_file, char *asm_file, List **op_table, List **sym_table, unsigned int *start, unsigned int *end);
int assemble_pass_two(char *mid_file, char *lst_file, char *obj_file, List **op_table, List **sym_table, unsigned int start, unsigned int end);


//assemble

