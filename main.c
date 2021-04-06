#include "huffman.h"
#include "lzw.h"
#include "io.h"

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>

void die(char *errstr);
char *get_filename(char *infile, int flag);

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

/* file extensions 		*/
#define HUFFEXT ".huff"
#define LZWEXT ".lzw"
#define TXTEXT ".txt"

void usage(int status){
	if(status){
		die("Invalid use, Try using -u option for more info");
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


long compress(file *infile, file *outfile, int flag, double *time_taken){
	long size = 0;

	clock_t start = 0, end = 0;
	if(ISHUFFMAN(flag)){
		start = clock();
		size = huffman_encoder(infile, outfile);
		end = clock();
	}
	else if(ISLZW(flag)){
		start = clock();
		size = lzw_encoder(infile, outfile);
		end = clock();
	}
	else {
		die("please specify only one method\n");
	}

	*time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;

	return size;
}


long decompress(file *infile, file *outfile, int flag, double *time_taken){
	long size = 0;

	clock_t start = 0, end = 0;
	if(ISHUFFMAN(flag)){
		start = clock();
		size = huffman_decoder(infile, outfile);
		end = clock();
	}
	else if(ISLZW(flag)){
		start = clock();
		size = lzw_decoder(infile, outfile);
		end = clock();
	}
	else {
		die("please specify only one method\n");
	}

	*time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;

	return size;
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

		case 'l':
			return LZW;
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

const char optstring[] = "uhlcxf:o:";

int main(int argc, char **argv){

	char *infilename = NULL, 
		 *outfilename = NULL;

	file *infile = NULL, 
		 *outfile = NULL;

	int option = 0, flag = 0;


	/* Parse argv for options 			*/
	while((option = getopt(argc, argv, optstring)) != -1){

		if(option == '?'){
			usage(1);
		}

		flag |= getflag(option);

		if(ISHELP(flag)){
			usage(0);
		}

		/* output filename specified 			*/
		if(option == 'f'){
			if(infilename != NULL){
				usage(1);
			}
			infilename = optarg;
		}
		/* output file name specified 		*/
		if(option == 'o'){
			if(outfilename != NULL){
				usage(1);
			}
			outfilename = optarg;
		}
	}
	if(infilename == NULL){
		die("Please specify filename\n");
	}

	if(outfilename == NULL){
		outfilename = get_filename(infilename, flag);
	}
	
	/* open both the files 								*/
	infile = open_file(infilename, RMODE, PERM);
	outfile = open_file(outfilename, WCMODE, PERM);
	/* compress or decompress both the files	 		*/
	if(ISCOMPRESSION(flag) && ISEXTRACT(flag)){
		die("You cannot specify both -x and -c options\n");
	}

	long insize = get_file_size(infile);
	long outsize = 0;
	/* time taken in seconds		*/
	double time_taken;
	if(ISCOMPRESSION(flag)){
		outsize = compress(infile, outfile, flag, &time_taken);
	}
	else{
		outsize = decompress(infile, outfile, flag, &time_taken);
	}
	/* get the percentage compress or decompress		*/
	double percentage = ((double)outsize / (double)insize) * 100.0f;
	if(!(flag && OUTPUT))
		free(outfilename);

	close_file(infile);
	close_file(outfile);

	/* print time required, infilesize, outfilesize, percentage [de]compressed */
	printf("%8f, %ld, %ld, %8f\n", time_taken, insize, outsize, percentage);

	return 0;
}

void die(char *errstr){
	fprintf(stderr, "%s", errstr);
	exit(EXIT_FAILURE);
	return;
}

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


char *detach(char *infilename, char *ext){
	char *last_dot = rindex(infilename, '.');
	if(strcmp(last_dot, ext) != 0){
		die("unknown inputfile suffix --ignored\n");
	}
	char *new = strndup(infilename, strlen(infilename) - strlen(last_dot));
	return new;
}
char *get_filename(char *infile, int flag){
	char *newfile;
	if(ISCOMPRESSION(flag)) { 
		if(ISHUFFMAN(flag)) {
			newfile = attach(infile, HUFFEXT);
		}
		else if(ISLZW(flag)) {
			newfile = attach(infile, LZWEXT);
		}
	}
	if(ISEXTRACT(flag)){
		if(ISHUFFMAN(flag)) {
			newfile = detach(infile, HUFFEXT);
		}
		else if(ISLZW(flag)) {
			newfile = detach(infile, LZWEXT);
		}
	}
	if(access(newfile, F_OK) == 0){
		printf("%s already exist do you want to overwrite? [y/n]: ", newfile);	
		char yn;
		scanf("%c", &yn);
		switch(yn) {
			case 'y':
				return newfile;
				break;
			case 'n':
				die("");
				break;
		}
	}

	return newfile;
}




