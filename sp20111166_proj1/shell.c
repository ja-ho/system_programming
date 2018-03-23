#include "20111166.h"

int tokenizer(char *str, char **token)
{
	int i=0;
	const char *delim = ",\t";
	printf("first strtok\n");
	token[i++] = strtok(str, delim);
	while(token != NULL) {
		token[i++] = strtok(NULL, delim);
	}
	printf("finish strtok\n");
	return i-1;	//number of token
}


/*
int shell()
{
	unsigned char memory[16][65536];
	char command_str[20];
	char token1[8];
	char token2[8];
	char token3[8];
	while(1) {
		printf("sicsim> ");
		fgets(command_str, sizeof(command_str), stdin);

	}
}
*/
