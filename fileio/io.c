#include <stdio.h>
#include <errno.h>

#include "io.h"
/* utimensat to set the time attribute of file */

#define NOERROR (0)
#define ERROR (-1)

/* Handles file input output */

void init_file_atrb(
		file *in, 
		int errstat, 
		struct timespec atime, 
		struct timespec mtime,
		int perm, 
		off_t size)
{

	in->atrb.compress = 0;
	in->atrb.method = 0;
	in->atrb.time[0] = atime;
	in->atrb.time[1] = mtime;
	in->atrb.errstat = errstat;
	in->atrb.usrperm = perm;
	in->atrb.size = size;


}

/* opens the file for reading */
file *open_file(char *name, int perm, int mode) {
	//TODO: handle large file path name 
	
	/* opens the given file name with specified permission */
	int fd = open(name, perm, mode);

	if(fd == -1){
		return NULL;
	}

	file *inputfile = (file *)malloc(sizeof(file));

	if(inputfile == NULL){
		return NULL;
	}

	/* sets general file info */
	inputfile->fd = fd;
	inputfile->read_buffer = 0;
	inputfile->write_buffer = 0;
	inputfile->read_count = 0;
	inputfile->write_count = 0;
	inputfile->perm = perm;


	struct stat filestat;
	int status = fstat(fd, &filestat);

	if(status == ERROR)
		return NULL;

	/* set file attributes */
	init_file_atrb(inputfile, NOERROR, filestat.st_atim, filestat.st_mtim, filestat.st_mode, filestat.st_size);

	return inputfile;
}

void setflags(file *fp, int compress, int method){

	/* for setting huffman or lzw flag compression flag must be set */
	fp->atrb.compress =  compress;
	fp->atrb.method = method;
	return;
}




#define BYTE (1)

/* Read functions				 				*/


/* reads the header in compressed file 			*/
/*
int read_binary_header(file *input){
	return 0;
}
*/

int read_file(file *input, void *ch, size_t count){
	return read(input->fd, ch, count);
}

int read_bit(file *input, bit *in_bit){
	int filestat;
	/* check if buffer is empty					 		*/
	if(input->read_count == 0){
		filestat = read(input->fd, &(input->read_buffer), BYTE);
		input->read_count = BIT_BUFFER_SIZE;
	}

	/* read bit from the MSB side and decrement count 	*/
	int offset = --input->read_count;
	*in_bit = (input->read_buffer & (1 << offset)) >> offset;

	return filestat;
}


off_t lseek_file(file *infile, off_t offset, int whence){
	return lseek(infile->fd, offset, whence);
}



/* Write functions 					*/

int flush_file(file *infile){
	int x = 0;
	if(!infile)
		return -1;

	if(infile->write_count != 0 && infile->perm & O_WRONLY){

		x = write_file(infile, &(infile->write_buffer), sizeof(infile->write_buffer));

		infile->write_count = 0;
	}

	return x;
}

int write_file(file *input, void *ch, size_t count){
	return write(input->fd, ch, count);
}

/* write the header to header file */
int write_binary_header(file *offile, file *tofile){

	if(offile == NULL || tofile == NULL)
		return -1;

	attributes *file_header = &(offile->atrb);

	return write_file(tofile, file_header, sizeof(attributes));
}




/* writes to the file bit by bit */

// if eof_flag is set in_bit is ignored

int write_bit(file *output, bit in_bit, int eof_flag){

	//TODO if end of the file flag is set then empty the buffer 
	if(eof_flag & EOF_FLAG){
		return flush_file(output);
	}

	if(!(in_bit == 1 || in_bit == 0))
		return -1;

	int write_status = 0;
	if(output->write_count == BIT_BUFFER_SIZE){
		write_status = write(output->fd, &(output->write_buffer), BYTE);
		output->write_count = 0;
		output->write_buffer = 0;
	}

	/* Bit wise fill the write buffer 				*/
	output->write_count++;
	output->write_buffer |= in_bit << (BIT_BUFFER_SIZE - output->write_count);

	return write_status;
}

void close_file(file *input){

    if(input == NULL) {
        return;
    }

	flush_file(input);

	close(input->fd);

	free(input);

	input = NULL;
	return;
}




/*
   int read_file(file *input, char *ch, size_t count){
   char newbyte;
   int ct = read(input->fd, &newbyte, count);
   if(count == 0){
   return count;
   }
   int n = input->read_count;

// we have to take care of the remaining bit buffer also 

 *ch = ((((1 << n) - 1) & input->read_buffer) << (8 - n)) | 
 (((newbyte & (((1 << (8 - n)) - 1) << n))) >> n);

 input->read_buffer = newbyte & ((1 << n) - 1);

 return ct;
 }
 */
