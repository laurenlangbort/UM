/*
 * um.c
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68) 
 * 11/22/2022
 * 
 * File that holds and runs the actual program. Opens the given
 * .um file and calls the driver module to run the file through 
 * the program. Exits the program when halted or when program is over.
 * 
 */
 
#include "assert.h"
#include "mem.h"
#include "seq.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "um_memory.h" 
#include "um_read.h"
#include "instructions.h"
#include "execution_cycle.h"
#include <stdint.h>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, 
        "um binary from command line violates its contract\n");
        exit(1);
    }
    driver(fp, argv[1]);

    // Um_memory um = create_segment0(fp, argv[1]);
    // fprintf(stderr, "length: %d\n", memory_length(um));
    
    // uint32_t *registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    // Seq_T recycle = Seq_new(1);

    // fprintf(stderr, "first word of seg0: %d\n", getWord(um, 0, 0));
    // fprintf(stderr, "second word of seg0: %d\n", getWord(um, 0, 1));
    // fprintf(stderr, "third word of seg0: %d\n", getWord(um, 0, 2));
    // fprintf(stderr, "fourth non existent word of seg0: %d\n", 
    //         getWord(um, 0, 3));
    // fprintf(stderr, "fifth nonexistent word of seg0: %d\n", 
    //         getWord(um, 0, 4));
    
    // assert(memory_length(um) == 1); 
    // memory_free(um);

/* Testing for um_memory */
    // Um_memory um = memory_new();   
    // fprintf(stderr, "%d\n", memory_length(um));
    // addseg(um, 3);  
    // seg_info seg0 = getSeg(um, 0);
    // assert(wordLength(seg0) == 3);
    // uint32_t *seg0_seg = getSeg_array(seg0);
    // seg0_seg[0] = 1;
    // seg0_seg[1] = 2;
    // seg0_seg[2] = 3;

    // addseg(um, 5);
    // seg_info seg1 = getSeg(um, 1);
    // assert(wordLength(seg1) == 5);
    // uint32_t *seg1_seg = getSeg_array(seg1);
    // seg1_seg[0] = 5;
    // seg1_seg[1] = 6;
    // seg1_seg[2] = 7;
    // seg1_seg[3] = 8;
    // seg1_seg[4] = 9;
    
    // seg_info duplicated1 = duplicateSeg(seg1);
    // uint32_t *dup = getSeg_array(duplicated1);
    // for (int i = 0; i < wordLength(duplicated1); i++) {
    //     fprintf(stderr, "dup[%d]: %d\n", i, dup[i]);
    // }

    // int index = remSeg(um, 0);
    // changeSeg(um, index, duplicated1);
    // seg_info newseg0 = getSeg(um, index);
    // assert(wordLength(newseg0) == 5);
    // uint32_t *newseg0_seg = getSeg_array(newseg0);
    // for (int j = 0; j < wordLength(newseg0); j++) {
    //     fprintf(stderr, "newseg0[%d]: %d\n", j, newseg0_seg[j]);
    // } 
    // memory_free(um);

    


//     seg_info segment_g; 
//     segment_g = getSeg(um, 0); 
//     fprintf(stderr, "wordLength: %d\n", wordLength(segment_g)); 
//     for (int i = 0; i < 3; i++) 
//     { 
//         uint32_t *arr = getSeg_array(segment_g);
//         printf("word1 in main [%d] = [%d] \n", i, arr[i]);
//     }


//     assert(memory_length(um) == 1); 
//    //  printf("words in segment = %ld\n", words_in_seg(um, 0));  
//     uint32_t word = getWord(um, 0, 1);  
//     printf("word before change = %d\n", word); 
    
//     uint32_t new_word = 15;
//     changeWord(um, 0, 1, new_word);   
//     uint32_t changed_word = getWord(um, 0, 1);
//     printf("new word = %d\n", changed_word); 

//     // remSeg(um, 0); 
//     // printf("length now %d\n", memory_length(um)); 
//     seg_info segment_a;
//     segment_a = getSeg(um, 0);  
//     for(int i = 0; i < 3; i++) 
//     { 
//         uint32_t *s = getSeg_array(segment_a);
//         printf("word in main [%d] = [%d] \n", i, s[i]);
//     } 
    
//     int seg_remove = remSeg(um, 0);
//     printf("seg_remove = %d \n", seg_remove);
//     assert(memory_length(um) == 1); 

//     memory_free(um);
    // //size_t size = words_in_seg(um, 2); 
    // printf("size = %ld\n", words_in_seg(um, 0));
    return 0;
}


// void test_duplicate_seg () 
// {
//     Um_memory um = memory_new();   
//     fprintf(stderr, "%d\n", memory_length(um));
//     addseg(um, 3);  
//     seg_info seg0 = getSeg(um, 0);
//     assert(wordLength(seg0) == 3);
//     uint32_t *seg0_seg = getSeg_array(seg0);
//     seg0_seg[0] = 1;
//     seg0_seg[1] = 2;
//     seg0_seg[2] = 3;

//     addseg(um, 5);
//     seg_info seg1 = getSeg(um, 1);
//     assert(wordLength(seg1) == 5);
//     uint32_t *seg1_seg = getSeg_array(seg1);
//     seg1_seg[0] = 5;
//     seg1_seg[1] = 6;
//     seg1_seg[2] = 7;
//     seg1_seg[3] = 8;
//     seg1_seg[4] = 9;
    
//     seg_info duplicated1 = duplicateSeg(seg1);
//     uint32_t *dup = getSeg_array(duplicated1);
//     for (int i = 0; i < wordLength(duplicated1); i++) {
//         fprintf(stderr, "dup[%d]: %d\n", i, dup[i]);
//     }

//     int index = remSeg(um, 0);
//     changeSeg(um, index, duplicated1);
//     seg_info newseg0 = getSeg(um, index);
//     assert(wordLength(newseg0) == 5);
//     uint32_t *newseg0_seg = getSeg_array(newseg0);
//     for (int j = 0; j < wordLength(newseg0); j++) {
//         fprintf(stderr, "newseg0[%d]: %d\n", j, newseg0_seg[j]);
//     }

// }