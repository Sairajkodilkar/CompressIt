#ifndef __HASH_H
#define __HASH_H
#include <stdio.h>


/* 2K entries */
#define _TABLESIZE (1 << 12)



typedef struct entry {
	int key;
	unsigned char *str;
	int size;
	struct entry *next;
} entry;


typedef struct hash {
	entry *entries[_TABLESIZE];
	int size;
	int maxsize;
} hash;



void init_hash(hash *hp, int maxsize);

entry *getnewentry(void);

/* str must be already allocated by user		 			*/
entry *hinsert(hash *hp, entry *e);

/* searches the key and returns the pointer to the string 	*/
entry *hsearch(hash *hp, entry *e);

void hdestroy(hash *hp);


#endif 
