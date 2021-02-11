#ifndef _HUFFMAN
#define _HUFFMAN

#include <stdio.h>
#include <stdint.h>

#define CHAR_RANGE (256)

typedef struct symbol {
	
	/* Byte representing any character from 0 - 255   		*/
	unsigned char ch;

	/* Frequency of the character 							*/
	int frequency;

	/* Huffman code length of that character 				*/
	int codelength;

	/* Canonical huffman code for charater 					*/
	int64_t code[4];

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
	return s->frequency;
}

static inline void set_frequency(symbol *s, int x){
	s->frequency = x;
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




