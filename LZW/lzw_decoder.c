#include <string.h>

#include "lzw.h"
#include "hash.h"

int insert_symbols(hash *hp){

	char str[] = "a";
	int i;
	for(i = 0; i < 256; i++){
		str[0] = (char ) i;
		char *t = strdup(str, 1);
		hinsert(hp, i, t);
	}
	return i;
}

int write_string(file *outfile, char *str){
	int l = strlen(str);
	return write_file(outfile, str, l);
}

char *concatinate(char *str, char ch){
	int l = strlen(str);
	char *conc = (char *) malloc(sizeof(char) * (l + 2));

	if(conc == NULL){
		return NULL;
	}

	for(int i = 0; i < l; i++){
		conc[i] = str[i];
	}
	conc[l] = ch;
	conc[l + 1] = '\0';
	return conc;
}

long lzw_decoder(file *infile, file *outfile){
	/* initizalize the dictionary to all 256 chars 				*/
	hash hashtable;

	init_hash(&hashtable, LZWCODE_MAX);
	long filesize = 0;

	int index = 0;
	index += insert_symbols(&hashtable);

	lzw_codetype code = t;

	/* read two byte code from compressed file (infile)			*/
	if(read_file(infile, &code, sizeof(code)) <= 0){
		/* file is empty 	*/
		hdestroy(&hashtable);
		return 0;
	}
	char *str, *pstr ;

	pstr = hsearch(&hashtable, (int) code);

	if(pstr == NULL){
		hdestroy(&hashtable);
		return -1;
	}
	write_string(outfile, pstr);

	while(read_file(infile, &code, sizeof(code)) > 0){
		str = hsearch(&hashtable, (int) code);
		if(str == NULL) {
			hinsert(&hashtable, (str = concatinate(pstr, pstr[0])), index);
		}
		else {
			hinsert(&hashtable, concatinate(pstr, str[0]), index);
		}
		write_string(outfile, str);
		index++;
		pstr = str;
	}
	hdestroy(&hashtable);
	
	return filesize;
}
