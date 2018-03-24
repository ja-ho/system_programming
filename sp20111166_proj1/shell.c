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





int command(unsigned char memory[][MEM_ROW], List *history_list, List **op_table, char **token, int token_number)
{
	static int address = 0;					//internal address pointer
	int start, end, value;					//change into hex numbers
	char *check1, *check2, *check3;			//check whether change is ok
	int status=0;
	//distinguish command by token number
	if(token_number == 1) {					//help, dir, quit, history, dump, reset, opcodelist
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
			dir();
		} else if (!strcmp(token[0], "q") || !strcmp(token[0], "quit")) {
			//before exit, free all assigned memory
			free_list(history_list);
			free_hashTable(op_table);
			exit(0);	
		} else if (!strcmp(token[0], "hi") || !strcmp(token[0], "history")) {
			print_list(history_list);		
		} else if (!strcmp(token[0], "du") || !strcmp(token[0], "dump")) {
			status = memory_dump(memory, &address);
		} else if (!strcmp(token[0], "reset")) {
			memory_reset(memory);	
		} else if (!strcmp(token[0], "opcodelist")) {
			print_op_table(op_table);
		} 
	} else if (token_number == 2) {			  //opcode mnemonic, dump start 	//TODO: need to check upper case
		int op_code;
		if(!strcmp(token[0], "opcode")) {
			op_code = find_opcode(op_table, token[1]);
			printf("opcode is %d\n", op_code);
		} else if(!strcmp(token[0], "du") || !strcmp(token[0], "dump")) {
			start = (int)strtol(token[1], &check1, 16);
			if(*check1 == '\0') {
				status = memory_start_dump(memory, start, &address);
			}
		}
	} else if (token_number == 3) {			  //(dump start, end), (edit address, value)
		if(!strcmp(token[0], "du") || !strcmp(token[0], "dump")) {
			start = (int)strtol(token[1], &check1, 16);
			end = (int)strtol(token[2], &check2, 16);
			if(*check1 == '\0' && *check2 == '\0') {
				status = memory_range_dump(memory, start, end, &address);
			}
		} else if(!strcmp(token[1], "e") || !strcmp(token[1], "edit")) {
			value = (int)strtol(token[2], &check1, 16);
			if(*check1 == '\0') {
				status = memory_edit(memory, address, value);
			}
		}
	} else if (token_number == 4) {				//fill start, end, value
		if(!strcmp(token[0], "f") || !strcmp(token[0], "fill")) {
			start = (int)strtol(token[1], &check1, 16);
			end = (int)strtol(token[2], &check2, 16);
			value = (int)strtol(token[3], &check3, 16);
			if(*check1 == '\0' && *check2 == '\0' && *check3 == '\0') {
				status = memory_fill(memory, start, end, value);
			}
		}
		
	}

	return status;
}

