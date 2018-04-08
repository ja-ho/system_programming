#include "20111166.h"

/*---------------------*/
/*함수 : memory_range_dump*/
/*목적 : 범위 안의 메모리 값을 출력한다*/
/*리턴값: 0= 정상, -1 = 비정상*/
/*---------------------*/
int memory_range_dump(unsigned char memory[][MEM_ROW], int start, int end, int *address) {		//dump memory from start, to end
	int rows, cols, end_rows;
	int range;
	int i,j;
	if(start > 0xfffff || end > 0xfffff || (start > end) || start < 0 || end < 0) {				//error handling(check the start, end, address are in the memory boundary)
		return handle_error(out_of_boundary);
	}
	cols = start/16;	//start col
	rows = start%16;	//start row
	end_rows = end%16;	//end col
	range = end/16 - start/16 + 1;
	for(i=cols; i<range + cols; i++) {
		printf("%05X ", i*16);				//memory address

		for(j=0; j<16; j++) {					//memory content
			if((i==cols && j < rows) || (i == range + cols - 1 && j > end_rows)) {		//handle the exception case(first, end last row)
				printf("   ");
			} else {
				printf("%02X ", memory[i][j]);
			}
		}
		printf("; ");
		for(j=0; j<16; j++) {					//print ascii code
			if((i==cols && j<rows) || (i == range + cols - 1 && j > end_rows)) {		//handle the exception case(first, end last row)
				printf(".");
			} else {
				if(memory[i][j] >= 0x20 && memory[i][j] <= 0x7E) {
					printf("%c", memory[i][j]);
				} else {
					printf(".");
				}
			}
		}
		printf("\n");
	}
	*address = end + 1;
	return 0;
}

/*---------------------*/
/*함수 : memory_dump*/
/*목적 : address부터 160개의 메모리값을 출력한다*/
/*리턴값: 0= 정상, -1 = 비정상*/
/*---------------------*/
int memory_dump(unsigned char memory[][MEM_ROW], int *address)
{
	if(*address > 0xFFFFF) {
		memory_range_dump(memory, 0x00000,  0x00000+159, address);
		return 0;
	}
	if(*address + 159 > 0xFFFFF) {
		memory_range_dump(memory, *address, 0xFFFFF, address);
		return 0;
	}
	memory_range_dump(memory, *address, *address+159, address);
	return 0;
}

/*---------------------*/
/*함수 : memory_start_dump*/
/*목적 : start address부터 160개의 메모리 값을 출력한다*/
/*리턴값: 0= 정상, -1 = 비정상*/
/*---------------------*/
int memory_start_dump(unsigned char memory[][MEM_ROW], int start, int *address)
{
	if(start + 159 > 0xFFFFF) {
		return memory_range_dump(memory, start, 0xFFFFF, address);
	} else {
		return memory_range_dump(memory, start, start+159, address);
	}
}


/*---------------------*/
/*함수 : memory_edit*/
/*목적 : address의 메모리 값을 수정한다*/
/*리턴값: 0= 정상, -1 = 비정상*/
/*---------------------*/
int memory_edit(unsigned char memory[][MEM_ROW], int address, int value)
{
	int rows, cols;
	if(address < 0 || address > 0xfffff || value < 0 || value > 0xff) {
		return handle_error(out_of_boundary);
	}

	cols = address/16;	//start col
	rows = address%16;	//start row
	memory[cols][rows] = value;
	return 0;
}

/*---------------------*/
/*함수 : memory_fill*/
/*목적 : 범위 안의 메모리 값을 value로 채운다*/
/*리턴값: 0= 정상, -1 = 비정상*/
/*---------------------*/
int memory_fill(unsigned char memory[][MEM_ROW], int start, int end, int value)
{
	int rows, cols, end_rows, range;
	int i, j;

	if(start > 0xfffff || end > 0xfffff || (start > end) || start < 0 || end < 0 || value < 0 || value > 0xff) {
		return handle_error(out_of_boundary);
	}
	
	cols = start/16;	//start col
	rows = start%16;	//start row
	end_rows = end%16;	//end row
	range = end/16 - start/16 + 1;

	for(i=cols; i<range + cols; i++) {
		for(j=0; j<16; j++) {					//memory content
			if((i==cols && j < rows) || (i == range + cols - 1 && j > end_rows)) {
				continue;
			} else {
				memory[i][j] = value;
			}
		}
	}
	return 0;
}

/*---------------------*/
/*함수 : memory_reset*/
/*목적 : 메모리 값을 전부 0으로 초기화한다*/
/*리턴값: 없음*/
/*---------------------*/
void memory_reset(unsigned char memory[][MEM_ROW]) 
{
	int i, j;
	for(i=0; i<MEM_COL; i++) {
		for(j=0; j<MEM_ROW; j++) {
			memory[i][j] = 0;
		}
	}
	return;
}

