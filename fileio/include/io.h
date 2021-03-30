#ifndef _IO_HDR
#define _IO_HDR


#include <stdio.h>
#include <stdlib.h>

/* for file read write function 		*/
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* for timestamp */
#include <time.h>

/* for standard data type definition 	*/
#include <stdint.h>

#include <errno.h>

/* Maximum bits in the buffer 			*/
#define BIT_BUFFER_SIZE (8)


/* end of the file flag 				*/
#define NO_EOF (0)
#define EOF_FLAG (1)




typedef char bit;

typedef struct attributes {
	/* Flag to determine if its already compressed or not */
	int8_t compress;

	/* To determine method of compression 
	 * 00H : Huffman 
	 * 01H : LZW
	 */
	int8_t method;

	/* File timestamp 
	 * time[0] : access time
	 * time[1] : modification time
	 */
	struct timespec time[2]; 

	/* flag to determine if file is in error state */
	int errstat;
	

	/* original file permission 		*/
	short usrperm;

	/* file block size in bytes 		*/
	off_t size;

	/* no. of characters in file 		*/
	long int char_size;


}attributes;

typedef struct file{
	/* File descriptor of the file */
	int fd;


	/* buffer to store incoming bits 
	 * NOTE: buffer is considered to be 8 bit because of endianess
	 */
	uint8_t read_buffer;
	uint8_t write_buffer;
	int read_count;
	int write_count;

	/* permission bits for program */
	short perm;

	/* file attributes for input file */
	attributes atrb;

} file;

static inline void set_char_size(file *infile, long int size){

	if(infile == NULL)
		return;

	infile->atrb.char_size = size;
	return;
}

static inline long int get_char_size(file *infile){
	if(infile == NULL)
		return -1;

	return infile->atrb.char_size;
}

static inline void set_file_size(file *infile, long int size){
	if(infile == NULL)
		return;
	infile->atrb.size = size;
}

static inline long int get_file_size(file *infile){
	if(infile == NULL)
		return -1;

	return infile->atrb.size;
}


file *open_file(char *name, int perm, int mode) ;

int read_file(file *input, void *ch, size_t count);

int read_binary_header(file *input);

int read_bit(file *input, bit *in_bit);

off_t lseek_file(file *infile, off_t offset, int whenec);

int write_file(file *input, void *ch, size_t count);

int write_binary_header(file *output, file *infile);

int write_bit(file *output, bit in_bit, int eof_flag);

void close_file(file *fi);

#endif


