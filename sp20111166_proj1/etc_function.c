#include "20111166.h"


//print all files in current directory
int dir(void)	// TODO: check error
{
	DIR *dir = NULL;
	struct stat buf;
	struct dirent *dp = NULL;

	dir = opendir(".");
	if(dir) {
		while((dp = readdir(dir))) {
			lstat(dp->d_name, &buf);
			if( S_ISDIR(buf.st_mode)) {
				printf("%s\\  ", dp->d_name);
			} else if (S_IXUSR & buf.st_mode) {
				printf("%s*  ", dp->d_name);
			} else {
				printf("%s  ", dp->d_name);
			}
		}
		closedir(dir);
		printf("\n");
	} else {
		printf("can not open directory\n");
	}
	return 0;
}

int find_opcode(List **opTable, char *mnemonic) 
{
	Node *temp;
	int hash;
	hash = hash_function(mnemonic);
	temp = opTable[hash]->head;
	while(temp != NULL) {
		if(!strcmp(temp->content, mnemonic)) {
			return temp->op_code;
		}
		temp = temp->next;
	}
	return -1;	//can't find
}

//error handling
//
int handle_error(error_num num) {
	switch(num) {
	case out_of_boundary : 						   
		printf("out of memory boundary\n");
		break;
	

	default : 
		break;
	}
	return -1;
}
