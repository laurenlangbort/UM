/*
 * instructions.c
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68)
 * 11/22/2022 
 *
 * Implementation of our instructions module. 
 * Instantiates our representation of registers as a C array
 * and contains functions that complete the 14 UM operations 
 * 
 */

#include "assert.h"
#include "mem.h"
#include "seq.h"
#include "instructions.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define POWER32 4294967296
#define register_length 3
#define rC_lsb 0 
#define rB_lsb 3 
#define rA_lsb 6 
#define value_length 25 
#define value_lsb 0 
#define width_max 32 

/*
 * struct title: registers
 * holds: an array with 32 bit ints
 * effects: initializes a structure where the register values can be held
 */
struct registers {
        uint32_t *array;
}; 

/*
 * title: make_registers()
 * parameters: none
 * effects: creates space on the heap for the registers structs and the values
 *          inside each register
 * returns: our array of registers
 * errors detected: none
 * errors not detected: none
 * notes: checked run-time error if memory allocated for registers struct is 
 *        not null and checked run-time error if memory allocated for register
 *        values is not null
 */
registers make_registers() 
{
        /* allocates space for the register struct */ 
        registers gp_registers = (registers)malloc(sizeof(registers));
        assert(gp_registers != NULL);

         /* allocates space for the register values */ 
        gp_registers->array = (uint32_t *)calloc(8, sizeof(uint32_t));
        assert(gp_registers->array != NULL);
        return gp_registers;
}

/*
 * title: free_registers
 * parameters: the array representation of registers
 * effects: frees memory allocated for registers 
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: calls free on both array and struct
 */
void free_registers(registers gp_registers)
{
        /* free the register values malloced */
        free(gp_registers->array);
        
        /* free the register struct itself */
        free(gp_registers);
} 


/*
 * title: cmov
 * parameters: a 32 bit word, and the array of registers
 * effects: sets the value of the Ath register equal to the value of the Bth
 *          register, if the index for register C is not 0 
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: assertions for the getting the word are made in bitpack
 */
void cmov(uint32_t word, registers gp_registers) 
{ 
        assert(gp_registers != NULL);

        /* gets the register number */
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
}

/*
 * title: sload (segmented load)
 * parameters: a 32 bit word, our instance of memory, the array of registers
 * effects: get the Cth word of the Bth segment and put it in the Ath 
 *          register
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: assertions for the getting the word are made in bitpack, assertion
 *        for getting word from memory is made in getWord
 */
void sload(uint32_t word, Um_memory memory, registers gp_registers) 
{
        assert(gp_registers != NULL);

        /* gets the register indexes  */

        unsigned hiA = rA_lsb + register_length; 
        uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
        unsigned hiB = rB_lsb + register_length;
        uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length); 
        unsigned hiC = rC_lsb + register_length;
        uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);
        /* sets value of register A to a uint32_t word */
	seg_info get_seg = getSeg(memory, gp_registers->array[r_B]);
        uint32_t *words = getSeg_array(get_seg); 
        gp_registers->array[r_A] = words[gp_registers->array[r_C]];
        //gp_registers->array[r_A] = getWord(memory, gp_registers->array[r_B], 
                                           //gp_registers->array[r_C]);

}

/*
 * title: sstore (segmented store)
 * parameters: a 32 bit word, our instance of memory, the array of registers
 * effects: changes the word at the rA-th segment and rB-th offset to
 *          the value in the C-th register
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: assertions for the getting the word are made in bitpack, and 
 *        assertion for the bounds of the word wanted to change is made
 *        in changeWord
 */
void sstore(uint32_t word, Um_memory memory, registers gp_registers) 
{
        assert(gp_registers != NULL);
        assert(memory != NULL);

        /* gets the register indexes */
        unsigned hiA = rA_lsb + register_length; 
        uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length);
        unsigned hiB = rB_lsb + register_length; 
        uint32_t r_B = (word << (32 - hiB)) >> (32 - register_length);
	unsigned hiC = rC_lsb + register_length; 
        uint32_t r_C = (word << (32 - hiC)) >> (32 - register_length);

        /* changes word at a certain index in memory */
        seg_info change_seg = getSeg(memory, gp_registers->array[r_A]);
        assert(change_seg != NULL);
        uint32_t *seg_array = getSeg_array(change_seg);
        seg_array[gp_registers->array[r_B]] = gp_registers->array[r_C];
}

