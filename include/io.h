#ifndef _IO_HDR
#define _IO_HDR


#include <stdio.h>
#include <stdlib.h>

/* for file read write function */
#include <sys/stat.h>
#include <fcntl.h>

/* for timestamp */
#include <time.h>

/* for standard data type definition */
#include <stdint.h>

/* Maximum bits in the buffer */
#define BIT_BUFFER_SIZE (8)

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
	

	/* original file permission */
	short usrperm;

	/* file size in bytes */
	off_t size;

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

}file;


file *open_file(char *name, int perm, int mode);

char read_char(file *input);

int write_char(file *output);

int read_binary_header(file *input);

int write_binary_header(file *output);

int write_bit(file *output, bit input_bit);

bit read_bit(file *input);

void closefile(file *input);

#endif


