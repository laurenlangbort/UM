/*
 * um_memory.h
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68)
 * 11/22/2022 
 *
 * Interface of the Um_memory. Allows functions for the representation
 * of UM memory to be accessible in other differenet modules. Hides the
 * struct with the information of how we implement UM memory so it can
 * not be modified.
 *
 */

#include "assert.h"
#include "mem.h"
#include "seq.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifndef UM_MEMORY_H 
#define UM_MEMORY_H 

typedef struct Um_memory *Um_memory;
typedef struct seg_info *seg_info;

extern Um_memory memory_new(int size);
extern uint32_t memory_length(Um_memory memory);
extern void addseg(Um_memory memory, uint32_t size);
extern size_t words_in_seg(Um_memory memory, uint32_t seg_index);
extern uint32_t getWord(Um_memory memory, uint32_t seg_index, 
                        uint32_t word_index);
extern void changeWord(Um_memory memory, uint32_t seg_index,  
                       uint32_t word_index, uint32_t word); 
extern uint32_t remSeg(Um_memory memory, uint32_t seg_index); 
extern seg_info getSeg(Um_memory memory, uint32_t seg_index); 
extern void changeSeg(Um_memory memory, uint32_t seg_index, seg_info segment);
extern void memory_free(Um_memory memory); 
extern uint32_t wordLength(seg_info segment);
extern seg_info set_segInfo(uint32_t size, uint32_t *words);
extern uint32_t *getSeg_array(seg_info segment);
extern seg_info duplicateSeg(seg_info segment);

#endif
