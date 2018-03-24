#include "20111166.h"

void memory_dump(unsigned char memory[][MEM_ROW], int *address) 
{
	int i, j;
	int rows= *address/16;
	if(rows + 10 >= MEM_COL) {
		
	}
	for(i=0; i<10; i++) {
		printf("%5x  ", *address);					//memory address
		for(j=0; j<16; j++) {
			printf("%2x ", memory[rows + i][j]);	//memory content
		}
		printf(";  ");
		for(j=0; j<16; j++) {						//show ascii code
			if(memory[rows + i][j] >= 0x20 && memory[rows + i][j] <= 0x7E) {
				printf("%c", memory[rows + i][j]);
			} else {
				printf(".");
			}
		}
		printf("\n");
	}
	*address += 160;
}


int memory_range_dump(unsigned char memory[][MEM_ROW], int start, int end, int *address) {
	int rows, cols;
	int count;
	if(end > 0xfffff) {
		return handle_error(out_of_boundary);
	}
	
	count = end - start;
	rows = start/16;
	cols = start%16;
	for(;;);
	while(count--) {
		
	}
}

