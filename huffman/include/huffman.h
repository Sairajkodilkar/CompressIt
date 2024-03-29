#ifndef _HUFFMAN
#define _HUFFMAN

#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#include "io.h"

/*! \def CHAR_RANGE
 * \brief Maximum number of character possible
 */
#define CHAR_RANGE (256)

/*! \def CODE_SIZE
 *\brief determines array length for canonical huffman code 
 */
#define CODE_SIZE (8)

/*! Type of the huffman code that would be written to the file              */
typedef uint32_t codetype;

/*! maximum code possible           */
#define CODETYPE_MAX UINT32_MAX 

/*! double of Type of the huffman code that would be written to the file    */
typedef uint64_t dcodetype; 

/*! maximum double code type possible for huffman codes  */
#define DCODETYPE_MAX UINT64_MAX 


typedef struct symbol {

	/*! Byte representing any character from 0 - 255   		*/
	unsigned char ch;

	/*! Frequency of the character 							*/
	int frequency;

	/*! Huffman code length of that character 				*/
	int codelength;

	/*! Canonical huffman code for charater 			    */
	codetype code[CODE_SIZE];

} symbol;

/* Node for building huffman tree 					*/
typedef struct node {

    /*! Pointer to the symbol struct                */
	symbol *sym;

    /*! Left and right child pointer                */
	struct node *left, *right;

} node;


/*! Table to contain symbol struct for each byte in byte range */
typedef symbol symboltable[CHAR_RANGE];

/*! Huffman tree is Tree which is traversed for generating huffman code */
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

static inline codetype *get_code(symbol *st){
	if(st == NULL)
		return NULL;

	codetype *temp = st->code;

	return temp;
}

static inline symbol *get_symbol(node *n){
	if(!n)
		return NULL;

	return n->sym;
}

static inline void set_char(symbol *s, unsigned char ch){
	if(!s)
		return;
	s->ch = ch;
	return;
}
static inline unsigned char get_char(symbol *s){
	if(!s)
		return 0;

	return s->ch;
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


