#ifndef _DICT_H
#define _DICT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>



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


typedef int32_t dict_index;
typedef uint32_t udict_index;

struct trie {

	unsigned char ch;
	dict_index ind;

	struct trie *sibling,
				*left, 
				*right;

};

typedef struct dict {

	struct trie *root;

	dict_index count;
	dict_index maxlength;

} dict;


//init the dictionary
void init_dict(dict *dictionary, dict_index maxsize);

//enters the string in trie 
dict_index insert_string(dict *dictionary, unsigned char ch, int *reset);

//searchs for the string dict_index in the trie
dict_index get_dict_index(dict *dict, char *string);

void destroy_dict(dict *dict);


#endif

