#include "huffman.h"


/* code_one < code_two */
int code_less_than(symbol *one, symbol *two){

	if(!one || !two)
		return -1;

	int l1, l2;
	l1 = get_length(one);
	l2 = get_length(two);

	if(l1 < l2)
		return 1;

	if(l2 > l1)
		return 0;

	else{
		codetype *code_one = one->code;
		codetype *code_two = two->code;

		int bit_size = sizeof(codetype) * 8;
		int index = CODE_SIZE - ((l1 - 1) / bit_size) - 1;

		for(; index < CODE_SIZE; index++){

			if(code_one[index] < code_two[index]){
				return 1;
			}
			else if(code_one[index] > code_two[index]){
				return 0;
			}
			else
				continue;
		}
	}

	return 0;

}

/* n goes from 0 to codelenght - 1 */

bit get_nbit(symbol *s, int n){
	if(!s)
		return 2;

	int length = s->codelength;

	if(n >= length)
		return 2; 		//ivalid bit

	//as index starts from right side
	n = length - n - 1;

	codetype *ct = s->code;

	int bit_size = sizeof(codetype) * 8;

	//global index
	int index = CODE_SIZE - (int)(n / bit_size) - 1;

	//local index
	codetype bit_index = n % bit_size;
	codetype mask = 1 << bit_index;

	bit b = (ct[index] & mask) >> bit_index;

	return b;
}

void plus_equal(codetype *code, int count){

	if(code == NULL)
		return;

	dcodetype i = 0, carry = 0; 
	int j = 0;

	for(j = CODE_SIZE - 1; j >= 0; j--){

		i = (dcodetype) code[j];
		i = (dcodetype) i + (dcodetype) count + (dcodetype) carry;

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

	if(!code)
		return;

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

	for(int i = CODE_SIZE - 1; i >= 0; i--){

		prev_t = code[i] & mask;

		code[i] = code[i] << 1;

		code[i] = code[i] | (t >> (count - 1));

		t = prev_t;
	}

	return;
}

void shift_right_by_one(codetype *code){
	if(!code)
		return ;

	codetype mask = 1, 
			 t,
			 prev_t;

	int count = 0;

	while(mask != 0){
		mask = mask << 1;
		count++;
	}

	mask = 1;
	t = 0;

	for(int i = 0; i < CODE_SIZE; i++){

		prev_t = code[i] & mask;

		code[i] = code[i] >> 1;

		code[i] = code[i] | (t << (count - 1));

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

	if(st == NULL || codelength_count == NULL)
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

		int length = get_length(&st[i]);
		if(length > 0){
			/* copy the base code for that codelenght  	*/
			copy(get_code(&st[i]), next_code[length], CODE_SIZE);

			/* Increment code by one 					*/
			plus_equal(next_code[length], 1);
		}
	}

	return;
}

