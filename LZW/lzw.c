#include "dict.h"

long insert_chars(dict *code_dict){
	char ch = 0;
	int reset = 1;
	long size = 0;
	for(ch = 0; ch < 256; ch++){
		reset = 1;
		if(insert_string(code_dict, ch, &reset) == -2)
			return -1;
		size++;
	}
	return size;
}
