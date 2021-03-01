#include <stdio.h>

void raise(const char *str, char **argv){
	if(argv == NULL){
		fprintf(stderr, "%s\n", str);
		return;
	}
	fprintf(stderr, "%s: %s\n", argv[0], str);
	return;
}
