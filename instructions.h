/*
 * instructions.h
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68)
 * 11/22/2022 
 *
 * Interface of our instructions module.
 * Hides how exactly we implement our representiation of registers 
 * Allows our driver program to access these functions without knowing
 * how they were implemented
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

#ifndef INSTRUCTIONS_H 
#define INSTRUCTIONS_H 

typedef struct registers *registers;

extern registers make_registers(); 
extern void free_registers(registers gp_registers);
extern void cmov(uint32_t word, registers gp_registers);
extern void sload(uint32_t word, Um_memory memory, registers gp_registers); 
extern void sstore(uint32_t word, Um_memory memory, registers gp_registers);
extern void add(uint32_t word, registers gp_registers);
extern void multiply(uint32_t word, registers our_registers);
extern void divide(uint32_t word, registers gp_registers);
extern void nand(uint32_t word, registers gp_registers);
extern void halt(Um_memory memory);
extern void map(uint32_t word, Um_memory memory, Seq_T recycle, 
                registers gp_registers);
extern void unmap(uint32_t word, Um_memory memory, Seq_T recycle, 
                  registers gp_registers);
extern void output(uint32_t word, registers gp_registers);
extern void input(uint32_t word, registers gp_registers);
void load_program(Um_memory memory, uint32_t word, registers gp_registers, 
                  uint32_t *program_counter);
extern void load_value(uint32_t word, registers gp_registers);

#endif