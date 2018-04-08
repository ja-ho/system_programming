#include "20111166.h"

/*프로그램 시작*/
int main() {
	/*변수*/
	List *history_list;
	List *op_table[20];
	List *sym_table[40];
	unsigned char memory[MEM_COL][MEM_ROW];	//1MB memory
	char command_str[MAX_STR];
	char *token[MAX_TOKEN];	//max number of tokens is 4
	char *temp, *comp_temp;
	int token_number, comma_token_number;

	history_list = (List *)malloc(sizeof(List));
	list_init(history_list);
	init_table(op_table, sym_table);
	make_hashTable(op_table);
	
	while(1) {
		printf("sicsim> ");
		fgets(command_str, sizeof(command_str), stdin);
		command_str[strlen(command_str)-1] = '\0';
		copy_str(command_str, &temp);
		copy_str(command_str, &comp_temp);
		token_number = tokenizer(temp, token);		//tokenize the command and return the number of tokens
		comma_token_number = count_token_with_comma(comp_temp);		//check that input is right or not(about comma)
		

		if(token_number != comma_token_number) {					//check if num of comma is not same
			handle_error(wrong_input);
			continue;
		}
		if(token_number == 0 || token_number >4) {							//check if num of token is too small or too much
			handle_error(wrong_input);
			continue;
		}

		if(!command(memory, history_list, op_table, sym_table, token, token_number, command_str)) {		//process the command
			list_insert(history_list, command_str, 0, not_format);		//if there's no error, put the last command into the history list
		}
	}
			

	return 0;
}
