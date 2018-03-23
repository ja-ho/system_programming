#include "20111166.h"

int tokenizer(char *str, char **token)
{
	int i=0;
	const char *delim = " ,\t\n";
	token[i++] = strtok(str, delim);
	while(token[i-1] != NULL) {
		if(i >= 6) break; 
		token[i++] = strtok(NULL, delim);
	}
	if(token[i-2] && token[i-2][strlen(token[i-2])-1] == '\n') {		//remove end of token '\n'
		token[i-2][strlen(token[i-2])-1] = '\0';		//need to check
	}

	return (i-1);	//number of token
}



int command(unsigned char memory[][MEM_COL], List *history_list, List **op_table, char **token, int token_number)
{
	//distinguish command by token number
	if(token_number == 1) {
		if(!strcmp(token[0], "h") || !strcmp(token[0], "help")) {
			printf("h[elp]\n");
			printf("d[ir]\n");
			printf("q[uit]\n");
			printf("hi[story]\n");
			printf("du[mp] [start, end]\n");
			printf("e[dit] address, value\n");
			printf("f[ill] start, end, value\n");
			printf("reset\n");
			printf("opcode mnemonic\n");
			printf("opcodelist\n");
		} else if (!strcmp(token[0], "d") || !strcmp(token[0], "dir")) {
		
		} else if (!strcmp(token[0], "q") || !strcmp(token[0], "quit")) {
			//before exit, free all assigned memory
			free_list(history_list);
			free_hashTable(op_table);
			exit(0);	
		} else if (!strcmp(token[0], "hi") || !strcmp(token[0], "history")) {
			print_list(history_list);		
		} else if (!strcmp(token[0], "reset")) {
		
		} else if (!strcmp(token[0], "opcodelist")) {
			print_op_table(op_table);
		} 
	} else if (token_number == 2) {
	
	} else if (token_number == 3) {
	
	} else if (token_number == 4) {
		
	}

	return 0;
}

