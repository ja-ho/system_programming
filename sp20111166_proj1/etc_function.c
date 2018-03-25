#include "20111166.h"


/*---------------------*/
/*함수 : copy_str*/
/*목적 : str을 카피한다*/
/*리턴값: 없음*/
/*---------------------*/
void copy_str(char *from, char **to)
{
	int length = strlen(from);
	*to = (char *)malloc(sizeof(char)*length+1);
	strncpy(*to, from, length+1);
}


/*---------------------*/
/*함수 : dir*/
/*목적 : 현재 디렉토리의 모든 파일을 ㅊ ㅜㄹ력한다*/
/*리턴값: 0 = 정상, -1 = error*/
/*---------------------*/
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
		return -1;
	}
	return 0;
}


/*---------------------*/
/*함수 : find_opcode*/
/*목적 : mnemonic에 맞는 opcode를 찾는다*/
/*리턴값: opcode, -1 = error*/
/*---------------------*/
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

//
/*---------------------*/
/*함수 : handle_error*/
/*목적 : error에 맞는 메세지를 출력한다*/
/*리턴값: -1 = error*/
/*---------------------*/
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
	case not_allocate :
		printf("memory allocation fail\n");
		break;
	default : 
		printf("something wrong. try again\n");
		break;
	}
	return -1;										//return -1, so do not put the command into the history list
}