/*
 * title: add
 * parameters: a 32 bit word, and the array of registers
 * effects: adds the values of register B and register C and sets it to the
 *          value of register A
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: asserts if the sum can fit in a 32 bit word. 
 *        assertions for the getting the word are made in bitpack
 */
void add(uint32_t word, registers gp_registers)
{ 
        assert(gp_registers != NULL);

        /* gets the register number */
        unsigned hiA = rA_lsb + register_length; 
        uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
        unsigned hiB = rB_lsb + register_length;
        uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length); 
        unsigned hiC = rC_lsb + register_length;
        uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);
        
        /* adds and sets register A to sum */
        uint32_t new_value = (gp_registers->array[r_B] + 
                              gp_registers->array[r_C]) % POWER32;
        assert(Bitpack_fitsu(new_value, width_max) == 1); 
        gp_registers->array[r_A] = new_value; 
}


/*
 * title: multiply
 * parameters: a 32 bit word, and the array of registers
 * effects: multiplies the values of register B and register C and 
 *          sets it to the value of register A 
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: asserts if the product can fit in a 32 bit word 
 *        assertions for the getting the word are made in bitpack
 */
void multiply(uint32_t word, registers gp_registers) 
{
        assert(gp_registers != NULL);

        /* get register values from the word */
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
        assert(Bitpack_fitsu(new_value, width_max) == 1); 
        gp_registers->array[r_A] = new_value; 
} 

/*
 * title: divide
 * parameters: a 32 bit word, and the array of registers
 * effects: divides the values of register B and register C and 
 *          sets it to the value of register A 
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: assertions for the getting the word are made in bitpack
 */
void divide(uint32_t word, registers gp_registers)
{
        assert(gp_registers != NULL);

        /* get register values from the word */
        unsigned hiA = rA_lsb + register_length; 
        uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
        unsigned hiB = rB_lsb + register_length;
        uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length); 
        unsigned hiC = rC_lsb + register_length;
        uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);        
        /* sets register A */
        gp_registers->array[r_A] = gp_registers->array[r_B] / 
                                   gp_registers->array[r_C]; 
}

/*
 * title: nand
 * parameters: a 32 bit word, and the array of registers
 * effects: sets register A equal to the value of register A and register
 *          B anded together than negated
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: assertions for the getting the word are made in bitpack
 */
void nand(uint32_t word, registers gp_registers)
{
        assert(gp_registers != NULL);

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

}

/*
 * title: halt
 * parameters: a 32 bit word, the array of registers, and memory with an 
 *             Um_memory type
 * effects: frees data structures and halts/exits computation 
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: 
 */
void halt(Um_memory memory)
{ 
        assert(memory != NULL);

        /* free memory and exit with success */ 
        memory_free(memory);
}

/*
 * title: map
 * parameters: 32 bit word, an Um_memory memory, and the sequence with the 
 *             unmapped values
 * effects: maps a new segment, if there is a recycled index, it will map to
 *          that recycled index 
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: assertions for the getting the word are made in bitpack, and for 
 *        pointer values being passed into the function
 */
void map(uint32_t word, Um_memory memory, Seq_T recycle, 
         registers gp_registers)
{
        assert(gp_registers != NULL);
        assert(memory != NULL); 
        assert(recycle != NULL); 

        /* get register values from the word */

        unsigned hiB = rB_lsb + register_length;
        uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length); 
        unsigned hiC = rC_lsb + register_length;
        uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);        
        /* if there is an unmapped segment */
        if (Seq_length(recycle) != 0) { 
                
                /* gets the segment id from the recycled sequence */
                uint32_t recycled_id = (uint32_t)(uintptr_t)
                                       Seq_get(recycle, 0);
                
                /* gets the segment information at that id */
                seg_info new_segment = getSeg(memory, recycled_id);
                uint32_t *words = (uint32_t*)calloc(gp_registers->array[r_C],
                                 sizeof(uint32_t)); 
                int length = gp_registers->array[r_C];
                new_segment = set_segInfo(length, words);
                
                /* changes the unmapped segment, and removes segment index 
                from recycle sequence */
                changeSeg(memory, recycled_id, new_segment);
                Seq_remlo(recycle); 
                gp_registers->array[r_B] = recycled_id;

        /* if there is not an unmapped segment */
        } else {
                addseg(memory, gp_registers->array[r_C]); 
                gp_registers->array[r_B] = memory_length(memory) - 1;
        }
} 

