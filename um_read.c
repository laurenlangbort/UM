/*
 * um_read.c
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68)
 * 11/22/2022
 *
 * Implementation of our um_read module. Uses Um_memory module to 
 * initialize an instance of memory with a segment 0 that is filled with
 * words from a .um file. The purpose of this is to hide how we read 
 * in a file and instantiate segment 0 from our driver module that
 * runs on segment 0. 
 *
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
#include "bitpack.h" 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 

#define end_file_char -1
#define byte_length 4 
#define bit_width 8 
#define msb_lsb 24

/*
 * title: create_segment0
 * parameters: .um file and the filename
 * effects: creates an instance of Um_memory with a filled segment 0, 
 *          containing the instructions/words of a 32 bit word read
 *          in by 4 characters at a time in big endian order
 * returns: Um_memory with filled segment 0
 * errors detected: none
 * errors not detected: none
 * notes: asserts that the file is not null/empty and assertd if the stat
 *        is read in wrong
 */
Um_memory create_segment0(FILE *fp, char *filename) 
{
        assert(fp != NULL); 
        struct stat sb;  
        stat(filename, &sb); 

        assert (stat(filename, &sb) != end_file_char);

        /* used to get the amount of chars in the file */
        int size_of_file = (sb.st_size / byte_length); 
        Um_memory initialized_memory = memory_new(12 * size_of_file);
        addseg(initialized_memory, size_of_file);  
        int word_index = 0;

        /* reads in the file and bitpacks each word */  
        while (feof(fp) == 0) {  
                uint64_t word = 0;
                int byte = 0; 
                int b = msb_lsb; 

                /* gets the characters and bitpacks it into new word */
                while (b >= 0 && byte != end_file_char) {
                        byte = getc(fp);
                        if (byte != end_file_char) {  
                                unsigned hi = b + bit_width;
                                uint32_t save = word;
                                word = save >> hi << hi;
                                uint32_t second_part = save << (64 - b) >> (64 - b);
                                word = word | second_part | (byte << b);
                                
                              // word = Bitpack_newu(word, bit_width, b, byte);  
                        }  
                        if (byte == end_file_char && b != msb_lsb) { 
                                assert(feof(fp) == 0); 
                        }
                        b = b - bit_width;
                }

                /* adds the word to the segment */
                if (feof(fp) == 0) { 
                        changeWord(initialized_memory, 0, word_index, 
                                  (uint32_t) word); 
                        word_index++; 
                }
        } 
        return initialized_memory;
}