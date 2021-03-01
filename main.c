#include "huffman.h"
#include "io.h"
#include "err.h"

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define NAMESIZE (512)
#define PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define RMODE (O_RDONLY)
#define WCMODE (O_CREAT | O_WRONLY)
#define MAXEXT 5

char optstring[] = "hlcxuf:";

void usage(int status, char **argv){
	if(status){
		raise("'Try compression -u for usage'", NULL);
		exit(1);
	}
	else{
		printf("Usage compress [OPTION..] [FILE]..\n");
		printf("compress can compress the file using huffman or LZW algorithm\n");
		printf("\nExamples :\n\tcompress -chf compressfile.huff foo\n\tcompress -xhf compressedfile.huff foo\n");
		printf("\nMain operation Mode:\n\
	-h use huffman algorithm for compression\n\
	-l use lzw algorithm for compression\n\
	-c compress file\n\
	-x extract the compressed file\n\
	-f use the given file as input\n");

		exit(0);
	}
}

/* file extensions 		*/
#define HUFFEXT ".huff"
#define LZWEXT ".lzw"
#define TXTEXT ".txt"

char *attach(char *infilename, const char *ext){
	char *filename;
	if(infilename == NULL){
		return NULL;
	}
	filename = (char *) malloc(sizeof(char) * NAMESIZE);

	if(filename == NULL){
		return NULL;
	}
	strncpy(filename, infilename, NAMESIZE - 1 - MAXEXT);
	if(ext != NULL)
		strcat(filename, ext);

	return filename;
}

char *detach(char *infilename){
	char *last_dot = rindex(infilename, '.');
	if(last_dot){
		*last_dot = '\0';
	}
	return infilename;
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

		case 'u':
			return HELP;
			break;

		default:
			return -1;
	}
}

int main(int argc, char **argv){

	char *inputfilename = NULL;

	int option = 0, flag = 0;
	long size;

	file *infile = NULL, *outfile = NULL;

	while((option = getopt(argc, argv, optstring)) != -1){

		if(option == '?'){
			usage(1, argv);
			exit(1);
		}

		flag |= getflag(option);

		if(ISHELP(flag)){
			usage(0, argv);
		}

		if(ISCOMPRESSION(flag) && ISEXTRACT(flag)){
			raise("You can not specify both -x and -c", argv);
			exit(1);
		}

		if(option == 'f'){
			if(inputfilename != NULL){
				raise("only one source can be specified", argv);
				usage(1, argv);
			}
			inputfilename = optarg;
			infile = open_file(inputfilename, RMODE, 0);
			if(infile == NULL){
				perror("compression");
				exit(errno);
			}
		}
	}
	if(!ISFILE(flag)){
		raise("inputfile required", argv);
		usage(1, argv);
	}
	char *outfilename = NULL;

	if(outfilename == NULL){
		free(outfilename);
		perror("compression");
		exit(errno);
	}

	if(!ISHUFFMAN(flag) && !ISLZW(flag)){
		raise("please specify the algorithm", NULL);
		free(outfilename);
		usage(1, argv);
	}

	if(ISCOMPRESSION(flag)){
		if(ISHUFFMAN(flag)){
			outfilename = attach(inputfilename, HUFFEXT);
			if(outfilename == NULL){
				raise("invalid filename\n", argv);
				exit(1);
			}
			outfile = open_file(outfilename, WCMODE, PERM);
			size = huffman_encoder(infile, outfile);
		}
	}
	else if(ISEXTRACT(flag)){
		if(ISHUFFMAN(flag)){
			outfilename = detach(inputfilename);
			if(outfilename == NULL){
				raise("invalid filename\n", argv);
				exit(1);
			}
			outfile = open_file(outfilename, WCMODE, PERM);
			size = huffman_decoder(infile, outfile);
		}
	}
	else{
		raise("Please specify action", argv);
		free(outfilename);
		usage(1, argv);
	}
	free(outfilename);

	//calculate percentage;

	return 0;
}
