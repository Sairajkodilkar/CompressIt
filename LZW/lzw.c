#include "dict.h"
#include "limits.h"

long insert_chars(dict *code_dict){
	unsigned char ch = 1;
	int reset = 1;
	long size = 0;
	for(int i = 1; i <= UCHAR_MAX; i++, ch++){
		reset = 1;
		if(insert_string(code_dict, ch, &reset) == -2)
			return -1;
		size++;
	}
	return size;
}
