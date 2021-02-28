#include "huffman.h"
#include "io.h"
#include <unistd.h>

#define NAMESIZE (512)

char optstring[] = "hlcxuf:";

void usage(int status){
	printf("Compressor");
}

int getflag(int option){
	switch(option){
		case 'h':
			return HUFFMAN;
			break;

		case 'c':
			return COMPRESSION;
			break;

		case 'x':
			return EXTRACT;
			break;

		case 'f':
			return FI;
			break;

		default:
			return -1;
	}
}

int main(int argc, char **argv){

	char *inputfilename = NULL;

	int option = 0, flag = 0;
	long size;

	while((option = getopt(argc, argv, optstring)) != -1){

		if(option == '?'){
			exit(1);
		}

		flag |= getflag(option);

		if(ISCOMPRESSION(flag) && ISEXTRACT(flag)){
			//raise("You can not specify both -x and -c");
			exit(1);
		}

		if(option == 'f'){
			/*
			   if(inputfilename != NULL){
			   raise("only one source can be specified");
			   }
			   */
			inputfilename = optarg;
			printf("filname %s\n", inputfilename);
		}
	}
	if(!ISFILE(flag)){
		raise("inputfile required");
	}

	if(ISCOMPRESSION(flag)){
		if(ISHUFFMAN(flag)){
			size = huffman_encoder(infile, outfile);
		}
	}
	else if(ISEXTRACT(flag)){
		if(ISHUFFMAN(flag)){
			size = huffman_decoder(infile, outfile);
		}
	}

	return 0;
}
