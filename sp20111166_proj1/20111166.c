#include "20111166.h"

int main() {
	List *history_list;
	List *op_table[20];
	
	unsigned char memory[MEM_COL][MEM_ROW];	//1MB memory
	char command_str[MAX_STR];
	char *token[MAX_TOKEN];	//number of token is 4
	
	int token_number;

	history_list = (List *)malloc(sizeof(List));
	list_init(history_list);
	make_hashTable(op_table);
	
	while(1) {
		printf("sicsim> ");
		fgets(command_str, sizeof(command_str), stdin);
		command_str[strlen(command_str)-1] = '\0';
		token_number = tokenizer(command_str, token);
		if(token_number == 0) continue;
		if(token_number > 4) continue;
		if(!command(memory, history_list, op_table, token, token_number)) {
			list_insert(history_list, command_str, 0);
		}
	}
			

	return 0;
}
