#include "lzw.h"
#include "hash.h"

typedef unsigned char uchar;

int insert_symbols(hash *hp){
	entry e;
	e.size = 1;
	int i;
	for(i = 0; i < 256; i++){
		uchar *t = (uchar *) malloc(sizeof(uchar));
		*t = (uchar) i;
		e.key = i;
		e.str = t;
		hinsert(hp, &e);
	}
	return i;
}

int write_ustring(file *outfile, uchar *str, int size){
	return write_file(outfile, str, size);
}

char *concatinate(uchar *str, int size, uchar ch){
	uchar *nstr = (uchar *) malloc(sizeof(uchar) * (size + 1));
	if(nstr == NULL)
		return NULL;
	for(int i = 0; i < size; i++){
		nstr[i] = str[i];
	}
	nstr[size] = ch;
	return nstr;
}

long lzw_decoder(file *infile, file *outfile){
	/* initizalize the dictionary to all 256 chars 				*/
	hash hashtable;

	init_hash(&hashtable, LZWCODE_MAX);
	long filesize = 0;

	int dict_index = 0;
	dict_index += insert_symbols(&hashtable);

	lzw_codetype code = 0;

	/* read two byte code from compressed file (infile)			*/
	if(read_file(infile, &code, sizeof(code)) <= 0){
		/* file is empty 	*/
		hdestroy(&hashtable);
		return 0;
	}
	entry *e, *pe, *temp;
	e = getnewentry();
	temp = pe = getnewentry();

	pe->key = (int ) code;

	pe = hsearch(&hashtable, pe);

	if(pe == NULL){
		free(e);
		free(temp);
		hdestroy(&hashtable);
		return -1;
	}
	filesize += write_ustring(outfile, pe->str, pe->size);

	while(read_file(infile, &code, sizeof(code)) > 0){
		e->key = (int) code;
		temp = hsearch(&hashtable, e);
		if(temp == NULL) {
			e->key = dict_index;
			e->str = concatinate(pe->str, pe->size, pe->str[0]);
			e->size = pe->size + 1;
			hinsert(&hashtable, e);
		}
		else {
			entry conc;
			conc.str = concatinate(pe->str, pe->size, e->str[0]);
			conc.size = pe->size + 1;
			conc.key = dict_index;
			hinsert(&hashtable, &conc);
		}
		filesize += write_ustring(outfile, e->str, e->size);
		dict_index++;
		*pe = *e;
	}
	free(pe);
	free(e);
	hdestroy(&hashtable);
	return filesize;
}
