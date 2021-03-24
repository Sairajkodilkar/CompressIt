#ifndef _DICT_H
#define _DICT_H

#include <stdio.h>
#include <stdlib.h>



/* Builds the trie tree of the form
 * 			   root
 *            /    \
 *           /      \
 * 			/        \
 * 		   /          \
 * 		  /            \
 * 		 /              \
 * 	   ch1-->ch2-->ch3-->ch4
 * 	 
 */


typedef int32_t index;

struct trie {

	unsigned char ch;
	index ind;

	struct trie *sibling,
				*left, 
				*right;

};

typedef struct dict {

	struct trie *root;

	index count;
	index maxlength;

} dict;


//init the dictionary
void init_dict(dict *dictionary);

//enters the string in trie 
index insert_string(dict *dictionary, unsigned char ch, int *reset);

//searchs for the string index in the trie
index get_index(dict *dict, char *string);

void destroy_dict(dict *dict);


#endif

