#include "huffman.h"
#include "huff.h"
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

void print_code(codetype code[CODE_SIZE]){
	for(int i = 0; i < CODE_SIZE; i++){
		printf("%d ", code[i]);
	}
	printf("\n");
	return;
}

int isleaf(huffman_tree n, huffman_tree root){

	if(n && n->left == NULL && n->right == NULL && n != root)
		return 1;

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
	huffman_tree t = right ? right : left;

	symbol *sroot = get_symbol(root);
	symbol *st = get_symbol(t);

	if(t){
		copy(get_code(sroot), get_code(st), CODE_SIZE);

		shift_right_by_one(get_code(sroot));

		set_codelength(sroot, get_length(st) - 1);
	}
	else {
		codetype cp[CODE_SIZE] = {0};

		set_codelength(sroot, 0);

		copy(get_code(sroot), cp, CODE_SIZE);
	}

	return root;
}

void init_sym_table(symboltable st){

	if(st == NULL)
		return;

	symbol null_sym = {0, 0, 0, {0, 0, 0, 0}};

	for(int i = 0; i < CHAR_RANGE; i++){
		st[i] = null_sym;
		set_char(&st[i], i);
	}

	return ;
}

long int build_sym_table(file *infile, symboltable st){

	if(!infile || !st)
		return -1;

	unsigned char ch;
	long int char_count = 0;

	while(read_file(infile, &ch, 1)){
		int fre = get_frequency(&st[ch]);

		set_frequency(&st[ch], fre + 1);

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
	if(sym_que == NULL || st == NULL || comp == NULL || valid == NULL)
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
	if(!sym_que || !comp)
		return NULL;

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

	if(isleaf(t, NULL)){
		printf("%c , %d", t->sym->ch, code);
		print_code(t->sym->code);
	}
	traverse(t->left, (code << 1) + 0);
	traverse(t->right, (code << 1) + 1);

}

int search_tree(huffman_tree codetree, bit b, char *ch){

	if(!ch)
		return -1;

	if(!codetree)
		return NOT_FOUND;

	static huffman_tree next = NULL;

	if(next == NULL){
		next = codetree;
	}

	next = (b == 0) ? next->left : next->right;

	if(isleaf(next, codetree)){
		*ch = get_char(get_symbol(next));
		next = codetree;
		return FOUND;
	}
	else{
		return NOT_FOUND;
	}

	return -1;
}


void _get_code_length(symboltable st, huffman_tree codetree, int depth){

	if(codetree == NULL)
		return;

	if(codetree->left == NULL && codetree->right == NULL){

		symbol *s = get_symbol(codetree);

		set_codelength(s, depth);

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

    free((*codetree)->sym);
	free(*codetree);
	*codetree = NULL;

	return ;
}


