#include "huffman.h"
#include "canonical.h"
#include "huff.h"

#include "heap.h"
#include "io.h"

#include <stdlib.h>
#include <unistd.h>



int greaterthan(void *s1, void *s2){

	symbol *sym1 = ((node *) s1)->sym,
		   *sym2 = ((node *) s2)->sym;

	if(get_length(sym1) > get_length(sym2))
		return 1;

	else if(get_length(sym1) == get_length(sym2)){

		return code_less_than(sym1, sym2);
	}

	return 0;
}



int valid_length(void *s){

	symbol *sym = (symbol *) s;

	return get_length(sym) > 0;
}


long huffman_decoder(file *infile, file *outfile){

	if(infile == NULL || outfile == NULL){
		return -1;
	}

	symboltable sym_table;
	init_sym_table(sym_table);

	//read the long char count from infile.
	long char_size = read_char_size(infile);
	set_char_size(infile, char_size);

	int codelength_count[CHAR_RANGE];

	//read the count table from the file.
	long file_size = read_code_length_count(infile, sym_table, CHAR_RANGE);

	init_code_lenght_count(sym_table, codelength_count);

	//generate code from that count.
	get_canonical_huffman_code(sym_table, codelength_count);


	priority_queue sym_que;
	init_priority_queue(&sym_que);
	build_priority_queue(&sym_que, sym_table, &greaterthan, &valid_length);

	//generate the tree from that code.
	huffman_tree codetree = build_huffman_tree(&sym_que, &greaterthan);


	//read file bit by bit till char count is zero and decode the character.
	file_size += inflate_file(infile, outfile, codetree, get_char_size(infile));

	destroy_huffman_tree(&codetree);

	return file_size;
}








