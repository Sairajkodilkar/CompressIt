#include "huffman.h"
#include "canonical.h"
#include "huff.h"

#include "heap.h"
#include "io.h"

#include <stdlib.h>
#include <unistd.h>

int lessthan(void *s1, void *s2){
	return ((node *) s1)->sym->frequency < ((node *) s2)->sym->frequency;
}

int valid_sym(void *s){

	symbol *sym = (symbol *) s;

	return get_frequency(sym) > 0;
}

long huffman_encoder(file *infile, file *outfile){

	symboltable sym_table;
	init_sym_table(sym_table);

	long int n = build_sym_table(infile, sym_table);

	set_char_size(infile, n);

	lseek_file(infile, 0, SEEK_SET);

	priority_queue sym_que;
	init_priority_queue(&sym_que);
	build_priority_queue(&sym_que, sym_table, &lessthan, &valid_sym);

	huffman_tree codetree = build_huffman_tree(&sym_que, &lessthan);


	/* traverse through tree and find its code lenght 	*/
	get_code_length(sym_table, codetree);


	int codelength_count[CHAR_RANGE];

	init_code_lenght_count(sym_table, codelength_count);


	/* generates canonical huffman code for each symbol */
	get_canonical_huffman_code(sym_table, codelength_count);

	traverse(codetree, 0);

	/* write huffman code to the file 					*/
	long filesize = write_huffman_code(sym_table, infile, outfile, codelength_count);

	destroy_huffman_tree(&codetree);

	return filesize ;
}
