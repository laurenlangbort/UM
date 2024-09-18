/*
 * execution_cycle.c
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68)
 * 11/22/2022 
 *
 * Implementation of our driver program execution_cycle. 
 * Holds the driver function (loop) that executes the program. 
 * Hids from um.c file how exactly the program works. 
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
#include "execution_cycle.h"
#include "bitpack.h" 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 

#define opcode_length 4 
#define opcode_lsb 28
#define POWER32 4294967296
#define register_length 3
#define rC_lsb 0 
#define rB_lsb 3 
#define rA_lsb 6 
#define value_length 25 
#define value_lsb 0 
#define width_max 32 
#define end_file_char -1
#define byte_length 4 
#define bit_width 8 
#define msb_lsb 24


typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode; 

struct registers {
        uint32_t *array;
}; 

struct recycle { 
        uint32_t *recycled_ids;
        uint32_t recycle_length;
};

/*
 * title: driver
 * parameters: pointer to the .um file and a filename
 * effects: goes through the segment 0 created with the program code/
 *          instructions and calls certain functions based on what instruction
 *          is being passed in. Uses a program counter to increment from one
 *          word to the next word in segment 0. Uses bitpack get to get the
 *          opcode from the word which decides which instruction to preform.
 * returns: nothing
 * errors detected: none
 * errors not detected:
 * notes: CRE if the file being passed in is NULL/empty, CRE if the memory, 
 *        registers, or the recycle sequence is null
 */
