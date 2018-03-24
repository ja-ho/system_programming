#include "20111166.h"

int memory_range_dump(unsigned char memory[][MEM_ROW], int start, int end, int *address) {
	int rows, cols, end_cols;
	int count, range;
	int i,j;
	if(start > 0xfffff || end > 0xfffff || (start > end) || start < 0 || end < 0) {
		return handle_error(out_of_boundary);
	}
	
	count = end - start;//total count
	rows = start/16;	//start row
	cols = start%16;	//start col
	end_cols = end%16;	//end col
	range = end/16 - start/16 + 1;
	for(i=rows; i<range; i++) {
		printf("%05x ", rows*16);				//memory address
		if(i==range-1) {
			
		}
		for(j=0; j<16; j++) {					//memory content
			if((i==0 && j < cols) || (i==range-1 && j > end_cols)) {
				printf("   ");
			}
			printf("%02x ", memory[i][j]);
		}
		printf("; ");
		for(j=0; j<16; j++) {					//print ascii code
			if((i==0 && j<cols) || (i==range-1 && j > end_cols)) {
				printf(".");
			}
			if(memory[i][j] >= 0x20 && memory[i][j] <= 0x7E) {
				printf("%c", memory[i][j]);
			} else {
				printf(".");
			}
		}
		printf("\n");
	}
	*address = end + 1;
	return 0;
}

int memory_dump(unsigned char memory[][MEM_ROW], int *address)
{
	if(*address + 159 > 0xFFFFF) {
		return handle_error(out_of_boundary);
	}
	memory_range_dump(memory, *address, *address+159, address);
	return 0;
}

int memory_start_dump(unsigned char memory[][MEM_ROW], int start, int *address)
{
	if(start + 159 > 0xFFFFF) {
		memory_range_dump(memory, start, 0xFFFFF, address);
	} else {
		memory_range_dump(memory, start, start+159, address);
	}
}
