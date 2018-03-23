#include "20111166.h"

int main() {
	List *history_list;
	List *op_table[20];
	
	char command_str[50];
	char *token[max_token];	//number of token is 4
	
	int token_number;
	int i;

	history_list = (List *)malloc(sizeof(List));
	list_init(history_list);
	make_hashTable(op_table);
	
	while(1) {
		printf("sicsim> ");
		fgets(command_str, sizeof(command_str), stdin);
		printf("command_str: %s\n"< command_str);
		token_number = tokenizer(command_str, token);
		printf("token_number : %d\n", token_number);
		for(i=0; i<token_number; i++) {
			printf("%s\n", token[i]);
		}
	}
		

	return 0;
}
