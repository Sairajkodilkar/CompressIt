#include "huffman.h"
#include "canonical.h"

#include "heap.h"
#include "io.h"

#include <stdlib.h>
#include <unistd.h>

huffman_tree get_node(symbol *sp){

	huffman_tree temp = (node *) malloc(sizeof(node));

	if(temp == NULL)
		return NULL;

	temp->sym = sp;
	temp->left = temp->right = NULL;

	return temp;
}

int lessthan(void *s1, void *s2){
	return ((node *) s1)->sym->frequency < ((node *) s2)->sym->frequency;
}

huffman_tree combine_node(huffman_tree left, huffman_tree right){

	symbol *sp = (symbol *) malloc(sizeof(symbol));

	if(sp == NULL)
		return NULL;

	int left_freq = 0, right_freq = 0;

	if(left)
		left_freq = get_frequency(left->sym);

	if(right)
		right_freq = get_frequency(right->sym);

	set_frequency(sp, left_freq + right_freq);

	huffman_tree root = get_node(sp);

	root->left = left;
	root->right = right;

	copy(root->sym->code, right->sym->code, CODE_SIZE);
	shift_right_by_one(root->sym->code);
	set_codelength(root->sym, get_length(root->right->sym) - 1);

	return root;
}

void init_sym_table(symboltable st){

	if(st == NULL)
		return;

	symbol null_sym = {0, 0, 0, {0, 0, 0, 0}};

	for(int i = 0; i < CHAR_RANGE; i++){
		st[i] = null_sym;
		st[i].ch = i;
	}

	return ;
}

long int build_sym_table(file *infile, symboltable st){

	unsigned char ch;
	long int char_count = 0;

	while(read_file(infile, &ch, 1)){

		(st[ch].frequency)++;

		char_count++;
	}

	return char_count;
}

void build_priority_queue(priority_queue *sym_que, symboltable st, int (*comp) (void *, void *)){
	if(sym_que == NULL)
		return;

	symbol *sp = NULL;

	for(int i = 0; i < CHAR_RANGE; i++){

		sp = &st[i];

		if(get_frequency(sp)){
			queue_insert(sym_que, get_node(sp), comp);
		}
	}

	return;
}

huffman_tree build_huffman_tree(priority_queue *sym_que){
	huffman_tree left, right, root;

	do {
		left = queue_extract_min(sym_que, &lessthan);
		right = queue_extract_min(sym_que, &lessthan);

		root = combine_node(left, right);

		queue_insert(sym_que, root, &lessthan);

	} while(get_queue_size(sym_que) > 1);

	return queue_extract_min(sym_que, &lessthan);

}


void traverse(huffman_tree t){
	if(t == NULL)
		return;

	printf("%c ", t->sym->ch);
	traverse(t->left);
	traverse(t->right);

}

void _get_code_length(symboltable st, huffman_tree codetree, int depth){

	if(codetree == NULL)
		return;

	if(codetree->left == NULL && codetree->right == NULL){

		codetree->sym->codelength = depth;

		return;

	}

	_get_code_length(st, codetree->left, depth + 1);
	_get_code_length(st, codetree->right, depth + 1);

	return;
}

#define ZERO_DEPTH (0)

void get_code_length(symboltable st, huffman_tree codetree){

	return _get_code_length(st, codetree, ZERO_DEPTH);
}

void init_code_lenght_count(symboltable sym_table, int *codelength_count) {

	if(codelength_count == NULL)
		return;

	for(int i = 0; i < CHAR_RANGE; i++){
		codelength_count[i] = 0;
	}

	for(int i = 0; i < CHAR_RANGE; i++){

		int length = get_length(&sym_table[i]);

		if(length > 0)
			codelength_count[length]++;
	}

	return;
}

void destroy_huffman_tree(huffman_tree *codetree){
	if(!codetree || !(*codetree))
		return;

	if((*codetree)->left == NULL && (*codetree)->right == NULL){
		return;
	}

	destroy_huffman_tree(&((*codetree)->left));
	destroy_huffman_tree(&((*codetree)->right));

	free(*codetree);
	*codetree = NULL;

	return ;
}



long huffman_encoder(file *infile, file *outfile){

	symboltable sym_table;

	init_sym_table(sym_table);

	long int n = build_sym_table(infile, sym_table);

	set_char_size(infile, n);

	lseek_file(infile, 0, SEEK_SET);

	priority_queue sym_que;
	init_priority_queue(&sym_que);
	build_priority_queue(&sym_que, sym_table, &lessthan);

	huffman_tree codetree = build_huffman_tree(&sym_que);


	/* traverse through tree and find its code lenght 	*/
	get_code_length(sym_table, codetree);


	int codelength_count[CHAR_RANGE];

	init_code_lenght_count(sym_table, codelength_count);

	/* generates canonical huffman code for each symbol */
	get_canonical_huffman_code(sym_table, codelength_count);

	/* write huffman code to the file 					*/
	long filesize = write_huffman_code(sym_table, infile, outfile, codelength_count);

	destroy_huffman_tree(&codetree);

	return filesize ;
}

void build_huffman_tree_from_codelenght(symboltable st, int *codelenght_count){
	//build max priority_queue based on codelength
	//extract form it 
	//check the LSB of the extracted node, 0 goes to left & 1 to right
	//combine nodes in that fashion
	//build heap
}

long huffman_decoder(file *infile, file *outfile){

	symboltable sym_table;
	init_sym_table(sym_table);

	//read the long char count from infile.
	long char_size = read_char_size(infile);
	set_char_size(infile, char_size);

	int codelength_count[CHAR_RANGE];
	//read the count table from the file.
	read_code_length_count(infile, codelength_count, CHAR_RANGE);

	//generate code from that count.
	get_canonical_huffman_code(sym_table, codelength_count);

	//generate the tree from that code.
	//read file bit by bit till char count is zero and decode the character.
}





