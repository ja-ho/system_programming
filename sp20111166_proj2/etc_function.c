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
/*함수 : type_fileName*/
/*목적 : 인자로 전달된 파일 이름을 찾아 화면에 파일 내용을 출력한다.*/
/*리턴값: 0 = 정상, -1 = error*/
/*---------------------*/

int type_file(char *fileName)
{
	DIR *dir = NULL;
	FILE *fp;
	struct dirent *dp = NULL;
	char ch;
	dir = opendir(".");					//open current directory stream
	
	if(dir) {
		while((dp = readdir(dir))) {	//point to the next file
			if(!strcmp(dp->d_name, fileName)) {	//find file
				fp = fopen(fileName, "r");
				if(fp != NULL) {					//print file
					while((ch=fgetc(fp)) != EOF) {
						putchar(ch);	
					}
				} else {
					closedir(dir);
					printf("can't open the file\n");
					return -1;
				}
				closedir(dir);
				printf("\n");
				return 0;
			}
		}
		closedir(dir);
		printf("\n");
		return handle_error(not_found_file);
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
int find_opcode(List **opTable, char *mnemonic, opcode_format *format) 	
{
	Node *temp;
	int hash;
	hash = hash_function(mnemonic, 20);					//find where the mnemonic is saved in the hashTable
	temp = opTable[hash]->head;
	while(temp != NULL) {
		if(!strcmp(temp->name, mnemonic)) {
			*format = temp->format;
			return temp->value;					//return the opcode of the mnemonic
		}
		temp = temp->next;
	}
	*format = not_format;
	return -1;	//can't find
}

/*---------------------*/
/*함수 : find_symbol*/
/*목적 : symbolTable에서 symbol을 찾는 함수*/
/*리턴값: 없음*/
/*---------------------*/

int find_symbol(List **sym_table, char *label)
{
	Node *temp;
	int hash = hash_function(label, 40);
	temp = sym_table[hash]->head;
	while(temp != NULL) {
		if(!strcmp(temp->name, label)) {
			return temp->value;		//return the address of the label
		}
		temp = temp->next;
	}
	return -1; //can't find the label
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
	case not_found_file :
		printf("can't find file. try again\n");
		break;
	case can_t_open_file :
		printf("can't open file. try again\n");
		break;
	case exist_symbol :
		printf("symbol is already exit. try again\n");
		break;
	case wrong_statement :
		printf("statement is wrong. check the asm file\n");
		break;
	default : 
		printf("something wrong. try again\n");
		break;
	}
	return -1;										//return -1, so do not put the command into the history list
}
