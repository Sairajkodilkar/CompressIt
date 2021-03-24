#ifndef __HASH_H
#define __HASH_H
#include <stdio.h>


/* 2K entries */
#define _TABLESIZE (1 << 12)



typedef struct entry {
	int key;
	char *str;
	struct entry *next;
} entry;


typedef struct hash {
	entry *entries[_TABLESIZE];
	int size;
	int maxsize;
} hash;



void init_hash(hash *hp, int maxsize);

/* str must be already allocated by user		 			*/
int hinsert(hash *hp, int key, char *str);

/* searches the key and returns the pointer to the string 	*/
char *hsearch(hash *hp, int key);

/* deletes the entry and return string associated with it 	*/
char *hdelete(hash *hp, int key);



















#endif 

