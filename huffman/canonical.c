#include "huffman.h"

void plus_equal(codetype *code, int count){

	if(code == NULL)
		return;

	dcodetype i = 0, carry = 0; 
	int j = 0;

	for(j = 0; j < CODE_SIZE; j++){

		i = (dcodetype) code[j];
		i = i + (dcodetype) count + carry;

		if(i > (dcodetype)CODETYPE_MAX){

			carry = i / ((dcodetype) CODETYPE_MAX + (dcodetype) 1);
			
			i = i % ((dcodetype) CODETYPE_MAX + (dcodetype) 1);

			code[j] = (codetype) i;
		}
		else{
			carry = 0;
			code[j] = (codetype) i;
		}
		i = 0;
		count = 0;
	}

	return;
}

void shift_left_by_one(codetype *code){
	codetype mask = 1, 
			 prevmask = 1, 
			 t,
			 prev_t;

	int count = 0;

	while(mask != 0){
		prevmask = mask;
		mask = mask << 1;
		count++;
	}

	mask = prevmask;
	t = 0;

	for(int i = 0; i < CODE_SIZE; i++){

		prev_t = code[i] & mask;

		code[i] = code[i] << 1;

		code[i] = code[i] | (t >> (count - 1));

		t = prev_t;
	}

	return;
}

int copy(codetype *dest, codetype *src, int size){
	if(dest == NULL || src == NULL)
		return 0;

	for(int i = 0; i < size; i++){
		dest[i] = src[i];
	}

	return size;
}

void get_canonical_huffman_code(
		symboltable st, 
		int *codelength_count)
{

	if(st == NULL)
		return;

	if(codelength_count == NULL)
		return;

	codetype code[CODE_SIZE] = {0};
	codetype next_code[CHAR_RANGE][CODE_SIZE] = {0};

	int bits;

	codelength_count[0] = 0;

	/* get the base code 					*/
	for(bits = 1; bits < CHAR_RANGE; bits++){

		plus_equal(code, codelength_count[bits - 1]);

		shift_left_by_one(code);

		copy(next_code[bits], code, CODE_SIZE);
	}

	/* get code for each alphabet			*/
	int i;
	for(i = 0; i < CHAR_RANGE; i++){

		int length = st[i].codelength;
		if(length > 0){
			/* copy the base code for that codelenght  	*/
			copy(st[i].code, next_code[st[i].codelength], CODE_SIZE);

			/* Increment code by one 					*/
			plus_equal(next_code[st[i].codelength], 1);
			printf("%c, %d: ", st[i].ch, st[i].codelength);
			print_table(st[i].code, CODE_SIZE);
		}
	}

	return;
}

