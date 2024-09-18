/*
 * um_read.h
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68)
 * 11/22/2022 
 *
 * Interface of our um_read module. Allows our driver program to create
 * a representation of memory with an already filled segment 0 without 
 * having to know how exactly it was implemented.  
 *
 */

#include "assert.h"
#include "mem.h"
#include "seq.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bitpack.h"
#include "um_memory.h"

extern Um_memory create_segment0(FILE *fp, char *filename);