/*
 * title: unmap
 * parameters: a 32 bit word, our representation of memory, and our sequence 
 *             of recycled IDs
 * effects: removes a segment from our memory and puts the ID into 
 *          a sequence of recycled IDs  
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: assertions for the getting the word are made in bitpack and 
 *        asserts that the pointer values being passed in are not null/empty
 */
void unmap(uint32_t word, Um_memory memory, Seq_T recycle, 
           registers gp_registers) 
{ 
        assert(gp_registers != NULL); 
        assert(memory != NULL); 
        assert(recycle != NULL);

        /* gets the register index from the word */

        unsigned hiC = rC_lsb + register_length;
        uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);        
        /* frees the memory of the array and adds to recycle sequence */
        remSeg(memory, gp_registers->array[r_C]); 
        
        /* adds the unmaped segment index to a sequence */
        Seq_addhi(recycle, (void *)(uintptr_t) gp_registers->array[r_C]);  
}

/*
 * title: output
 * parameters: a 32 bit word, our representation of registers
 * effects: outputs the value at a specific register to the IO stream
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: asserts if the value is not within 0 - 255 so it cannot be a 
 *        character, assertions for getting the value are made in bitpack,
 *        asserts that the registers are not null
 */
void output(uint32_t word, registers gp_registers)
{ 
        assert (gp_registers != NULL);
        
        /* gets the register index */
        unsigned hiC = rC_lsb + register_length;
        uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);    
        assert(gp_registers->array[r_C] <= 255); 

        /* prints to stdout as a character */
        printf("%c", gp_registers->array[r_C]);
}

/*
 * title: input
 * parameters: a 32 bit word, our representation of our registers
 * effects: expects an input from standard input and store it in 
 *          a specific register 
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: asserts the value of input is between 0 and 255 &
 *        assertions for the getting the word are made in bitpack
 */
void input(uint32_t word, registers gp_registers)
{ 
        assert (gp_registers != NULL);
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
}

/*
 * title: load program
 * parameters: a Um_memory memory, a 32 bit word, a 32 bit array of the
 *             registers, the program counter, and sequence of the unmapped
 *             segments
 * effects: expects an input from standard input and store it in 
 *          a specific register 
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: asserts the value of input is between - and 255
 *        assertions for the getting the word are made in bitpack
 */
void load_program(Um_memory memory, uint32_t word, registers gp_registers, 
                  uint32_t *program_counter)
{
        assert(memory != NULL);
        assert(gp_registers != NULL);
        /* gets the indices of register B and register C */

        unsigned hiB = rB_lsb + register_length;
        uint32_t r_B = (word << (32  - hiB)) >> (32 - register_length);         
        unsigned hiC = rC_lsb + register_length;
        uint32_t r_C = (word << (32  - hiC)) >> (32 - register_length);   

        /* checks that r[B] is not zero so that segment 0 doesn't duplicate 
        itself */
        if (gp_registers->array[r_B] != 0) {
                
                /* gets the segement info of the segment needed to duplicate */
                seg_info rB_segment = getSeg(memory, gp_registers->array[r_B]);

                /* duplicates the segment and removes old 0 segment */
                seg_info duplicated_seg = duplicateSeg(rB_segment);
                uint32_t replace = remSeg(memory, 0); 
                changeSeg(memory, replace, duplicated_seg); 
        } 
        
        /* reassigns the program counter */
        *program_counter = gp_registers->array[r_C];   
}

/*
 * title: load_value
 * parameters: a 32 bit word, our representation of our registers
 * effects: load a specific value into a specific register
 * returns: nothing
 * errors detected: none
 * errors not detected: none
 * notes: assertions for the getting the word are made in bitpack and 
 *        assertions for pointers being passed in
 */
void load_value(uint32_t word, registers gp_registers)
{
        assert(gp_registers != NULL);

        /* gets the register index from the word */

        unsigned hiA = value_length + register_length; 
        uint32_t r_A = (word << (32 - hiA)) >> (32 - register_length); 
        unsigned hiValue = value_lsb + value_length; 
        uint32_t value = (word << (32 - hiValue)) >> (32 - value_length); 

        /* sets the register index to a certain value */ 
        gp_registers->array[r_A] = value;
}  

