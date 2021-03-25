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

void init_entry(entry *curr, int key, char *str, entry *next){
	if(curr == NULL){
		return;
	}
	curr->key = key;
	curr->str = str;
	curr->next = next;
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

char *hinsert(hash *hp, int key, char *str){
	/* get hashtable dict_index from hash function */
	int dict_index = __hashfunc(key);
	/* go to that row and search for key linearly in linked list */
	entry **found = search(&(hp->entries[dict_index]), key);
	if(found == NULL){
		return NULL;
	}

	if(*found == NULL){
		if(hp->size >= hp->maxsize){
			return NULL;
		}
		*found = getnewentry();
		if(*found == NULL){
			return NULL;
		}
		init_entry(*found, key, str, NULL);
		hp->size++;
		return str;
	}
	else{
		/* entry already exist */
		char *temp;
		temp = (*found)->str;
		(*found)->str = str;
		return temp;
	}
}

char *hsearch(hash *hp, int key){
	int dict_index = __hashfunc(key);

	entry **found = search(&(hp->entries[dict_index]), key);

	if(found == NULL || *found == NULL){
		return NULL;
	}
	else{
		return (*found)->str;
	}
}

char *hdelete(hash *hp, int key){
	int dict_index = __hashfunc(key);

	entry **found = search(&(hp->entries[dict_index]), key);

	if(found == NULL){
		return NULL;
	}
	if(*found == NULL){
		return NULL;
	}
	else{
		entry *temp = (*found);
		*found = (*found)->next;
		char *str = temp->str;
		free(temp);
		hp->size--;
		return str;
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
}

