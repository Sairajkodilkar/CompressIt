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
	for(int i = 1; i <= 255; i++, ch++){
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

	unsigned char ch = '\0';

	int reset = 1;
	dict_index status = 0;
	lzw_codetype prevdict_index = -1;

	int stringsize = 0, max = 0;

	while(read_file(infile, &ch, sizeof(ch)) > 0){

		status = insert_string(&code_dict, ch, &reset);
		stringsize++;
		if(stringsize > max){
			max = stringsize;
		}
		if(status == -2){
			destroy_dict(&code_dict);
			return -1;
		}
		else if(status == -1 || stringsize >= INT16_MAX){
			reset = 1;

			filesize += write_file(outfile, &prevdict_index, sizeof(prevdict_index));

			status = insert_string(&code_dict, ch, &reset);
			stringsize = 0;
		}
		prevdict_index = (lzw_codetype)status;
	}

	/* end of the file condition */
	filesize += write_file(outfile, &prevdict_index, sizeof(prevdict_index));

	destroy_dict(&code_dict);
	printf("max %d\n", max);
	return filesize;
}
