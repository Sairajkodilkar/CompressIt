#include <stdio.h>

#include "io.h"

#include <fcntl.h>
#include <sys/types.h>

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

	if(fd = -1){
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

/* Read functions 		*/

int read_file(file *input, char *ch, size_t count){
	return read(input->fd, ch, count);
}


/* reads the header in compressed file */
int read_binary_header(file *input){
}

bit read_bit(file *input){

}





/* Write functions 			*/

int write_file(file *input, char *ch, size_t count){
	return write(input->fd, ch, count);
}

/* write the header to header file */
int write_binary_header(file *output){

}
/* writes to the file bit by bit */
int write_bit(file *output, bit in_bit){
}


void close_file(file *input){
}



