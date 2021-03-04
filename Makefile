INCLUDE = -I include/

vpath %.c huffman
vpath %.c tools
vpath %.o bin

BIN = bin
SOURCES = canonical.c decoder.c encoder.c heap.c huffman.c huffmanio.c err.c io.c main.c
HEADER = canonical.h err.h heap.h huff.h huffman.h io.h
CC_FLAG = -I include/

OBJECT = $(subst .c,.o, $(SOURCES))


all: bin compress 


compress : $(OBJECT)
	$(CC) $(CC_FLAG) $^ -o $@

%.o: %.c
	$(CC) $(CC_FLAG) $(DEBUG) $< -c -o $@

.PHONY: clean

movobj: bin 
	-mv *.o bin/

bin:
	-mkdir bin

clean:
	-rm ./bin/*.o
	-rm *.o 



