/*
 * instruction_tests.c
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68)
 * 11/22/2022 
 *
 * File where we write the words for each instruction and build our 
 * .um instruction tests. Implements a series of different tests
 * with different words that hold instructions in order to create 
 * a .um test that can be ran with the ./um executable
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"
#include "bitpack.h"

typedef uint32_t Um_instruction;

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;  

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;


/* adds an instruction to a sequence */ 
static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
} 


/* bit packs a word with 3 registers and an opcode */
Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction our_instruction = 0; 
        our_instruction = Bitpack_newu(our_instruction, 4, 28, op); 
        our_instruction = Bitpack_newu(our_instruction, 3, 6, ra); 
        our_instruction = Bitpack_newu(our_instruction, 3, 3, rb);
        our_instruction = Bitpack_newu(our_instruction, 3, 0, rc);
        return our_instruction;
}  

/* uses bitpack functions to create a load val instruction word */
Um_instruction loadval(unsigned ra, unsigned val) 
{ 
        Um_instruction our_instruction = 0; 
        our_instruction = Bitpack_newu(our_instruction, 4, 28, 13); 
        our_instruction = Bitpack_newu(our_instruction, 3, 25, ra);
        our_instruction = Bitpack_newu(our_instruction, 25, 0, val);
        return our_instruction;
}


/* creates a word with the add instruction */
Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

/* creates a word with the multiply instruction */
Um_instruction multiply(Um_register a, Um_register b, Um_register c) 
{
        return three_register(MUL, a, b, c);
} 

/* creates a word with the divide instruction */
Um_instruction divide(Um_register a, Um_register b, Um_register c) 
{
        return three_register(DIV, a, b, c);
} 

/* creates a word with the nand instruction */
Um_instruction nand(Um_register a, Um_register b, Um_register c) 
{
        return three_register(NAND, a, b, c);
} 

/* creates a word with the output instruction */
Um_instruction output(Um_register c) 
{
        return three_register(OUT, 0, 0, c);
} 

/* creates a word with the input instruction */
Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

/* creates a word with the sload instruction */
Um_instruction sload(Um_register a, Um_register b, Um_register c)
{ 
        return three_register(SLOAD, a, b, c);
}

/* creates a word with the cmov instruction */
Um_instruction cmov(Um_register a, Um_register b, Um_register c)
{ 
        return three_register(CMOV, a, b, c);
}


/* creates a word with the halt instruction */
Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

