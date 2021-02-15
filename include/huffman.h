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


/*! 
 * compares two nodes.
 *  @param[in] pointer to the first node
 *  @param[in] pointer to the second node
 *  @param[out] boolean denoting a < b
 */
static inline int lessthan(node *a, node *b){
	return a->sym->frequency < b->sym->frequency;
}

static inline int get_length(symbol *s){
	if(s == NULL)
		return -1;
	return s->codelength;
}


void huffman_encoder(file *infile, file *outfile);

void print_table(codetype *ct, int size);

#endif


