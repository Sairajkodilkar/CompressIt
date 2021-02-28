#ifndef _HUFF_H
#define _HUFF_H
#include "huffman.h"
#include "heap.h"
#include "io.h"

huffman_tree get_node(symbol *sp);

huffman_tree combine_node(huffman_tree left, huffman_tree right);

void init_sym_table(symboltable st);

long int build_sym_table(file *infile, symboltable st);

void build_priority_queue(
		priority_queue *sym_que, 
		symboltable st, 
		int (*comp) (void *, void *),
		int (*valid) (void *)
		);

huffman_tree build_huffman_tree(priority_queue *sym_que, int (*comp) (void *, void *));

void traverse(huffman_tree t, int code);

void get_code_length(symboltable st, huffman_tree codetree);

void init_code_lenght_count(symboltable sym_table, int *codelength_count) ;

void destroy_huffman_tree(huffman_tree *codetree);

int isleaf(huffman_tree n, huffman_tree root);

#endif
