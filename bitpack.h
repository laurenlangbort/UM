#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include "except.h"
#include "assert.h"

bool Bitpack_fitss(int64_t n, unsigned width); 
bool Bitpack_fitsu(uint64_t n, unsigned width); 
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb); 
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb); 
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value);
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                      int64_t value);
