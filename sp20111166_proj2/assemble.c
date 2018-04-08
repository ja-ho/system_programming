#include "20111166.h"

#define MAX_FILE_LENGTH 20
#define MAX_STATEMENT_LENGTH 100
#define MAX_STATEMENT_TOKEN 5
#define MAX_TOKEN_NAME 10
#define MAX_TEXT_RECORD_BYTE 60



/*---------------------*/
/*함수 : overall_assemble*/
/*목적 : 전체적인 assemble 작업을 한다.*/
/*리턴값: 에러가 없으면 0을 리턴*/
/*---------------------*/

int overall_assemble(char *fileName, List **op_table, List **sym_table)
{
	char *mid_file, *lst_file, *obj_file;
	unsigned int start, end;
	mid_file = (char *)malloc(sizeof(char)*MAX_FILE_LENGTH);
	lst_file = (char *)malloc(sizeof(char)*MAX_FILE_LENGTH);
	obj_file = (char *)malloc(sizeof(char)*MAX_FILE_LENGTH);
	
	strcpy(mid_file, fileName);
	strtok(mid_file, ".asm");
	sprintf(lst_file, "%s.lst", mid_file);
	sprintf(obj_file, "%s.obj", mid_file);
	
	if(!assemble_pass_one (mid_file, fileName, op_table, sym_table, &start, &end)) {
	} else {
		remove(mid_file);
		return -1;
	}
	if(!assemble_pass_two(mid_file, lst_file, obj_file, op_table, sym_table, start, end)) {
		remove(mid_file);					//if finished, remove mid_file.
	} else {
		remove(mid_file);					//if there is a error, then remove all the files
		remove(lst_file);
		remove(obj_file);
		return -1;
	}

	printf("\toutput file : [%s], [%s]\n", lst_file, obj_file);
	
	free(mid_file);
	free(lst_file);
	free(obj_file);
	return 0;
}

/*---------------------*/
/*함수 : read_statement*/
/*목적 : statement 한 줄을 읽는다.*/
/*리턴값: token_count*/
/*---------------------*/
int read_statement(char *buffer, List **op_table, char **token, bool *is_symbol, int pass)			//is_symbol parameter notice that first token is symbol
{
	char temp_buffer[MAX_STATEMENT_LENGTH];
	int token_count=0;
	opcode_format temp_format;
	strcpy(temp_buffer, buffer);
	token[token_count++] = strtok(temp_buffer, " ,\t\n");
	while(token[token_count-1]) {
		token[token_count++] = strtok(NULL, " ,\t\n");
	}
	//find opcode_table by first token. if can't find. it is symbol. 
	if(pass == 2) {
		if(find_opcode(op_table, token[2], &temp_format) == -1) {
			*is_symbol = true;
		} else {
			*is_symbol = false;
		}
	} else {
		if(find_opcode(op_table, token[0], &temp_format) == -1) { 	//it is symbol
			*is_symbol = true;
		} else { 										//it is opcode
			*is_symbol = false;
		}
	}
	return token_count-1;
}


