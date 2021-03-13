#include "file.h"
#include "lzw.h"

//longest length possible is log2(file length count)

long lzw_encoder(file *infile, file *outfile){
	/* NOTE : max code is limited to 2 byte 
	 * which limits the dictionary to 2^16 = 64k 
	 */

	/* initialize the dictionary with all characters		*/

	/* initialize the empty string  						*/

	/* if file is not empty read byte from file 			*/

	/* if str + ch is in file then go to step 3 			*/

	/* else output str code to file and add new entry
	 * str + ch to the dict and initialize the string to ch 
	 * i.e str = ch 
	 * go to step 3
	 */
}
