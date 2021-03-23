#include "io.h"
#include "lzw.h"
#include "dict.h"


/* NOTE : max code is limited to 2 byte 
 * which limits the dictionary to 2^16 = 64k 
 * longest length possible is log2(file length count)
 */

long lzw_encoder(file *infile, file *outfile){

	if(infile == NULL || outfile == NULL || infile == outfile)
		return -1;

	long filesize = 0;

	dict code_dict;
	init_dict(&code_dict);

	/* initialize the dictionary with all characters		*/
	if((filesize += insert_chars(&code_dict)) < 0){
		destroy_dict(&code_dict);
		return -1;
	}

	char ch = '\0';

	int reset = 1;
	index status = 0;
	uint16_t previndex = -1;

	while(read_file(infile, &ch, sizeof(ch)) > 0){

		status = insert_string(&code_dict, ch, &reset);
		if(status == -2){
			destroy_dict(&code_dict);
			return -1;
		}
		else if(status == -1){
			reset = 1;

			filesize += write_file(outfile, &previndex, sizeof(previndex));

			status = insert_string(&code_dict, ch, &reset);
		}
		previndex = (int16_t)status;
	}
	destroy_dict(&code_dict);
	return filesize;
}
