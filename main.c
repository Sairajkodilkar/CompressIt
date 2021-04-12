/*! \file main.c
 * \brief entry point for the program.
 *
 * This file integrates the Huffman and LZW module
 */

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

/*!\def NAMESIZE 
 * \brief Max filename size.
 */
#define NAMESIZE (512)

/*!\def PERM
 *\brief File permissions for new file creation.
 */
#define PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*!\def RMODE
 * \brief Read mode flag
 */
#define RMODE (O_RDONLY)

/*!\def WCTMODE
 * \brief Write, Truncate, Create flag
 */
#define WCTMODE (O_CREAT | O_WRONLY | O_TRUNC)

/*!\def MAXEXT
 * \brief Maximum file extension size
 */
#define MAXEXT (5)

/*! \enum Flag
 *  \brief Flag values for option handling.
 */
enum {
	COMPRESSION = 1, 
	EXTRACT = COMPRESSION << 1, 
	HUFFMAN = EXTRACT << 1, 
	LZW = HUFFMAN << 1,
	IFILE = LZW << 1,
	OFILE = IFILE << 1,
	HELP = OFILE << 1,
	OVERWRITE = HELP << 1
} Flag;


/*! \def ISCOMPRESSION(flag)
 * \brief checks if compression flag is enable or not 
 */
#define ISCOMPRESSION(flag) ((flag) & COMPRESSION)

/*! \def ISEXTRACT(flag)
 * \brief checks if extract flag is enable or not 
 */
#define ISEXTRACT(flag) ((flag) & EXTRACT)

/*! \def ISHUFFMAN(flag)
 * \brief checks if huffman flag is enable or not 
 */
#define ISHUFFMAN(flag) ((flag) & HUFFMAN)

/*! \def ISLZW(flag)
 * \brief checks if LZW flag is enable or not 
 */
#define ISLZW(flag) ((flag) & LZW)

/*! \def ISIFILE(flag)
 * \brief checks if IFILE flag is enable or not 
 */
#define ISIFILE(flag) ((flag) & IFILE)

/*! \def ISHELP(flag)
 * \brief checks if Help flag is enable or not 
 */
#define ISHELP(flag) ((flag) & HELP)

/*! \def ISOFILE(flag)
 * \brief checks if OFILE flag is enable or not
 */
#define ISOFILE(flag) ((flag) & OFILE)


/*! \def ISOVERWRITE(flag)
 * \brief checks if OVERWRITE is enable or not
 */
#define ISOVERWRITE(flag) ((flag) & OVERWRITE)

/* file extensions 		*/

/*! \def HUFFEXT
 *  \brief defines huffman file extension
 */
#define HUFFEXT ".huff"

/*! \def LZWEXT
 *  \brief defines lzw file extension
 */
#define LZWEXT ".lzw"

/*! \def TXTEXT 
 *  \brief defines text file extension
 */
#define TXTEXT ".txt"

/*! \fn void usage(int status)
 *  \brief prints the help message to the user depending on status value
 *  \param status if this is one then program exits using die
 *  \see \ref die
 */
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

/*! 
 * \brief compress the file using appropriate algorithm
 * \param infile input file which need to be compress 
 * \param outfile output file where compressed data needs to
 *          store.
 * \param flag This determines compression algorithm.
 * \param time_taken pointer to the variable where time of compression or 
 *          decompression is store
 *
 * \see lzw_encoder()
 * \see huffman_encoder()
 */
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
		size = lzw_encoder(infile, outfile); end = clock();
	}
	else {
		die("please specify only one method\n");
	}

	*time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;

	return size;
}

/*!
 * \brief decompress the file using appropriate algorithm
 * \param infile input file which need to be decompress
 * \param outfile output file where decompressed data needs to
 *          store.
 * \param flag This determines compression algorithm.
 * \param time_taken pointer to the variable where time of compression or 
 *          decompression is store
 *
 * \see \ref lzw_decoder()
 * \see \ref huffman_decoder()
 */
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

/*!
 * \brief gets the flag flag value from option
 * \param option The option character.h
 * \return emum value from Flag
 */
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
			return IFILE;
			break;

		case 'u':
			return HELP;
			break;

		case 'o':
			return OFILE;
			break;

		case 'y':
			return OVERWRITE;
			break;

		default:
			return -1;
	}
}

/*! \brief option string used parsing commandline options */
const char optstring[] = "uhlcxf:o:y";

/*! 
 * \brief Entry point for the program.
 *      
 *      Integrate all modules 
 */
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

    if(access(infilename, F_OK) == -1){
        die("file does not exist\n");
    }

	if(outfilename == NULL){
		outfilename = get_filename(infilename, flag);
	}
	
	/* open both the files 								*/
	infile = open_file(infilename, RMODE, PERM);
	outfile = open_file(outfilename, WCTMODE, PERM);
	/* compress or decompress both the files	 		*/
	if(ISCOMPRESSION(flag) && ISEXTRACT(flag)){
		die("You cannot specify both -x and -c options\n");
	}
	if(ISLZW(flag) && ISHUFFMAN(flag)){
		die("You cannot specify both -h and -l options\n");
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
	double percentage = (((double)insize - (double)outsize) / (double) insize) * 100.0f;

	if(!(ISOFILE(flag))) {
		free(outfilename);
    }

	close_file(infile);
	close_file(outfile);

	/* print infilename, time required, infilesize, outfilesize, percentage [de]compressed */
	printf("%s, %8f, %ld, %ld, %8f\n", infilename, time_taken, insize, outsize, percentage);

	return 0;
}

/*! 
 * \brief exits the program by printing error string to standard error
 * \param errstr pointer to the error string 
 * \note After calling this program exits with Failure status
 */
void die(char *errstr){
	fprintf(stderr, "%s", errstr);
	exit(EXIT_FAILURE);
	return;
}


/*!
 * \brief attache the extension string to the filename
 * \param infilename pointer to the string where extension needs to be added
 * \param ext filename extension to be attached
 * \return pointer to the new file name with extension
 * \note The return string is malloced so user must call free after usage
 */
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

/*! 
 * \brief removes the given extension from filename
 * \param infilename original filename 
 * \param ext extension which needs to be removed
 * \return returns the new file name with extension removed
 * \note return string is allocated using malloc hence needs to be 
 *       freed using free. 
 */

char *detach(char *infilename, char *ext){
	char *last_dot = rindex(infilename, '.');
	if(strcmp(last_dot, ext) != 0){
		die("unknown inputfile suffix --ignored\n");
	}
	char *new = strndup(infilename, strlen(infilename) - strlen(last_dot));
	return new;
}

/*! \brief gets the outputfilename using flag values and inputfilename
 * \param infile input filename
 * \param flag appropriate flag
 * \see \ref enum Flag
 *
 */
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
	if(!(ISOVERWRITE(flag)) && access(newfile, F_OK) != -1){

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



