

INCLUDE = -I include/

vpath %.c huffman
vpath %.c tools

testio: encoder.c io.c testio.c heap.c
	cc $(INCLUDE) -Wall tools/io.c huffman/encoder.c huffman/heap.c testio.c -o testio
