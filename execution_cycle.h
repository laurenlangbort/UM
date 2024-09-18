/*
 * execution_cycle.h
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68)
 * 11/22/2022 
 *
 * Interface of execution cycle. Holds the driver function that runs
 * the instructions
 */


#include "assert.h"
#include "mem.h"
#include "seq.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "um_read.h"
#include "um_memory.h" 
#include "instructions.h"
#include "bitpack.h" 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct recycle *recycle;
void driver(FILE *fp, char *filename);
