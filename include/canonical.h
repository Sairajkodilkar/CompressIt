#ifndef _CANONICAL_H
#define _CANONICAL_H

#include "huffman.h"

void get_canonical_huffman_code(
		symboltable st,
		int *codelength_count);

int code_less_than(symbol *one, symbol *two);

void shift_left_by_one(codetype *code);

void shift_right_by_one(codetype *code);

int copy(codetype *dest, codetype *src, int size);

bit get_nbit(symbol *st, int n);


#endif

