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

	if(root->right){
		copy(root->sym->code, right->sym->code, CODE_SIZE);

		shift_right_by_one(root->sym->code);

		set_codelength(root->sym, get_length(root->right->sym) - 1);
	}
	else if(root->left){
		copy(root->sym->code, left->sym->code, CODE_SIZE);

		shift_right_by_one(root->sym->code);

		set_codelength(root->sym, get_length(root->left->sym) - 1);
	}
	else {
		codetype cp[CODE_SIZE] = {0};

		set_codelength(root->sym, 0);

		copy(root->sym->code, cp, CODE_SIZE);
	}

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

void build_priority_queue(
		priority_queue *sym_que, 
		symboltable st, 
		int (*comp) (void *, void *),
		int (*valid) (void *)
		)
{
	if(sym_que == NULL)
		return;

	symbol *sp = NULL;

	for(int i = 0; i < CHAR_RANGE; i++){

		sp = &st[i];

		if((*valid)(sp)){
			queue_insert(sym_que, get_node(sp), comp);
		}
	}

	return;
}

huffman_tree build_huffman_tree(priority_queue *sym_que, int (*comp) (void *, void *)){
	huffman_tree left, right, root;

	do {
		left = queue_extract_min(sym_que, comp);
		right = queue_extract_min(sym_que, comp);

		root = combine_node(left, right);

		queue_insert(sym_que, root, comp);

	} while(get_queue_size(sym_que) > 1);

	return queue_extract_min(sym_que, comp);

}


void traverse(huffman_tree t, int code){
	if(t == NULL)
		return;

	if(isleaf(t)){
		printf("%c , %d", t->sym->ch, code);
		print_code(t->sym->code);
	}
	traverse(t->left, (code << 1) + 0);
	traverse(t->right, (code << 1) + 1);

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
int valid_length(void *s){
	symbol *sym = (symbol *) s;

	return get_length(sym) > 0;
}

void print_code(codetype *code){
	for(int i = 0; i < CODE_SIZE; i++){
		printf("%d ", code[i]);
	}
	printf("\n");
	return;
}

long inflate_file(
		file *infile, 
		file *outfile, 
		huffman_tree codetree, 
		long char_size);

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
	long size = read_code_length_count(infile, sym_table, CHAR_RANGE);

	init_code_lenght_count(sym_table, codelength_count);

	//generate code from that count.
	get_canonical_huffman_code(sym_table, codelength_count);


	priority_queue sym_que;
	init_priority_queue(&sym_que);
	build_priority_queue(&sym_que, sym_table, &greaterthan, &valid_length);

	//generate the tree from that code.
	huffman_tree codetree = build_huffman_tree(&sym_que, &greaterthan);
	traverse(codetree, 0);


	//read file bit by bit till char count is zero and decode the character.

	long file_size = inflate_file(infile, outfile, codetree, get_char_size(infile));


	return 0;
	//return file_size;
}

enum status {NOT_FOUND, FOUND};

int isleaf(huffman_tree n, huffman_tree root){

	if(n && n->left == NULL && n->right == NULL && n != root)
		return 1;

	return 0;
}

int search_tree(huffman_tree codetree, bit b, char *ch){
	static huffman_tree next = NULL;


	if(next == NULL){
		next = codetree;
	}

	next = (b == 0) ? next->left : next->right;

	if(isleaf(next, codetree)){
		*ch = next->sym->ch;
		next = codetree;
		return FOUND;
	}
	else{
		return NOT_FOUND;
	}

	return -1;
}

long inflate_file(
		file *infile, 
		file *outfile, 
		huffman_tree codetree, 
		long char_size)
{

	bit b;
	long count = 0;
	int found;
	char ch;

	while(count != char_size){

		read_bit(infile, &b);

		found = search_tree(codetree, b, &ch);

		if(found){
			count++;
			write_file(outfile, &ch, sizeof(ch));
		}

	}
	//printf("\n");
	return count;
}






