#include "io.h"
#include "lzw.h"
#include "dict.h"

#include <limits.h>

#define MAXSIZE (1 << 16)

/* NOTE : max code is limited to 2 byte 
 * which limits the dictionary to 2^16 = 64k 
 * longest length possible is log2(file length count)
 */

long insert_chars(dict *code_dict){
	unsigned char ch = 0;
	int reset = 1;
	long size = 0;
	for(int i = 0; i <= UCHAR_MAX; i++, ch++){
		reset = 1;
		if(insert_string(code_dict, ch, &reset) == -2)
			return -1;
		size++;
	}
	return size;
}

long lzw_encoder(file *infile, file *outfile){

	if(infile == NULL || outfile == NULL || infile == outfile)
		return -1;

	long filesize = 0;

	dict code_dict;

	/* Maximum dict_index possible is UINT16_MAX 				*/
	init_dict(&code_dict,  LZWCODE_MAX);

	/* initialize the dictionary with all characters		*/
	if(insert_chars(&code_dict) < 0){
		destroy_dict(&code_dict);
		return -1;
	}
	int currentsize = 255;
	int max = LZWCODE_MAX;

	unsigned char ch = '\0';

	int reset = 1;
	dict_index status = 0;
	lzw_codetype prevdict_index = -1;
	int newline = 0;

	while(read_file(infile, &ch, sizeof(ch)) > 0){

		if(ch == '\n')
			newline++;

		status = insert_string(&code_dict, ch, &reset);

		if(status == -1 && currentsize < max)
			currentsize++;

		if(status == -2){
			destroy_dict(&code_dict);
			return -1;
		}
		else if(status == -1){

			reset = 1;

			filesize += write_file(outfile, &prevdict_index, sizeof(prevdict_index));

			status = insert_string(&code_dict, ch, &reset);
		}
		prevdict_index = (lzw_codetype)status;
	}

	/* end of the file condition */
	filesize += write_file(outfile, &prevdict_index, sizeof(prevdict_index));

	destroy_dict(&code_dict);
	return filesize;
}
