#include "lzw.h"

long lzw_decoder(file *infile, file *outfile){
	/* initizalize the dictionary to all 256 chars 	*/
	dict code_dict;

	init_dict(&code_dict);

	insert_chars(&code_dict);

	
	/* read two byte code from compressed file (infile)			*/
	/* decode the code, add string to outfile and add new entry as previous
	 * string + first character of new decoded string and*/
	/* If not possible to decode the entry then that string must be
	 * previous string plus the first char of prev string
	 */
	/* 	goto step 2 till all file is scan 	*/

}
