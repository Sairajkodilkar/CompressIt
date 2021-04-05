#include "hash.h"
#include <stdlib.h>

void init_hash(hash *hp, int maxsize){
	hp->size = 0;
	hp->maxsize = maxsize;

	for(int i = 0; i < _TABLESIZE; i++){
		hp->entries[i] = NULL;
	}
	return;
}

void init_entry(
		entry *curr, 
		int key, 
		u_char *str, 
		int size, 
		entry *next )
{
	if(curr == NULL){
		return;
	}
	curr->key = key;
	curr->str = str;
	curr->next = next;
	curr->size = size;
	return;
}

int __hashfunc(int key){
	return (key % _TABLESIZE);
}

entry **search(entry **start, int key){
	if(start == NULL){
		return NULL;
	}
	while(*start != NULL && ((*start)->key != key)){
		start = &((*start)->next);
	}
	return start;
}

entry *getnewentry(){
	entry *temp = (entry *) malloc(sizeof(entry));
	return temp;
}

/*char *hinsert(hash *hp, int key, unsigned char *str, int size){ */
entry *hinsert(hash *hp, entry *e){

	int key = e->key;

	/* get hashtable dict_index from hash function */
	int dict_index = __hashfunc(key);
	/* go to that row and search for key linearly in linked list */
	entry **found = search(&(hp->entries[dict_index]), key);
	if(found == NULL){
		return NULL;
	}

	if(*found == NULL){
		*found = getnewentry();
		if(*found == NULL){
			return NULL;
		}
		init_entry(*found, e->key, e->str, e->size, NULL);
		hp->size++;
		return e;
	}
	else{
		/* entry already exist */
		unsigned char *temp;
		int size;
		temp = (*found)->str;
		size = (*found)->size;
		(*found)->str = e->str;
		(*found)->size = e->size;
		e->str = temp;
		e->size = size;
		return e;
	}
}

entry *hsearch(hash *hp, entry *e){
	int key = e->key;
	int dict_index = __hashfunc(key);

	entry **found = search(&(hp->entries[dict_index]), key);

	if(found == NULL || *found == NULL){
		return NULL;
	}
	else{
		e->str = (*found)->str;
		e->size= (*found)->size;
		return e;
	}
}

void hdestroy(hash *hp){
	if(hp == NULL)
		return;
	entry *temp, *prev;
	for(int i = 0; i < _TABLESIZE; i++){
		temp = hp->entries[i];
		while(temp){
			prev = temp;
			temp = temp->next;
			free(prev);
		}
		hp->entries[i] = NULL;
	}
	hp->size = 0;
	return;
}

