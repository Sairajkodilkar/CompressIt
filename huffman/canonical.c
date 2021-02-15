#include "huffman.h"
#include "io.h"

void plus_equal(codetype *code, int count){

	if(code == NULL || codelength_count == NULL)
		return;

	dcodetype i = 0, carry = 0; 
	int j = 0;

	for(j = 0; j < CODE_SIZE; j++){

		i = (dcodetype) code[j];
		i = i + (dcodetype) count + carry;

		if(i > UINT32_MAX){

			carry = i / (UINT32_MAX + 1);
			
			i = i % (UINT32_MAX + 1);

			code[j] = (codetype) i;
		}
		else{
			carry = 0;
			code[j] = i;
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

		code[i] = code[i] | (t >> count - 1);

		t = prev_t;
	}

	return;
}

void get_canonical_huffman_code(
		symboltable st, 
		int *codelenght_count)
{


	codetype code[CODE_SIZE] = {0, 0, 0, 0};

	int bits;

	/* get the base code 					*/
	for(bits = 0; bits < CHAR_RANGE; bits++){

		plus_equal(code, codelength_count[bits - 1]);

		shift_by_one(code);
	}


}

