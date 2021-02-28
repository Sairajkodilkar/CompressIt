#ifndef _HUFFMAN
#define _HUFFMAN

#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#include "io.h"

#define CHAR_RANGE (256)

#define CODE_SIZE (8)

typedef uint32_t codetype;
typedef uint64_t dcodetype; 

#define CODETYPE_MAX UINT32_MAX 
#define DCODETYPE_MAX UINT64_MAX 

typedef struct symbol {
	
	/* Byte representing any character from 0 - 255   		*/
	unsigned char ch;

	/* Frequency of the character 							*/
	int frequency;

	/* Huffman code length of that character 				*/
	int codelength;

	/* Canonical huffman code for charater 					*/
	codetype code[CODE_SIZE];

} symbol;

/* Node for building huffman tree 					*/
typedef struct node {

	symbol *sym;

	struct node *left, *right;

} node;


/* symbol table cantaining each symbol 				*/
typedef symbol symboltable[CHAR_RANGE];

typedef node * huffman_tree;

static inline int get_frequency(symbol *s){

	if(s == NULL)
		return 0;

	return s->frequency;
}

static inline void set_frequency(symbol *s, int x){
	if(s == NULL)
		return;

	s->frequency = x;
	return;
}


static inline int get_length(symbol *s){
	if(s == NULL)
		return -1;
	return s->codelength;
}

static inline void set_codelength(symbol *s, int n){
	if(s == NULL) return ;

	if(n < 0) n = 0;

	s->codelength = n;
}

static inline codetype *get_code(symboltable st, unsigned char ch){
	if(st == NULL)
		return NULL;

	codetype *temp = st[ch].code;

	return temp;
}

enum {NOT_FOUND, FOUND};



long huffman_encoder(file *infile, file *outfile);
long huffman_decoder(file *infile, file *outfile);

void print_table(codetype *ct, int size);


long write_huffman_code(
		symboltable st,
		file *infile,
		file *outfile,
		int *codelength_count
		);

long read_char_size(file *infile);

long read_code_length_count(file *infile, symboltable st, int n);

long inflate_file(
		file *infile, 
		file *outfile, 
		huffman_tree codetree, 
		long char_size);

int search_tree(huffman_tree codetree, bit b, char *ch);

#endif


