#include "hash.h"

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
	while(*start != NULL && ((*start)->key != key)){
		start = (*start)->next;
	}
	return start;
}

char *hinsert(hash *hp, int key, char *str){
	/* get hashtable index from hash function */
	int index = __hashfunc(key);
	/* go to that row and search for key linearly in linked list */
	entry **found = search(&(hp->entries[index]), key);

	if(*found == NULL){
		if(hp->size >= maxsize){
			return NULL;
		}
		*found = getnewentry();
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
	int index = __hashfunc(key);

	entry **found = search(&(hp->entries[index]), key);

	if(*found == NULL){
		return NULL;
	}
	else{
		return (*found)->str;
	}
}

char *hdelete(hash *hp, int key){
	int index = __hashfunc(key);

	entry **found = search(&(hp->entries[index]), key);

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

