#ifndef _HUFFMAN
#define _HUFFMAN
//reading from file: 
// read file and count frequency

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



/*! 
 * compares two nodes.
 *  @param[in] pointer to the first node
 *  @param[in] pointer to the second node
 *  @param[out] boolean denoting a < b
 */
inline int lessthan(node *a, node *b){
	return a->sym->frequency < b->sym->frequency;
}



// Huffman :
// make the heap of that frequency TODO: write heap ADT for node
// extrating node from heap build huffman tree
// count the lenght of each pattern
// count the occurences of each length
// convert pattern into canonical huffman code

//writing to file:
//write header
//write the occurences of each length 
//write end of the file

#endif