/* creates a word with the map instruction */
Um_instruction map(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

/* creates a word with the unmap instruction */
Um_instruction unmap(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

/* creates a word with the sstore instruction */
Um_instruction sstore(Um_register a, Um_register b, Um_register c) 
{
        return three_register(SSTORE, a, b, c);
}

/* creates a word with the loadp instruction */
Um_instruction loadp(Um_register b, Um_register c) 
{
        return three_register(LOADP, 0, b, c);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}

/* tests just the halt instruction */
void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

/* 
 * loads values to add together to test the load val and add instruction
 * also tests the output instruction against our expected output 
 */
void build_add_test(Seq_T stream)
{   
        append(stream, loadval(r2, 48));
        append(stream, loadval(r3, 50));
        append(stream, add(r1, r2, r3)); 
        append(stream, output(r1));
        append(stream, halt());
} 

/* 
 * loads values to multiply together to test the load val and multiply 
 * instruction also tests the output instruction against our expected output 
 */
void build_multiply_test(Seq_T stream)
{
        append(stream, loadval(r2, 10)); 
        append(stream, loadval(r3, 5));
        append(stream, multiply(r1, r2, r3));  
        append(stream, output(r1));
        append(stream, halt());
}

/* 
 * loads values to divide to test the load val and divide instruction
 * also tests the output instruction against our expected output 
 */
void build_divide_test(Seq_T stream)
{
        append(stream, loadval(r2, 100)); 
        append(stream, loadval(r3, 2));
        append(stream, divide(r1, r2, r3));  
        append(stream, output(r1));
        append(stream, halt());
}

/* 
 * loads values to nand to test the load val and nand instruction 
 * also tests the output instruction against our expected output 
 */
void build_nand_test(Seq_T stream)
{
        append(stream, loadval(r2, 33554431)); 
        append(stream, loadval(r3, 128));
        append(stream, multiply(r1, r2, r3));
        append(stream, nand(r4, r1, r1));  
        append(stream, output(r4));
        append(stream, halt());
}

/* loads values to output to test the load val and output instruction */
void build_output_test(Seq_T stream)
{
        append(stream, loadval(r2, 98));
        append(stream, loadval(r3, 99));
        append(stream, output(r2));
        append(stream, output(r3));
        append(stream, halt());
}

/* tests input instruction and directly outputs input */
void build_input_test(Seq_T stream)
{
        append(stream, input(r3));
        append(stream, output(r3));
        append(stream, halt());
} 

/* loads and outputs then halts at end */
void build_verbose_halt_test(Seq_T stream)
{
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
        append(stream, halt());
}

/* 
 * loads values and tests map function by then inputs store and load
 * and outputs the result, should be what was stored in 
 */
void build_sload_test(Seq_T stream)
{
        append(stream, loadval(r1, 50));
        append(stream, loadval(r2, 51));
        append(stream, loadval(r3, 52));
        append(stream, map(r4, r1));
        append(stream, loadval(r5, 1));
        append(stream, sstore(r5, r5, r2));
        append(stream, sload(r3, r5, r5));
        append(stream, output(r3));
        append(stream, halt());
}

/* loads values and tests map function then immediately halts */
void build_map_test(Seq_T stream)
{
        append(stream, loadval(r1, 3));
        append(stream, loadval(r2, 4));
        append(stream, loadval(r3, 5));
        append(stream, map(r2, r3));
        append(stream, halt());
}

/* 
 * loads values and tests map function, then immediately unmaps
 * then halts 
 */
void build_unmap_test(Seq_T stream)
{
        append(stream, loadval(r2, 3));
        append(stream, loadval(r3, 1));
        append(stream, map(r2, r3));
        append(stream, unmap(r3));
        append(stream, halt());
}

/* 
 * loads values and tests map function, then unmaps, and immediately
 * maps after to test that it can and goes to the right index then halts 
 */
void build_recycle_test(Seq_T stream)
{
        append(stream, loadval(r2, 3));
        append(stream, loadval(r3, 1));
        append(stream, map(r2, r3));
        append(stream, unmap(r3));
        append(stream, map(r2, r3));
        append(stream, halt());
}

/* repeatitively loads vals and maps and unmaps */
void build_recycle_test2(Seq_T stream)
{
        append(stream, loadval(r2, 3));
        append(stream, loadval(r3, 1));
        append(stream, map(r2, r3));
        append(stream, unmap(r3));
        
        append(stream, loadval(r4, 4));
        append(stream, loadval(r5, 8));
        append(stream, map(r4, r5));
        append(stream, loadval(r5, 1));
        append(stream, unmap(r5));

        append(stream, loadval(r6, 6));
        append(stream, loadval(r7, 3));
        append(stream, map(r6, r7));
        append(stream, loadval(r7, 1));
        append(stream, unmap(r7));

        append(stream, map(r6, r2));

        append(stream, halt());
}

/* maps then loads a value into a word using sstore then halts */
void build_sstore_test(Seq_T stream) 
{
        append(stream, loadval(r3, 5));
        append(stream, loadval(r2, 3));
        append(stream, loadval(r1, 1));
        append(stream, loadval(r4, 2));

        append(stream, map(r2, r3));

        append(stream, sstore(r1, r4, r3));

        append(stream, halt());
}

/* 
 * appends values then calls cmov on those values and outputs both
 * register A and B to see if they are the same 
 */
void build_cmov_test1(Seq_T stream)
{ 
        append(stream, loadval(r3, 80));
        append(stream, loadval(r2, 81));
        append(stream, loadval(r1, 82));
        append(stream, cmov(r1, r2, r3));
        append(stream, output(r2));
        append(stream, output(r1));
        append(stream, halt());
}

/* calls cmov when register C is 0 */
void build_cmov_test2(Seq_T stream)
{ 
        append(stream, loadval(r3, 0));
        append(stream, loadval(r2, 81));
        append(stream, loadval(r1, 82));
        append(stream, cmov(r1, r2, r3));
        append(stream, output(r2));
        append(stream, output(r1));
        append(stream, halt());
}

/* tests load programs */
void build_load_program_test(Seq_T stream) 
{
        append(stream, loadval(r1, 1));
        append(stream, loadval(r2, 1));
        append(stream, map(r1, r2));
       
        append(stream, loadval(r6, 0));

        append(stream, loadval(r5, 12));
        append(stream, loadval(r3, 50));
        append(stream, loadval(r7, 51));
        append(stream, sload(r4, r6, r5));
        append(stream, sstore(r1, r6, r4)); 

        append(stream, loadp(r2, r6));
        append(stream, output(r3)); 
        append(stream, output(r7));
        append(stream, halt());
} 

/* tests load program */
void build_load_program_test2(Seq_T stream) 
{
        append(stream, loadval(r1, 1));
        append(stream, loadval(r2, 1));
        append(stream, map(r1, r2));
       
        append(stream, loadval(r6, 0));
        
        append(stream, loadval(r5, 10));
        append(stream, sload(r4, r6, r5));
        append(stream, sstore(r1, r6, r4)); 

        append(stream, loadval(r3, 6));
        append(stream, loadp(r2, r6));
        append(stream, cmov(r7, r3, r2));
        append(stream, halt());
} 

/* tests with multiple stores */
void build_many_sstore_test(Seq_T stream) 
{
        append(stream, loadval(r3, 5));
        append(stream, loadval(r2, 3));
        append(stream, loadval(r1, 1));
        append(stream, loadval(r4, 2));
        append(stream, loadval(r6, 0));

        append(stream, map(r2, r3));

        append(stream, sstore(r1, r4, r3));
        
        append(stream, loadval(r7, 3));
        append(stream, map(r4, r7)); 
        append(stream, sstore(r4, r1, r3));
        append(stream, sstore(r4, r4, r2));
        append(stream, halt());
}

/*  tests multiple stores and loads */
void build_sload_test2(Seq_T stream)
{
        append(stream, loadval(r1, 90));
        append(stream, loadval(r2, 91));
        append(stream, loadval(r3, 92));
        append(stream, map(r4, r1));
        append(stream, loadval(r5, 1));
        append(stream, sstore(r5, r5, r2));
        append(stream, sload(r3, r5, r5));
        append(stream, output(r3));
        append(stream, loadval(r6, 0)); 
        append(stream, sstore(r5, r6, r2));
        append(stream, sload(r1, r5, r6));
        append(stream, output(r1));
        append(stream, halt());

}