/*---------------------*/
/*함수 : token_mapping*/
/*목적 : token의 상황에 따라 label, opcode 등을 연결해준다.*/
/*리턴값: 정상적이면 0리턴*/
/*---------------------*/
int token_mapping(char **token, char *label, char *opcode, char *operand_1, char *operand_2, int token_count, bool *is_symbol)
{	
	if(token_count == 4) {
		strcpy(label, token[0]);
		strcpy(opcode, token[1]);
		strcpy(operand_1, token[2]);
		strcpy(operand_2, token[3]);
	} else {
		if(*is_symbol) {					//first token is label
			if(!strcmp(token[0], "BASE")) {		//base exception
				label[0] = '\0';
				strcpy(opcode, token[0]);
				strcpy(operand_1, token[1]);
				operand_2[0] = '\0';
				*is_symbol = false;
			} else if (!strcmp(token[0], "END")) {		//end exception
				label[0] = '\0';
				strcpy(opcode, token[0]);
				strcpy(operand_1, token[1]);
				operand_2[0] = '\0';
				*is_symbol = false;
			} else {
				if(token[0][0] == '+') {			//it's exceptional situation. it's format4 without label
					*is_symbol = false;
					label[0] = '\0';
					strcpy(opcode, token[0]);
					operand_1[0] = '\0';
					operand_2[0] = '\0';
					if(token_count == 2) {
						strcpy(operand_1, token[1]);
					} else if (token_count == 3) {
						strcpy(operand_1, token[1]);
						strcpy(operand_2, token[2]);
					}
				} else {
					strcpy(label, token[0]);
					strcpy(opcode, token[1]);
					operand_1[0] = '\0';
					operand_2[0] = '\0';
					if(token_count == 3) {
						strcpy(operand_1, token[2]);
					}
				}
			}
		} else {						//first token is opcode
			strcpy(opcode, token[0]);
			label[0] = '\0';
			operand_1[0] = '\0';
			operand_2[0] = '\0';
			if(token_count == 2) {
				strcpy(operand_1, token[1]);
				operand_2[0] = '\0';
			} else if (token_count == 3) {
				strcpy(operand_1, token[1]);
				strcpy(operand_2, token[2]);
			}
		}
	}
	return 0;
}


/*---------------------*/
/*함수 : assemble_pass_one*/
/*목적 : first pass*/
/*리턴값: 정상 수행 시 0을 리턴*/
/*---------------------*/
int assemble_pass_one(char *mid_file, char *asm_file, List **op_table, List **sym_table, unsigned int *start, unsigned int *end)
{
	FILE *asm_fp, *mid_fp;
	char buffer[MAX_STATEMENT_LENGTH];
	char label[MAX_TOKEN_NAME], opcode[MAX_TOKEN_NAME], operand_1[MAX_TOKEN_NAME], operand_2[MAX_TOKEN_NAME];
	char *token[5];
	bool is_symbol, is_format_4;
	int line_num=5;
	int error_line=0;
	int hash, token_count, count;
	unsigned int LOCCTR = 0;
	unsigned int BEFORE_LOCCTR=0;
	opcode_format temp_format;
	asm_fp = fopen(asm_file, "r");
	mid_fp = fopen(mid_file, "w");
	
	if(asm_fp == NULL || mid_fp == NULL) {
		return handle_error(can_t_open_file);
	}

	//read first input line	
	while(fgets(buffer, MAX_STATEMENT_LENGTH, asm_fp) != NULL) {
		if(buffer[0] == '.') {	//it's comment
			continue;
		} 
		buffer[strlen(buffer)-1] = '\0';	
		token_count = read_statement(buffer, op_table, token, &is_symbol, 1);
		token_mapping(token, label, opcode, operand_1, operand_2, token_count, &is_symbol);
		if(!strcmp(opcode, "START")) {									//opcode is 'START' then initialize locctr
			LOCCTR = (unsigned int)strtol(operand_1, NULL, 16);
			fprintf(mid_fp, "%3d\t%04x\t%s\n", line_num, LOCCTR, buffer);
		} else {
			LOCCTR = 0;
		}	
		*start = LOCCTR;
		line_num+=5;
		break;
	}
	while(fgets(buffer, MAX_STATEMENT_LENGTH, asm_fp)) {			//read file
		if(buffer[0] !=  '.') {						//if this statement is not comment statement
			buffer[strlen(buffer)-1] = '\0';
			token_count = read_statement(buffer, op_table, token, &is_symbol, 1);
			token_mapping(token, label, opcode, operand_1, operand_2, token_count, &is_symbol);
			
			BEFORE_LOCCTR = LOCCTR;
			if(!strcmp(opcode, "END")) {
				*end = LOCCTR;
				break;
			}
			if(is_symbol) {								//there is a symbol in the statement
				if(find_symbol(sym_table, label) != -1) {
					error_line = line_num;
					fclose(asm_fp); fclose(mid_fp);
					printf("error_line is %d\n", error_line);
					fclose(asm_fp);
					fclose(mid_fp);
					return handle_error(exist_symbol);
				} else {
					hash = hash_function(label, 40);
					list_insert(sym_table[hash], label, BEFORE_LOCCTR, not_format);
				}
			}
			
			if(opcode[0] == '+') {				//detect format_4
				int i;
				is_format_4 = true;
				for(i=1; i<strlen(opcode); i++) {
					opcode[i-1] = opcode[i];
				}
				opcode[strlen(opcode)-1] = '\0';
			} else {
				is_format_4 = false;
			}

			if(find_opcode(op_table, opcode, &temp_format) != -1) {			//calculate LOCCTR
				if(temp_format == format_1) {
					LOCCTR += 1;
				} else if (temp_format == format_2) {
					LOCCTR += 2;
				} else if (temp_format == format_3_4) {
					if(is_format_4) {
						LOCCTR += 4;
					} else {
						LOCCTR += 3;
					}
				}
			} else if (!strcmp(opcode, "BASE")) {
					
			} else if (!strcmp(opcode, "WORD")) {		
				LOCCTR += 3;
			} else if (!strcmp(opcode, "RESW")) {
				count = strtol(operand_1, NULL, 10);
				LOCCTR += count * 3;			
			} else if (!strcmp(opcode, "RESB")) {
				count = strtol(operand_1, NULL, 10);
				LOCCTR += count;	
			} else if (!strcmp(opcode, "BYTE")) {
				count = strlen(operand_1);
				if(operand_1[1] != '\'' || operand_1[count-1] != '\'') {
					fclose(asm_fp);
					fclose(mid_fp);
					error_line = line_num;
					printf("error line is %d\n", error_line);
					return handle_error(wrong_statement);
				}
				if(operand_1[0] == 'X') {
					LOCCTR += (count-3) / 2;
				} else if (operand_1[0] == 'C') {
					LOCCTR += count-3;
				} else {
					fclose(asm_fp);
					fclose(mid_fp);
					error_line = line_num;
					printf("error line is %d\n", error_line);
					return handle_error(wrong_statement);
				}
			} else {
				fclose(asm_fp);
				fclose(mid_fp);

				error_line = line_num;
				printf("error line is %d\n", error_line);
				return handle_error(wrong_statement);
			}
		} else {
			line_num += 5;
			fprintf(mid_fp, "%3d\t\t%s", line_num, buffer);
			continue;
		}
		//printf("%3d\t%04x\t%s\n", line_num, BEFORE_LOCCTR, buffer);
		line_num += 5;
		fprintf(mid_fp, "%3d\t%04X\t%s\n", line_num, BEFORE_LOCCTR, buffer);			//make mid_file
	}
	fprintf(mid_fp, "%3d\t%04X\t%s\n", line_num, BEFORE_LOCCTR, buffer);
	fclose(asm_fp);
	fclose(mid_fp);
	return 0;
}

