#include "hash.h"

void init_hash(hash *hp, int maxsize){
	hp->size = 0;
	hp->maxsize = maxsize;

	for(int i = 0; i < _TABLESIZE; i++){
		hp->entries[i] = NULL;
	}

	return;
}


