#include "huffman.h"
#include "io.h"



int write_count(file *outfile, int *codelength_count){
	/* unsigned char is enough to hold max codelength count i.e 255 */
	unsigned char ch;
	int i = 0;
	
	for(i = 0; i < CHAR_RANGE; i++){
		ch = (unsigned char) codelength[i];
		write_file(outfile, ch, 1);
	}
	return i;
}




int write_huffman_code(
		symboltable st,
		file *infile,
		file *outfile,
		int *codelength_count
		)

{
	if(outfile == NULL)
		return -1;
	/* will consider later 
	header = get_file_header(infile);
	write_file_header(outfile, header);
	*/
	long file_char_count = 0;

	file_char_count += write_count(outfile, codelength_count);

	file_char_count += write_bits(infile, outfile, st);

	return 0;
}
		