/*---------------------*/
/*함수 : assemble_pass_two*/
/*목적 : pass 2*/
/*리턴값: 에러가 없으면 0을 리턴*/
/*---------------------*/
int assemble_pass_two(char *mid_file, char *lst_file, char *obj_file, List **op_table, List **sym_table, unsigned int start, unsigned int end)
{
	FILE *mid_fp, *lst_fp, *obj_fp;
	char buffer[MAX_STATEMENT_LENGTH];
	char label[MAX_TOKEN_NAME], opcode[MAX_TOKEN_NAME], operand_1[MAX_TOKEN_NAME], operand_2[MAX_TOKEN_NAME];
	char *token[8];
	char *check; 			//to check, does strtol operate correctly
	bool is_symbol, is_format_4;
	int line_num;
	unsigned int LOCCTR, BEFORE_LOCCTR;
	unsigned int current_row_LOCCTR;
	unsigned int operand_address;
	unsigned int obj_code;
	unsigned int base;
	int i;
	int token_count, count, temp_count;
	bool index_mode, immediate_addressing, opcode_is_byte_x, opcode_is_res;
	char obj_text_record[100] = {0};
	char obj_text_header[10];
	char temp_buffer[10];
	unsigned int text_record_byte = 0;
	
	opcode_format temp_format;
	mid_fp = fopen(mid_file, "r");
	lst_fp = fopen(lst_file, "w");
	obj_fp = fopen(obj_file, "w");
	
	if(mid_fp == NULL || lst_fp == NULL || obj_fp == NULL) {
		return handle_error(can_t_open_file);
	}
	//read first input line	
	while(fgets(buffer, MAX_STATEMENT_LENGTH, mid_fp) != NULL) {
		if(buffer[5] == '.') {	//it's comment
			continue;
		} 
		buffer[strlen(buffer)-1] = '\0';
		token_count = read_statement(buffer, op_table, token, &is_symbol, 2);
		line_num = (int)strtol(token[0], NULL, 10);
		LOCCTR = (unsigned int)strtol(token[1], NULL, 16);
		BEFORE_LOCCTR = LOCCTR;
		if(token_count == 6) {									//parsing for pass 2
			strcpy(label, token[2]);
			strcpy(opcode, token[3]);
			strcpy(operand_1, token[4]);
			strcpy(operand_2, token[5]);
		} else {
			if(is_symbol) {					//first token is label
				if(!strcmp(token[2], "BASE")) {		//base exception
					label[0] = '\0';
					strcpy(opcode, token[2]);
					strcpy(operand_1, token[3]);
					operand_2[0] = '\0';
					is_symbol = false;
				} else if (!strcmp(token[2], "END")) {		//end exception
					label[0] = '\0';
					strcpy(opcode, token[2]);
					strcpy(operand_1, token[3]);
					operand_2[0] = '\0';
					is_symbol = false;
				} else {
					if(token[2][0] == '+') {			//it's exceptional situation. it's format4 without label
						is_symbol = false;
						label[0] = '\0';
						strcpy(opcode, token[2]);
						operand_1[0] = '\0';
						operand_2[0] = '\0';
						if(token_count == 4) {
							strcpy(operand_1, token[3]);
						} else if (token_count == 5) {
							strcpy(operand_1, token[3]);
							strcpy(operand_2, token[4]);
						}
					} else {
						strcpy(label, token[2]);		//symbol 존재, opcode 존재
						strcpy(opcode, token[3]);
						operand_1[0] = '\0';
						operand_2[0] = '\0';
						if(token_count == 5) {
							strcpy(operand_1, token[4]);
						}
					}
				}
			} else {						//first token is opcode
				strcpy(opcode, token[2]);
				label[0] = '\0';
				operand_1[0] = '\0';
				operand_2[0] = '\0';
				if(token_count == 4) {
					strcpy(operand_1, token[3]);
					operand_2[0] = '\0';
				} else if (token_count == 5) {
					strcpy(operand_1, token[3]);
					strcpy(operand_2, token[4]);
				}
			}	
		}	
		if(!strcmp(opcode, "START")) {									//opcode is 'START' then initialize locctr
			//printf("%s\n", buffer);
			fprintf(lst_fp, "%s\n", buffer);
		}
																		//write header record to object program
		fprintf(obj_fp, "H%-6s%06X%06X\n", label, start, end - start);															
		//printf("H%-6s%06X%06X\n", label, start, end-start);
		break;
	}
	while(fgets(buffer, MAX_STATEMENT_LENGTH, mid_fp)) {			//read file
		if(buffer[5] !=  '.') {						//if this statement is not comment statement
			buffer[strlen(buffer)-1] = '\0';
			token_count = read_statement(buffer, op_table, token, &is_symbol, 2);

			line_num = (int)strtol(token[0], NULL, 10);
			LOCCTR = (unsigned int)strtol(token[1], NULL, 16);
			BEFORE_LOCCTR = LOCCTR;
			if(token_count == 6) {
				strcpy(label, token[2]);
				strcpy(opcode, token[3]);
				strcpy(operand_1, token[4]);
				strcpy(operand_2, token[5]);
			} else {
				if(is_symbol) {					//first token is label
					if(!strcmp(token[2], "BASE")) {		//base exception
						label[0] = '\0';
						strcpy(opcode, token[2]);
						strcpy(operand_1, token[3]);
						operand_2[0] = '\0';
						is_symbol = false;
					} else if (!strcmp(token[2], "END")) {		//end exception
						label[0] = '\0';
						strcpy(opcode, token[2]);
						strcpy(operand_1, token[3]);
						operand_2[0] = '\0';
						is_symbol = false;
					} else {
						if(token[2][0] == '+') {			//it's exceptional situation. it's format4 without label
							is_symbol = false;
							label[0] = '\0';
							strcpy(opcode, token[2]);
							operand_1[0] = '\0';
							operand_2[0] = '\0';
							if(token_count == 4) {
								strcpy(operand_1, token[3]);
							} else if (token_count == 5) {
								strcpy(operand_1, token[3]);
								strcpy(operand_2, token[4]);
							}
						} else {
							strcpy(label, token[2]);
							strcpy(opcode, token[3]);
							operand_1[0] = '\0';
							operand_2[0] = '\0';
							if(token_count == 5) {
								strcpy(operand_1, token[4]);
							}
						}
					}
				} else {						//first token is opcode
					strcpy(opcode, token[2]);
					label[0] = '\0';
					operand_1[0] = '\0';
					operand_2[0] = '\0';
					if(token_count == 4) {
						strcpy(operand_1, token[3]);
						operand_2[0] = '\0';
					} else if (token_count == 5) {
						strcpy(operand_1, token[3]);
						strcpy(operand_2, token[4]);
					}
				}	
			}	
			is_format_4 = false;					//detect format4
			if(opcode[0] == '+') {
				is_format_4 = true;
				count = strlen(opcode);
				for(i=1; i<count; i++) {
					opcode[i-1] = opcode[i];
				}
				opcode[count-1] = '\0';
			}
			if(!strcmp(opcode, "END")) {			//if finished, write obj_file, lst_file's last line
				line_num += 5;
				//printf("%3d\t\t", line_num);
				//printf("       %s    %s\n", opcode, operand_1);
				sprintf(obj_text_header, "T%06X%02X", current_row_LOCCTR, text_record_byte);
				fprintf(obj_fp, "%s%s\n", obj_text_header, obj_text_record);
				//printf("%s%s\n", obj_text_header, obj_text_record);
				fprintf(obj_fp, "E%06X", start);
				fprintf(lst_fp, "%3d\t\t           %s    %s", line_num, opcode, operand_1);
				//printf("E%06X\n", start);

				break;
			} else if (!strcmp(opcode, "BASE")) {		//notice base line
				if(operand_1[0] == '#') {
					count = strtol(operand_1, NULL, 10);
					base = count;
				} else {
					operand_address = find_symbol(sym_table, operand_1);
					if(operand_address == -1) {
						printf("error_line is %d\n", line_num);
						fclose(mid_fp);
						fclose(lst_fp);
						fclose(obj_fp);
						return handle_error(wrong_statement);
					} else {
						base = operand_address;
					}
				}
				//printf("%3d\t\t", line_num);
				//printf("       %s   %s\n", opcode, operand_1);
				fprintf(lst_fp, "%s\n", buffer);
				continue;
			}
			obj_code = 0;						//initialize status
			opcode_is_byte_x = false;
			opcode_is_res = false;
			immediate_addressing = false;
			index_mode = false;

			if((obj_code = (unsigned int)find_opcode(op_table, opcode, &temp_format)) != -1) {		//initialize object code
				if(is_symbol) {								//there is a symbol in the statement
					if((operand_address = (unsigned int)find_symbol(sym_table, label) != -1)) {
					} else {
						operand_address = 0;
						printf("can't find op_code\n");
						printf("error_line: %d\n", line_num);
						fclose(mid_fp);
						fclose(lst_fp);
						fclose(obj_fp);
						return handle_error(wrong_statement);
					}
				} else {
					operand_address= 0;
				}
				if(temp_format == format_1) {					//format 1
					LOCCTR+=1;
				} else if (temp_format == format_2) {			//format 2
					obj_code <<= 4;
					LOCCTR+=2;
					if(strlen(operand_1) != 0) {
						if(!strcmp(operand_1, "A")) {
							obj_code += 0;
						} else if (!strcmp(operand_1, "X")) {
							obj_code += 1;
						} else if (!strcmp(operand_1, "L")) {
							obj_code += 2;
						} else if (!strcmp(operand_1, "PC")) {
							obj_code += 8;
						} else if (!strcmp(operand_1, "SW")) {
							obj_code += 9;
						} else if (!strcmp(operand_1, "B")) {
							obj_code += 3;
						} else if (!strcmp(operand_1, "S")) {
							obj_code += 4;
						} else if (!strcmp(operand_1, "T")) {
							obj_code += 5;
						} else if (!strcmp(operand_1, "F")) {
							obj_code += 6;
						} else {
							printf("format2_front is error\n");
							printf("error_line is %d\n", line_num);
							fclose(mid_fp);
							fclose(lst_fp);
							fclose(obj_fp);
							return handle_error(wrong_statement);
						}
					}
					obj_code <<= 4;
					if(strlen(operand_2) != 0) {
						if(!strcmp(operand_2, "A")) {
							obj_code += 0;
						} else if (!strcmp(operand_2, "X")) {
							obj_code += 1;
						} else if (!strcmp(operand_2, "L")) {
							obj_code += 2;
						} else if (!strcmp(operand_2, "PC")) {
							obj_code += 8;
						} else if (!strcmp(operand_2, "SW")) {
							obj_code += 9;
						} else if (!strcmp(operand_2, "B")) {
							obj_code += 3;
						} else if (!strcmp(operand_2, "S")) {
							obj_code += 4;
						} else if (!strcmp(operand_2, "T")) {
							obj_code += 5;
						} else if (!strcmp(operand_2, "F")) {
							obj_code += 6;
						} else {
							printf("format2 is error\n");
							printf("error_line is %d\n", line_num);
							fclose(mid_fp);
							fclose(lst_fp);
							fclose(obj_fp);
							return handle_error(wrong_statement);
						}
					}
				} else if (temp_format == format_3_4) {
					if(strlen(operand_2) != 0) {
						if(operand_2[0] == 'X') {
							index_mode = true;
						} else {
							printf("operand_2 is not null\n");
							printf("error_line is %d\n", line_num);
							fclose(mid_fp);
							fclose(lst_fp);
							fclose(obj_fp);
							return handle_error(wrong_statement);
						}
					}
					if(operand_1[0] == '#') {
						immediate_addressing = true;		// it is immediate_addressing -> n=0, i=1
						obj_code += 1;						// n=0, i=1
						count = strlen(operand_1);
						for(i=1; i<count; i++) {
							operand_1[i-1] = operand_1[i];
						}
						operand_1[count-1] = '\0';
					} else if (operand_1[0] == '@') {
															// it is indirect_addressing -> n=1, i=0
						obj_code += 2;
						count = strlen(operand_1);
						for(i=1; i<count; i++) {
							operand_1[i-1] = operand_1[i];
						}
						operand_1[count-1] = '\0';
					} else {
						obj_code += 3;						// it is simple_addressing -> n=1, i=1
					}
					obj_code <<= 4;
					if(!is_format_4) {			//format3
						LOCCTR+=3;
						if(strlen(operand_1) == 0) {			//operand isn't exist
							obj_code <<= 12;
						} else {
							if((count = (int)find_symbol(sym_table, operand_1)) == -1) {			//it is constant
								count = (int)strtol(operand_1, &check, 16);
								if(*check != '\0') {
									printf("error_line is %d\n", line_num);
									printf("operand is weird...\n");
									printf("it's not number!!!\n");
									fclose(mid_fp);
									fclose(lst_fp);
									fclose(obj_fp);
									return handle_error(wrong_statement);
								}
								if(immediate_addressing) {
									obj_code <<= 12;
									obj_code += count;									
								} else if (count >= -(1 << 11) && count < (1 << 11)) {		//pc relative
									if(LOCCTR > count) {
										count = count - LOCCTR;
										count &= 0x000FFF;
									} else {
										count = count - LOCCTR;
									}
									if(index_mode) {
										obj_code += 10;										//x=1, b=0, p=1, e=0
									} else {
										obj_code += 2;										//x=0, b=0, p=1, e=0
									}
									obj_code <<= 12;
									obj_code += count;
								} else if (count >= 0 && count < (1 << 12)) {				//base relative
									if(index_mode) {										//x=1
										obj_code += 12;										//x=1, b=1, p=0, e=0
									} else {
										obj_code += 4;										//x=0, b=1, p=0, e=0
									}
									count -= base; 
									obj_code <<= 12;
									obj_code += count;
								} else {
									printf("error_line is %d\n", line_num);
									printf("out of range. use intruction format 4\n");
									fclose(mid_fp);
									fclose(lst_fp);
									fclose(obj_fp);
									return handle_error(wrong_statement);
								}
							} else {														//it is symbol
								int disp = count - LOCCTR;
								int base_disp = count - base;
								if((disp >= -(1 << 11)) && (disp < (1 << 11))) {				//pc relative
																							//b=0, p=1, e=0
									if(LOCCTR > count) {
										temp_count = count - LOCCTR;
										temp_count &= 0X000FFF;	
									} else {
										temp_count = count - LOCCTR;
									}
									if(index_mode) {
										obj_code += 10;										//x=1, b=0, p=1, e=0
									} else {
										obj_code += 2;										//x=0, b=0, p=1, e=0
									}	
									obj_code <<= 12;
									obj_code += temp_count;
								} else if (base_disp >= 0 && base_disp < (1 << 12)) {		//base relative 
									if(index_mode) {										//x=1
										obj_code += 12;										//x=1, b=1, p=0, e=0
									} else {
										obj_code += 4;										//x=0, b=1, p=0, e=0
									}
									temp_count = count - base;	
									obj_code <<= 12;
									obj_code += temp_count;
								} else {
									printf("error_line is %d\n", line_num);
									printf("out of range. use intruction format 4\n");
									fclose(mid_fp);
									fclose(lst_fp);
									fclose(obj_fp);
									return handle_error(wrong_statement);
								}
							}
						} 
					} else {																//format 4
						LOCCTR+=4;
						if(index_mode) {
							obj_code += 9;
						} else {
							obj_code += 1;
						}
						obj_code <<= 20;
						if(operand_1 == NULL) {			//operand isn't exist
						} else {
							if((count = (int)find_symbol(sym_table, operand_1)) == -1) {			//it is constant
								count = (int)strtol(operand_1, &check, 10);
								if(*check != '\0') {
									printf("error_line is %d\n", line_num);
									printf("operand is weird...\n");
									printf("it's not number!!!\n");
									fclose(mid_fp);
									fclose(lst_fp);
									fclose(obj_fp);
									return handle_error(wrong_statement);
								}
								if(count > (1 << 20)) {
									printf("error_line is %d\n", line_num);
									printf("out of memory bound\n");
									fclose(mid_fp);
									fclose(lst_fp);
									fclose(obj_fp);
									return handle_error(wrong_statement);
								}
								obj_code += count;
							} else {
								if(count > (1 << 20)) {
									printf("error_line is %d\n", line_num);
									printf("out of memory bound\n");
									fclose(mid_fp);
									fclose(lst_fp);
									fclose(obj_fp);
									return handle_error(wrong_statement);
								}
								obj_code += count;
							}
						}
					}
				} 
			} else {	
				if(!strcmp(opcode, "RESB") || !strcmp(opcode, "RESW")) {
					opcode_is_res = true;
				} else if (!strcmp(opcode ,"BYTE") || !strcmp(opcode, "WORD")) {
					if(!strcmp(opcode, "WORD")) {
						count = strtol(operand_1, &check, 16);
						if(*check != '\0') {
							printf("error_line is %d\n", line_num);
							printf("operand is not constant\n");
							fclose(mid_fp);
							fclose(lst_fp);
							fclose(obj_fp);
							return handle_error(wrong_statement);
						}

						obj_code = count;
					} else if (!strcmp(opcode, "BYTE")) {
						count = strlen(operand_1);
						if(operand_1[0] == 'X') {
							opcode_is_byte_x = true;				//if opcode_is_byte_x == true, then just print operand_1
							for(i=2; i<count-1; i++) {
								operand_1[i-2] = operand_1[i];
							}
							operand_1[count-3] = '\0';
						} else if (operand_1[0] == 'C') {
							for(i=2; i<count-1; i++) {
								operand_1[i-2] = operand_1[i];
							}
							operand_1[count-3] = '\0';
							count = strlen(operand_1);
							obj_code = 0;
							obj_code = (unsigned int)operand_1[0];
							
							for(i=1; i<count; i++) {
								obj_code <<= 8;
								obj_code += operand_1[i];
							}
						}
					} else {
						printf("error_line is %d\n", line_num);
						fclose(mid_fp);
						fclose(lst_fp);
						fclose(obj_fp);
						return handle_error(wrong_statement);
					}
				}
			}
		} else {
			//printf("%s", buffer);
			fprintf(lst_fp, "%s", buffer);
			continue;
		}


		//translate obj_code into char array
		if(opcode_is_res) {
		} else if(opcode_is_byte_x) {
			strcpy(temp_buffer, operand_1);
			//strcat(obj_text_record, operand_1);
		} else {
			if(temp_format == format_1 || temp_format == format_2) {
				sprintf(temp_buffer, "%X", obj_code);
			} else if (!is_format_4) {
				sprintf(temp_buffer, "%06X", obj_code);
			} else {
				sprintf(temp_buffer, "%08X", obj_code);
			}
		}

		if(opcode_is_res) {													//if opcode is res, change text-record line
			if(text_record_byte != 0) {
				sprintf(obj_text_header, "T%06X%02X", current_row_LOCCTR, text_record_byte);
				fprintf(obj_fp, "%s%s\n", obj_text_header, obj_text_record);
				//printf("%s%s\n", obj_text_header, obj_text_record);
				text_record_byte = 0;
				obj_text_header[0] = '\0';
				obj_text_record[0] = '\0';
				temp_buffer[0] = '\0';
			}
		} else if((text_record_byte + strlen(temp_buffer)/2) > 30) {		//if text_record_byte is more than maximum record_byte(1E), then change line
			sprintf(obj_text_header, "T%06X%02X", current_row_LOCCTR, text_record_byte);
			fprintf(obj_fp, "%s%s\n", obj_text_header, obj_text_record);
			//printf("%s%s\n", obj_text_header, obj_text_record);
			text_record_byte = 0;
			obj_text_header[0] = '\0';
			obj_text_record[0] = '\0';
			text_record_byte += strlen(temp_buffer)/2;
			strcat(obj_text_record, temp_buffer);
			current_row_LOCCTR = BEFORE_LOCCTR;
		} else {															//strcat..ing
			if(text_record_byte == 0) {
				current_row_LOCCTR = BEFORE_LOCCTR;
			}
			text_record_byte += strlen(temp_buffer)/2;
			strcat(obj_text_record, temp_buffer);
		}
		if(opcode_is_res) {															//make lst file
			//printf("%s\n", buffer);
			fprintf(lst_fp, "%s\n", buffer);
		} else if(opcode_is_byte_x) {
			//printf("%-32s\t\t%s\n", buffer, operand_1);
			fprintf(lst_fp, "%-32s\t\t%s\n", buffer, operand_1);
		} else {
			//printf("%-32s\t\t%X\n", buffer, obj_code);
			if(!is_format_4) {
				if(temp_format == format_1 || temp_format == format_2) {
					fprintf(lst_fp, "%-32s\t\t%X\n", buffer, obj_code);
				} else {
					fprintf(lst_fp, "%-32s\t\t%06X\n", buffer, obj_code);
				}
			} else {
				fprintf(lst_fp, "%-32s\t\t%08X\n", buffer, obj_code);
			}
		}
	}
	fclose(mid_fp); fclose(lst_fp); fclose(obj_fp);	
	return 0;
}

