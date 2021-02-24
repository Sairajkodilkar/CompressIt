#include "huffman.h"
#include "canonical.h"
#include "io.h"



int write_count(file *outfile, int *codelength_count){
	/* unsigned char is enough to hold max codelength count i.e 255 */
	unsigned char ch;
	int i = 0;
	
	for(i = 0; i < CHAR_RANGE; i++){
		ch = (unsigned char) codelength_count[i];
		write_file(outfile, &ch, 1);
	}
	return i;
}

/* second pass for input file is done here */

int write_codes(file *infile, file *outfile, symboltable st){

	unsigned char ch;
	int count = 0;
	bit b = 0;

	while(read_file(infile, &ch, 1) != 0){

		int codelength = get_length(&st[ch]);

		for(int i = 0; i < codelength; i++){

			b = get_nbit(&st[ch], i);

			count += write_bit(outfile, b, NO_EOF);
		}
	}
	count += write_bit(outfile, b, EOF_FLAG);

	return count;
}

int write_char_size(file *infile, file *outfile){

	long int t = get_char_size(infile);

	return write_file(outfile, &t, sizeof(t));
}


int write_huffman_code(
		symboltable st,
		file *infile,
		file *outfile,
		int *codelength_count
		)

{
	if(outfile == NULL || infile == NULL)
		return -1;
	/* will consider later 
	   header = get_file_attribute(infile);
	   write_file_header(outfile, header);
	 */
	long file_char_count = 0;

	file_char_count += write_char_size(infile, outfile);

	file_char_count += write_count(outfile, codelength_count);

	file_char_count += write_codes(infile, outfile, st);

	return 0;
}


