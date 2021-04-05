SOURCE = $(wildcard main.c huffman/*.c fileio/*.c LZW/*.c error/*.c)

OBJ = $(subst .c,.o,$(SOURCE))

HEADER_I = huffman/include/ fileio/include/ LZW/include/ error/include/

HEADER = $(wildcard $(addsuffix *.h,$(HEADER_I)))

I_FLAG = $(addprefix -I ,$(HEADER_I))
C_FLAG = -Wall -g3 
#MEM_FLAG = -fsanitize=address

EXE = compress

all: compress

compress : $(OBJ)
	$(CC) $(I_FLAG) $(C_FLAG) $(MEM_FLAG) $(OBJ) -o $(EXE)

%.o:%.c $(HEADER) Makefile
	$(CC) $(I_FLAG) $(C_FLAG) $< -c -o $@

.PHONY: clean

clean:
	-rm $(OBJ)

