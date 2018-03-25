#include "20111166.h"


//print all files in current directory
int dir(void)	// TODO: check error
{
	DIR *dir = NULL;
	struct stat buf;
	struct dirent *dp = NULL;

	dir = opendir(".");					//open current directory stream
	if(dir) {
		while((dp = readdir(dir))) {	//point to the next file
			lstat(dp->d_name, &buf);
			if( S_ISDIR(buf.st_mode)) {				//is it directory?
				printf("%s\\  ", dp->d_name);
			} else if (S_IXUSR & buf.st_mode) {		//or is it executable file?
				printf("%s*  ", dp->d_name);
			} else {								//else
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
	hash = hash_function(mnemonic);					//find where the mnemonic is saved in the hashTable
	temp = opTable[hash]->head;
	while(temp != NULL) {
		if(!strcmp(temp->content, mnemonic)) {
			return temp->op_code;					//return the opcode of the mnemonic
		}
		temp = temp->next;
	}
	return -1;	//can't find
}

//error handling
//
int handle_error(error_num num) {					//error handling by num which is defined enum error type
	switch(num) {
	case out_of_boundary : 						   
		printf("out of memory boundary\n");
		break;
	case wrong_input :
		printf("wrong input. please try again\n");
		break;
	case not_found :
		printf("can't find mnemonic. try again\n");
		break;
	case not_number :
		printf("you have to input number arguments. try again\n");
		break;
	default : 
		printf("something wrong. try again\n");
		break;
	}
	return -1;										//return -1, so do not put the command into the history list
}
