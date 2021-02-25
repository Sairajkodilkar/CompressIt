#include "huffman.h"
#include "io.h"


int main(){
	file *infile  = open_file("testfile.huff", O_RDONLY, S_IRWXU);
	file *outfile = open_file("testfile2.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

	if(outfile == NULL){
		printf("file open fail\n");
		return 0;
	}

	huffman_decoder(infile, outfile);
	return 0;
}
