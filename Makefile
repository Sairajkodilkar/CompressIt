

INCLUDE = -I include/

vpath %.c huffman
vpath %.c tools

testio: encoder.c io.c testio.c heap.c canonical.c huffmanio.c  Makefile
	cc $(INCLUDE) -Wall -g3 tools/io.c huffman/encoder.c huffman/heap.c huffman/canonical.c huffman/huffmanio.c main.c -o test
