

INCLUDE = -I ../include/

testio: io.c testio.c
	cc $(INCLUDE) -Wall io.c testio.c -o testio