void driver(FILE *fp, char *filename)
{      
        assert(fp != NULL);

        /* instantiating memory with segment 0 */
        //Um_memory UM = create_segment0(fp, filename); 
        struct stat sb;  
        stat(filename, &sb); 

        assert (stat(filename, &sb) != end_file_char);

        /* used to get the amount of chars in the file */
        int size_of_file = (sb.st_size / byte_length); 
        Um_memory UM = memory_new(16 * size_of_file);
        addseg(UM, size_of_file);  
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
                        changeWord(UM, 0, word_index, 
                                  (uint32_t) word); 
                        word_index++; 
                }
        } 
        
        assert(UM != NULL);
       
        /* instantiating registers */
        registers gp_registers = make_registers();
        assert(gp_registers != NULL);

        /* instantiating our queue or recycled ID */
     
        recycle recycled_index = (recycle)malloc(sizeof(*recycled_index));
        assert (recycled_index != NULL);
        recycled_index->recycled_ids = (uint32_t*)calloc(size_of_file, sizeof(uint32_t));
        assert(recycled_index->recycled_ids != NULL); 
        recycled_index->recycle_length = 0;
    
        /* setting variables */
        uint32_t program_counter = 0;
        seg_info segment0 = getSeg(UM, 0);
        uint32_t *words = getSeg_array(segment0);
        uint32_t word = words[program_counter];
        uint32_t op_code = 0; 
        uint32_t rec_counter = 0; 
        uint32_t rem_counter = 0;
        uint32_t size = size_of_file;
        
        /* loops until halt instruction */
        while (op_code != 7) 
        {       
                /* get the op code from the word */
                unsigned hiOPCODE = opcode_lsb + opcode_length;
                op_code = (word << (32  - hiOPCODE)) >> (32 - opcode_length);
                /* do operation based on op code */
                if (op_code == LOADP) {
                        unsigned hiB = rB_lsb + register_length;
                        uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length);         
                        unsigned hiC = rC_lsb + register_length;
                        uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);   

                        /* checks that r[B] is not zero so that segment 0 doesn't duplicate 
                        itself */
                        if (gp_registers->array[r_B] != 0) {
                                
                                /* gets the segement info of the segment needed to duplicate */
                                seg_info rB_segment = getSeg(UM, gp_registers->array[r_B]);

                                /* duplicates the segment and removes old 0 segment */
                                seg_info duplicated_seg = duplicateSeg(rB_segment);
                                uint32_t replace = remSeg(UM, 0); 
                                changeSeg(UM, replace, duplicated_seg); 
                        } 
                        
                        /* reassigns the program counter */
                        program_counter = gp_registers->array[r_C];   
                
                        segment0 = getSeg(UM, 0); 
                        words = getSeg_array(segment0);
                } else {
                        if (op_code == CMOV) {
                                unsigned hi = rC_lsb + register_length; 
                                uint32_t r_C = (word << (32 - hi)) >> (32 - register_length); 


                                 /* sets value of register A to value of register B */
                                if (gp_registers->array[r_C] != 0) { 
                                        unsigned hiA = rA_lsb + register_length; 
                                        uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
                                        unsigned hiB = rB_lsb + register_length;
                                        uint32_t r_B = (word << (32 - hiB))>> (32- register_length); 
                                        gp_registers->array[r_A] = gp_registers->array[r_B];
                                }
                               // cmov(word, gp_registers);
                        } else if (op_code == SLOAD) {
                                unsigned hiA = rA_lsb + register_length; 
                                uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
                                unsigned hiB = rB_lsb + register_length;
                                uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length); 
                                unsigned hiC = rC_lsb + register_length;
                                uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);
                                /* sets value of register A to a uint32_t word */
                                seg_info get_seg = getSeg(UM, gp_registers->array[r_B]);
                                uint32_t *words = getSeg_array(get_seg); 
                                gp_registers->array[r_A] = words[gp_registers->array[r_C]];
                                // sload(word, UM, gp_registers);
                        } else if (op_code == SSTORE) { 
                                unsigned hiA = rA_lsb + register_length; 
                                uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length);
                                unsigned hiB = rB_lsb + register_length; 
                                uint32_t r_B = (word << (32 - hiB)) >> (32 - register_length);
                                unsigned hiC = rC_lsb + register_length; 
                                uint32_t r_C = (word << (32 - hiC)) >> (32 - register_length);

                                /* changes word at a certain index in memory */
                                seg_info change_seg = getSeg(UM, gp_registers->array[r_A]);
                                uint32_t *seg_array = getSeg_array(change_seg);
                                seg_array[gp_registers->array[r_B]] = gp_registers->array[r_C];
                                // sstore(word, UM, gp_registers);
                        } else if (op_code == ADD) { 
                                // add(word, gp_registers);
                                unsigned hiA = rA_lsb + register_length; 
                                uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
                                unsigned hiB = rB_lsb + register_length;
                                uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length); 
                                unsigned hiC = rC_lsb + register_length;
                                uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);
                                
                                /* adds and sets register A to sum */
                                uint32_t new_value = (gp_registers->array[r_B] + 
                                                gp_registers->array[r_C]) % POWER32; 
                                gp_registers->array[r_A] = new_value; 
                        } else if (op_code == MUL) {
                                unsigned hiA = rA_lsb + register_length; 
                                uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
                                unsigned hiB = rB_lsb + register_length;
                                uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length); 
                                unsigned hiC = rC_lsb + register_length;
                                uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);

                                /* take the product */
                                uint32_t new_value = (gp_registers->array[r_B] *
                                                gp_registers->array[r_C]) % POWER32;

                                /* make sure the product fits */        
                                gp_registers->array[r_A] = new_value; 
                        } else if (op_code == DIV) {
                                unsigned hiA = rA_lsb + register_length; 
                                uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
                                unsigned hiB = rB_lsb + register_length;
                                uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length); 
                                unsigned hiC = rC_lsb + register_length;
                                uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);        
                                /* sets register A */
                                gp_registers->array[r_A] = gp_registers->array[r_B] / 
                                                        gp_registers->array[r_C]; 
                        } else if (op_code == NAND) {
                               // nand(word, gp_registers);
                                 /* get register values from the word */
                                unsigned hiA = rA_lsb + register_length; 
                                uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
                                unsigned hiB = rB_lsb + register_length;
                                uint32_t r_B = (word << (32  - hiB)) >>(32 - register_length); 
                                unsigned hiC = rC_lsb + register_length;
                                uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);        
                                /* preforms bit operations */
                                gp_registers->array[r_A] = ~(gp_registers->array[r_B] & 
                                      gp_registers->array[r_C]);
                        } else if (op_code == HALT) { 
                                
                                /* free recycled ID sequence AND our_register*/ 
                                free_registers(gp_registers); 
                                free(recycled_index->recycled_ids);
                                free(recycled_index); 
                                memory_free(UM);
                                fclose(fp);
                                break;
                        } else if (op_code == ACTIVATE) {
                                unsigned hiB = rB_lsb + register_length;
                                uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length); 
                                unsigned hiC = rC_lsb + register_length;
                                uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);        
                                /* if there is an unmapped segment */
                                if (recycled_index->recycle_length != 0) { 
                
                                        /* gets the segment id from the recycled sequence */
                                        uint32_t recycled_id = recycled_index->recycled_ids[rem_counter];
                 
                                      //  fprintf(stderr, "rem_counter = %d \n", rem_counter);
                                        /* gets the segment information at that id */
                                        seg_info new_segment = getSeg(UM, recycled_id);
                                        uint32_t *words = (uint32_t*)calloc(gp_registers->array[r_C],
                                                           sizeof(uint32_t)); 
                                        int length = gp_registers->array[r_C];
                                        new_segment = set_segInfo(length, words);
                
                                        /* changes the unmapped segment, and removes segment index 
                                        from recycle sequence */
                                        changeSeg(, recycled_id, new_segment);
                                        recycled_index->recycled_ids[rem_counter] = 0;
                                        rem_counter++;
                                        recycled_index->recycle_length--;
                                        gp_registers->array[r_B] = recycled_id;

                                        /* if there is not an unmapped segment */
                                } else {
                                        addseg(UM, gp_registers->array[r_C]); 
                                        gp_registers->array[r_B] = memory->current_length - 1;
                                }
                                //map(word, UM, recycle, gp_registers);
                        } else if (op_code == INACTIVATE) { 
                                unsigned hiC = rC_lsb + register_length;
                                uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);        
                                /* frees the memory of the array and adds to recycle sequence */
                                remSeg(UM, gp_registers->array[r_C]); 
                                
                                /* adds the unmaped segment index to a sequence */
                                void *temp;
                                if (rec_counter == size) { 
                                        temp = realloc(recycled_index->recycled_ids, size * 2 * 4); 
                                        assert(temp != NULL); 
                                        recycled_index->recycled_ids = temp;
                                        size = size * 2;
                                }  
                                
                                recycled_index->recycled_ids[rec_counter] = gp_registers->array[r_C]; 
                                recycled_index->recycle_length++;
                                rec_counter++;
                               // Seq_addhi(recycle, (void *)(uintptr_t) gp_registers->array[r_C]); 
                        } else if (op_code == OUT) {
                                unsigned hiC = rC_lsb + register_length;
                                uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);    

                                /* prints to stdout as a character */
                                printf("%c", gp_registers->array[r_C]);
                        } else if (op_code == IN) {
                                unsigned hiC = rC_lsb + register_length;
                                uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);   
                                char c = getchar();
                                
                                /* if it gets the end file character */
                                if (c == EOF) {
                                        uint32_t end_of_file = ~0; 
                                        gp_registers->array[r_C] = end_of_file;
                                } else {
                                        gp_registers->array[r_C] = c;
                                }
                        } else if (op_code == LV) { 
                                /* gets the register index from the word */

                                unsigned hiA = value_length + register_length; 
                                uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
                                unsigned hiValue = value_lsb + value_length; 
                                uint32_t value = (word << (32 - hiValue)) >> (32 - value_length); 

                                /* sets the register index to a certain value */ 
                                gp_registers->array[r_A] = value;
                                //load_value (word, gp_registers);
                        }
        
                        /* increment program counter */ 
                        (program_counter)++;
                        
                }
                
                /* readjust how we get specific word from program counter */
                word = words[program_counter];
        }

}