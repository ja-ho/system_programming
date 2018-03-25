#include "20111166.h"

/*---------------------*/
/*함수 : tokenizer*/
/*목적 : str를 나눠 token에 저장한다*/
/*리턴값: token의 갯수*/
/*---------------------*/
int tokenizer(char *str, char **token)
{
	int i=0;
	char *delim = " ,\t\n";				//first delimeter.  tokenize except , \t, \n.
	token[i++] = strtok(str, delim);			//save the token into the token[i]
	while(token[i-1] != NULL) {
		if(i >= 5) return 5; 
		token[i++] = strtok(NULL, delim);
	}
	if(token[i-2] && token[i-2][strlen(token[i-2])-1] == '\n') {		//remove end of token '\n'
		token[i-2][strlen(token[i-2])-1] = '\0';	
	}
	return (i-1);	//return number of token
}

/*---------------------*/
/*함수 : count_token_with_comma*/
/*목적 : comma에 따라 토큰의 갯수를 센다*/
/*리턴값: 토큰의 갯수*/
/*---------------------*/
int count_token_with_comma(char *str)			//check that input is right or not about comma
{
	int i=0;
	char *token;
	strsep(&str, " ,\t\n");
	i++;
	while ((token = strsep(&str, ",")) != NULL) {
		i++;
	}
	
	return i;
}

/*---------------------*/
/*함수 : command*/
/*목적 : 저장된 토큰에 따라 명령을 수행한다*/
/*리턴값: 0 = 정상, -1 = error*/
/*---------------------*/
int command(unsigned char memory[][MEM_ROW], List *history_list, List **op_table, char **token, int token_number, char *history_temp)	//history_temp : to handle history. if input is "hi" or "history",
{																																		//input the command into the history list before print it.
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
			list_insert(history_list, history_temp, 0);
			status = -1;				//to not write history again in the history list
			print_list(history_list);		
		} else if (!strcmp(token[0], "du") || !strcmp(token[0], "dump")) {
			status = memory_dump(memory, &address);
		} else if (!strcmp(token[0], "reset")) {
			memory_reset(memory);	
		} else if (!strcmp(token[0], "opcodelist")) {
			print_op_table(op_table);
		} else {
			status = handle_error(wrong_input);
		}
	} else if (token_number == 2) {			  //opcode mnemonic, dump start 	//TODo: need to check upper case
		int op_code;
		if(!strcmp(token[0], "opcode")) {
			op_code = find_opcode(op_table, token[1]);
			if(op_code == -1) {
				status = handle_error(not_found);
			} else {
				printf("opcode is %02X\n", op_code);
			}
		} else if(!strcmp(token[0], "du") || !strcmp(token[0], "dump")) {
			start = (int)strtol(token[1], &check1, 16);
			if(*check1 == '\0') {
				status = memory_start_dump(memory, start, &address);
			} else {
				status = handle_error(not_number);
			}
		} else {
			status = handle_error(wrong_input);
		}
	} else if (token_number == 3) {			  //(dump start, end), (edit address, value)
		if(!strcmp(token[0], "du") || !strcmp(token[0], "dump")) {
			start = (int)strtol(token[1], &check1, 16);
			end = (int)strtol(token[2], &check2, 16);
			if(*check1 == '\0' && *check2 == '\0') {
				status = memory_range_dump(memory, start, end, &address);
			} else {
				status = handle_error(not_number);
			}
		} else if(!strcmp(token[0], "e") || !strcmp(token[0], "edit")) {
			address = (int)strtol(token[1], &check1, 16);
			value = (int)strtol(token[2], &check2, 16);
			if(*check1 == '\0' && *check2 == '\0') {
				status = memory_edit(memory, address, value);
			} else {
				status = handle_error(not_number);
			}
		} else {
			status = handle_error(wrong_input);
			
		}
	} else if (token_number == 4) {				//fill start, end, value
		if(!strcmp(token[0], "f") || !strcmp(token[0], "fill")) {
			start = (int)strtol(token[1], &check1, 16);
			end = (int)strtol(token[2], &check2, 16);
			value = (int)strtol(token[3], &check3, 16);
			if(*check1 == '\0' && *check2 == '\0' && *check3 == '\0') {
				status = memory_fill(memory, start, end, value);
			} else {
				status = handle_error(not_number);
			}
		} else {
			status = handle_error(wrong_input);
		}	
	}
	return status;
}

