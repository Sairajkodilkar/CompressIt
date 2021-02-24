#include "huffman.h"
#include "io.h"


int main(){
	file *infile  = open_file("testfile.txt", O_RDONLY, S_IRWXU);
	file *outfile = open_file("testfile.huff", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

	if(outfile == NULL)
		printf("file open fail\n");

	huffman_encoder(infile, outfile);
	return 0;
}
