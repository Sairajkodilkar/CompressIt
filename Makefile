SOURCE = $(wildcard main.c huffman/*.c fileio/*.c LZW/*.c error/*.c)

OBJ = $(subst .c,.o,$(SOURCE))


HEADER_I = huffman/include/ fileio/include/ LZW/include/ error/include/

HEADER = $(wildcard $(addsuffix *.h,$(HEADER_I)))


CC_FLAG = $(addprefix -I ,$(HEADER_I))

all: compress

compress : $(OBJ)
	$(CC) $(CC_FLAG) $(OBJ) -o compress

%.o:%.c $(HEADER)
	$(CC) $(CC_FLAG) $< -c -o $@


.PHONY: clean

clean:
	-rm $(OBJ)

