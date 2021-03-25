#ifndef __LZW_H
#define __LZW_H

#include "dict.h"
#include <limits.h>

typedef uint16_t lzw_codetype;

#define LZWCODE_MAX UINT32_MAX

long insert_chars(dict *code_dict);

long lzw_encoder(file *infile, file *outfile);

#endif
