#include "huffman.h"
#include "io.h"
#include "err.h"

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
/* TODO:
 * 	redesign Make the options simple and add first byte to destination compress file
 * 	to be compression method 
 */

#define NAMESIZE (512)
#define PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define RMODE (O_RDONLY)
#define WCMODE (O_CREAT | O_WRONLY)
#define MAXEXT 5

/* compression flags 					*/
enum {
	COMPRESSION = 1, 
	EXTRACT = COMPRESSION << 1, 
	HUFFMAN = EXTRACT << 1, 
	LZW = HUFFMAN << 1,
	FI = LZW << 1,
	HELP = FI << 1,
	OUTPUT = HELP << 1
};

#define ISCOMPRESSION(flag) ((flag) & COMPRESSION)
#define ISEXTRACT(flag) ((flag) & EXTRACT)
#define ISHUFFMAN(flag) ((flag) & HUFFMAN)
#define ISLZW(flag) ((flag) & LZW)
#define ISFILE(flag) ((flag) & FI)
#define ISHELP(flag) ((flag) & HELP)

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

		case 'o':
			return OUTPUT;
			break;

		default:
			return -1;
	}
}

const char optstring[] = "hlcxuf:o:";

int main(int argc, char **argv){

	char *inputfilename = NULL;
	char *outfilename = NULL;

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
		if(option == 'o'){
			outfilename = optarg;
		}
	}

	if(!ISFILE(flag)){
		raise("inputfile required", argv);
		usage(1, argv);
	}

	if(!ISHUFFMAN(flag) && !ISLZW(flag)){
		raise("please specify the algorithm", NULL);
		free(outfilename);
		usage(1, argv);
	}


	if(ISCOMPRESSION(flag)){
		if(ISHUFFMAN(flag)){
			if(outfilename == NULL){
				outfilename = attach(inputfilename, HUFFEXT);
				if(outfilename == NULL){
					raise("invalid filename\n", argv);
					exit(1);
				}
			}
			outfile = open_file(outfilename, WCMODE, PERM);
			size = huffman_encoder(infile, outfile);
		}
	}
	else if(ISEXTRACT(flag)){
		if(ISHUFFMAN(flag)){
			if(outfilename == NULL){
				outfilename = detach(inputfilename);
				attach(outfilename, TXTEXT);
				if(outfilename == NULL){
					raise("invalid filename\n", argv);
					exit(1);
				}
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
	close_file(infile);
	infile = NULL;
	close_file(outfile);
	outfile = NULL;

	//TODO :calculate percentage;

	return 0;
}
