#include "dict.h"


void init_trienode(struct trie *t, int count, unsigned char ch){
	if(!t)
		return;

	t->ch = ch;
	t->ind = count;
	t->sibling = NULL;
	t->left = NULL;
	t->right = NULL;
	return;
}


void init_dict(dict *dictionary){
	if(dictionary == NULL)
		return;

	dictionary->count = 0;
	dictionary->maxlength = MAXLENGTH;

	dictionary->root = (struct trie *) malloc(sizeof(struct trie));

	init_trienode(
			dictionary->root, 
			dictionary->count, 
			'\0');

	dictionary->count++;
}

struct trie *search_child(
		struct trie *root,
		struct trie **sibling,
		unsigned char ch)
{
	if(root == NULL || sibling == NULL)
		return NULL;

	*sibling = NULL;
	struct trie *temp = root->left;
	while(temp){
		if(temp->ch == ch)
			break;
		*sibling = temp;
		temp = temp->sibling;
	}

	return temp;
}

//enters the string in trie 
/* scans trie tree character by character and 
 * when it is found it continues to search in the its child
 * when it is not found in any of the child it is inserted as new child
 * and ind -1 is returned for not found 
 */
index insert_string(dict *diction, unsigned char ch, int *reset){
	static struct trie *temp;
	if(diction == NULL)
		return -2;

	if(*reset){
		temp = diction->root;
		*reset = 0;
	}

	if(diction->root == NULL)
		return -2;


	if(diction->count >= MAXLENGTH){
		return -1;
	}

	struct trie 
		*sibling = NULL,
		*next = NULL;

	next = search_child(temp, &sibling, ch);
	if(next == NULL){
		next = (struct trie *) malloc(sizeof(struct trie));
		init_trienode(next, diction->count++, ch);
		if(sibling == NULL){
			temp->left = temp->right = next;
		}
		else{
			sibling->sibling = next;
			temp->right = next;
		}
		return -1;
	}
	else{
		temp = next;
		return temp->ind;
	}

	return -1;
}

//searchs for the string ind in the trie
index get_ind(dict *dict, char *string){
	return 0;
}

void _destroy_dict(struct trie *root){
	if(root == NULL)
		return;

	struct trie *next = root->left;
	while(next){
		_destroy_dict(next);
		next = next->sibling;
	}
	free(root);
	return;
}

void destroy_dict(dict *dict){
	if(dict == NULL || dict->root == NULL){
		return;
	}
	struct trie *next = dict->root->left;
	struct trie *temp = NULL;
	while(next){
		temp = next->sibling;
		_destroy_dict(next);
		next = temp;
	}
	return;
}

