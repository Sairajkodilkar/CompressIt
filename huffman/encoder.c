#include "huffman.h"

#include "heap.h"
#include "io.h"

#include <stdlib.h>
#include <unistd.h>

huffman_tree get_node(symbol *sp){

	huffman_tree temp = (node *) malloc(sizeof(node));

	if(temp == NULL)
		return;

	temp->sym = sp;
	temp->left = temp->right = NULL;

	return temp;
}

huffman_tree combine_node(huffman_tree left, huffman_tree right){

	symbol *sp = (symbol *) malloc(sizeof(symbol));

	if(sp == NULL)
		return NULL;

	int left_freq = 0, right_freq = 0;

	if(left)
		left_freq = get_frequency(sp);

	if(right)
		right_freq = get_frequency(sp);

	set_frequency(sp, left_freq + right_freq);

	huffman_tree root = get_node(sp);

	root->left = left;
	root->right = right;

	return root;
}

void init_sym_table(symboltable st){

	if(st == NULL)
		return;

	symbol null_sym = {0, 0, 0, {0, 0, 0, 0}};

	for(int i = 0; i < CHAR_RANGE; i++){
		st[i] = null_sym;
	}

	return ;
}

int build_sym_table(file *infile, symboltable st){

	unsigned char ch;
	int count = 1;
	int char_count = 0;

	while(read_file(infile, &ch, 1)){

		(st[ch].frequency)++;

		char_count++;
	}

	return char_count;
}

void build_priority_queue(priority_queue *sym_que, symboltable st){
	if(sym_que == NULL)
		return;

	symbol *sp = NULL;

	for(int i = 0; i < CHAR_RANGE; i++){

		sp = &st[i];

		if(get_frequency(sp))
			queue_insert(sym_que, sp)
	}

	return;
}

huffman_tree build_huffman_tree(priority_queue *sym_que){
	huffman_tree left, right, root;

	do {
		left = queue_extract_min(sym_que);
		right = queue_extract_min(sym_que);

		root = combine_node(left, right);

		queue_insert(sym_que, root);

	} while(get_queue_size(sym_que) > 1);

	return queue_extract_min(sym_que);

}

void huffman_encoder(file *infile, file *outfile){

	sumboltable sym_table;

	init_sym_table(sym_table);

	build_sym_table(sym_table, infile);

	lseek_file(infile, 0, SEEK_SET);

	priority_queue sym_que;
	init_priority_queue(&sym_que)
	build_priority_queue(sym_que, sym_table);

	huffman_tree codetree = build_huffman_tree(sym_que);

	return;
}





