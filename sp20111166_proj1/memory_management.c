#include "20111166.h"

int memory_range_dump(unsigned char memory[][MEM_ROW], int start, int end, int *address) {
	int rows, cols, end_rows;
	int range;
	int i,j;
	if(start > 0xfffff || end > 0xfffff || (start > end) || start < 0 || end < 0) {
		return handle_error(out_of_boundary);
	}
	cols = start/16;	//start col
	rows = start%16;	//start row
	end_rows = end%16;	//end col
	range = end/16 - start/16 + 1;
	for(i=cols; i<range + cols; i++) {
		printf("%05x ", i*16);				//memory address

		for(j=0; j<16; j++) {					//memory content
			if((i==cols && j < rows) || (i == range + cols - 1 && j > end_rows)) {
				printf("   ");
			} else {
				printf("%02x ", memory[i][j]);
			}
		}
		printf("; ");
		for(j=0; j<16; j++) {					//print ascii code
			if((i==cols && j<rows) || (i == range + cols - 1 && j > end_rows)) {
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
		return memory_range_dump(memory, start, 0xFFFFF, address);
	} else {
		return memory_range_dump(memory, start, start+159, address);
	}
}

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

