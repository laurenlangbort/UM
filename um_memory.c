/*
 * um_memory.c
 * 
 * CS40 HW6
 * by Lauren Langbort (llangb01) and Stephanie Kim (skim68)
 * 11/22/2022 
 *
 * Implementation of the Um_memory. Creates module specific functions 
 * based on Hanson's Sequence functions to modify our representation of
 * UM memory and its segments containing 32 bit words
 */


#include "assert.h"
#include "mem.h"
#include "seq.h"
#include <stdlib.h>
#include <stdio.h>
#include "um_memory.h" 
#include <stdint.h>

/*
 * struct title: Um_memory
 * holds: a sequence of structs seg_info called segments
 * effects: used to help initialize the data structures for memory used by
 *          UM
 */
struct Um_memory {
        seg_info *segments; 
        int segments_length;  
        int current_length;
};

/*
 * struct title: seg_info
 * holds: an array with 32 bit ints called seg_of_words. The 32 bit ints
 *        represent the word and the array represents the segments
 * effects: allows access to the amount of words of the CArray and the 
 *          CArray itself
 */
struct seg_info {
        uint32_t *seg_of_words;
        uint32_t seg_length;
};

/*
 * title: memory_new
 * parameters: none
 * effects: creates an instance of Um_memory with 1 empty sequence
 * returns: instance of Um_memory
 * errors detected: none
 * errors not detected: none
 * notes: asserts that the memory allocated is not null/empty
 */
Um_memory memory_new(int size)
{
        Um_memory memory = (Um_memory)malloc(sizeof(struct Um_memory));
        assert(memory != NULL);  
        
        /* create a new sequence */
       // fprintf(stderr, "malloc memory size %d\n", size);
        memory->segments = (seg_info*)calloc(size, sizeof(seg_info));
        //seg_info dummy = (seg_info)calloc(size, sizeof(dummy));
        //memory->segments = dummy;
       // fprintf(stderr, "in between\n");
        memory->segments_length = size;
        memory->current_length = 0;
        //fprintf(stderr, "calloc segments\n");
        return memory;
}

/*
 * title: memory_length
 * parameters: an instance of Um_memory
 * effects: none
 * returns: int of how many segments there in Um_memory instance
 * errors detected: none
 * errors not detected:
 * notes: asserts that the memory being passed in is not null
 */
uint32_t memory_length(Um_memory memory)
{
        assert(memory != NULL);
        return memory->current_length;
}

/*
 * title: addseg
 * parameters: instance of memory, size of c array segment we want to add
 * effects: adds a sequence segment to our memory 
 * returns: none
 * errors detected: none
 * errors not detected: none
 * notes: asserts that the amount of words being added is greater than 0 and
 *        the memory being passed in is not null/empty, and the space we 
 *        malloc for the array and struct is not null/empty
 */
void addseg(Um_memory memory, uint32_t size) 
{
        // fprintf(stderr, "add segment \n");
        assert(memory != NULL);
       // fprintf(stderr, "current length: %d\n", memory->current_length);
        /* create space for a struct and a CArray with the amount of words
        as the size given */
        seg_info our_segment = (seg_info)malloc(sizeof(*our_segment));
      //  fprintf(stderr, "our segment made\n");
        assert(our_segment != NULL);
        our_segment->seg_of_words = (uint32_t*)calloc(size, sizeof(uint32_t));
        assert(our_segment->seg_of_words != NULL);
        
        our_segment->seg_length = size;

        /* adds the initialized array to the sequence */
       // fprintf(stderr, "adding to array \n");
        memory->segments[memory->current_length] = our_segment;
        memory->current_length = memory->current_length + 1;
      //  fprintf(stderr, "segment set\n");
        // Seq_addhi(memory->segments, our_segment); 
} 

/*
 * title: getWord
 * parameters: instance of memory, a segment index, 
 *             segment offset (word index)
 * effects: none 
 * returns: uint32_t word located at specific indices
 * errors detected: none
 * errors not detected:
 * notes: asserts the the memory is not null/empty and that the segment
 *        wanted to access is within bounds
 */
uint32_t getWord(Um_memory memory, uint32_t seg_index, uint32_t word_index)
{ 
        assert(memory != NULL); 

        //fprintf(stderr, "get word \n");
        /* gets the word at the index given */
        seg_info get_seg = memory->segments[seg_index];// lSeq_get(memory->segments, seg_index);
        uint32_t word = get_seg->seg_of_words[word_index];
        // fprintf(stderr, "after get word \n");
        return word;
}  

/*
 * title: changeWord
 * parameters: instance of memory, a segment index, 
 *             segment offset (word index), field we are changing to
 * effects: changes the word with a new one at a specified indices of memory 
 * returns: none
 * errors detected: none
 * errors not detected:
 * notes: asserts that the memory is not null and the segment index is within
 *        the bounds 
 */
void changeWord(Um_memory memory, uint32_t seg_index, uint32_t word_index, 
                uint32_t word) 
{ 
        assert(memory != NULL); 
        //assert(seg_index < memory_length(memory)); 

        /* gets the segment to change */
        seg_info change_seg = memory->segments[seg_index];

        /* gets a word at a certain index and sets it equal to the passed 
        in word */
        change_seg->seg_of_words[word_index] = word;  
} 

/*
 * title: remSeg
 * parameters: instance of memory, the segment index we want to remove
 * effects: sequence length is now one less; the segment at 
 *          index now holds nothing 
 * returns: index of segment that was just removed
 * errors detected: none
 * errors not detected: none
 * notes: asserts that the memory is not null/empty and the index of the
 *        segment is within the bounds of memory
 */
uint32_t remSeg(Um_memory memory, uint32_t seg_index) 
{ 
        assert(memory != NULL); 
        //assert(seg_index < memory_length(memory)); 
        // fprintf(stderr, "in remSeg\n");
        /* gets the segment wanting to remove and puts NULL in it's place */
        seg_info remove_seg = memory->segments[seg_index];
        memory->segments[seg_index] = NULL;
        // Seq_put(memory->segments, seg_index, NULL); 
        free(remove_seg->seg_of_words);
        free(remove_seg);
        return seg_index;
}

/*
 * title: getSeg
 * parameters: instance of memory, the segment index we want to get
 * effects: gets the seg_info struct from the memory 
 * returns: uint32_t C array (aka segment of memory)
 * errors detected: none
 * errors not detected: none
 * notes: asserts when the memory is not null and when the seg index is
 *        out of bounds
 */
 seg_info getSeg(Um_memory memory, uint32_t seg_index) 
{ 
        assert(memory != NULL); 
        //assert(seg_index < memory_length(memory)); 
        seg_info segment;

        /* gets the element at the index given */
       // fprintf(stderr, "seg index: %d\n", seg_index);
        segment = memory->segments[seg_index]; // Seq_get(memory->segments, seg_index); 
        return segment;
}

/*
 * title: changeSeg
 * parameters: instance of memory, the segment index we want to get replace, 
 *             a segment we want to put in instead
 * effects: replaces old segment with new segment and frees the old one
 * returns: none
 * errors detected: none
 * errors not detected:
 * notes: asserts if the memory is not empty/null
 */
void changeSeg(Um_memory memory, uint32_t seg_index, seg_info segment)
{
        assert(memory != NULL);

        /* put a new segment into the sequence and remove the old segment */
        seg_info remove_seg = memory->segments[seg_index];
        memory->segments[seg_index] = segment;
       //  remove_seg = Seq_put(memory->segments, seg_index, segment);
        free(remove_seg);
}

/*
 * title: memory_free
 * parameters: instance of memory
 * effects: frees C array segments in memory and the sequence 
 *          data structure that holds all the segments
 * returns: nothing
 * errors detected: none
 * errors not detected:
 * notes: remSeg also frees memory, so there is a case for if the memory
 *        is not null therefore it does not result in a double free, asserts
 *        if the memory is not empty/null
 */
void memory_free(Um_memory memory)
{
        assert(memory != NULL);
        
        int length = memory->segments_length;
        /* free array */
        for (int index = 0; index < length; index++) {
                seg_info curr_seg = memory->segments[index]; //Seq_get(memory->segments, index);
                
                /* if struct has not been freed */
                if (curr_seg != NULL) {
                        free((curr_seg->seg_of_words));
                        free(curr_seg);
                } 
        } 

        /* free sequence */
        free(memory->segments); 
        free(memory);
} 

/*
 * title: wordLength 
 * parameters: information about a segment
 * effects: gives access to the amount of words in the segment
 * returns: an integer of the length of words in the segment
 * errors detected: none
 * errors not detected: none
 * notes: 
 */
uint32_t wordLength(seg_info segment)
{ 
        return segment->seg_length;
}

/*
 * title: set_segInfo 
 * parameters: number of words in a segment and the C array
 *             that will be stored in memory
 * effects: creates a new seg_info struct using the info passed in through
 *          the parameters 
 * returns: the struct seg_info that holds the new C array passed in
 *          with the amount of words of the size passed in
 * errors detected: none
 * errors not detected: none
 * notes: that the new malloced memory is not null/empty
 */
seg_info set_segInfo(uint32_t size, uint32_t *words)
{
        /* malloc space for the struct */
        seg_info segment = (seg_info)malloc(sizeof(*segment));
        assert(segment != NULL);
        
        /* sets the segments to the information passed in */
        segment->seg_of_words = words;
        segment->seg_length = size;
        return segment;
}


/*
 * title: getSeg_array 
 * parameters: info struct for a specific segment 
 * effects: gives access to the CArray of the 32 bit words in the segment 
 * returns: the CArray of the 32 bit words in the segment
 * errors detected: none
 * errors not detected: none
 * notes: 
 */
uint32_t *getSeg_array(seg_info segment)
{
        return segment->seg_of_words;
}

/*
 * title: duplicateSeg 
 * parameters: info about a specific segment that you want to duplicate
 * effects: allocates space for a struct seg_info and fills that struct
 *          duplicates of the CArray/words and the length.  
 * returns: the duplicated seg_info struct
 * errors detected: none
 * errors not detected: none
 * notes: asserts the value of input is between - and 255
 *        assertions for the getting the word are made in bitpack
 */
seg_info duplicateSeg(seg_info segment)
{
        /* allocates space for the new duplicated segment */
        uint32_t length = segment->seg_length;
        seg_info duplicated_seg = (seg_info)malloc(sizeof(*duplicated_seg));
        assert(duplicated_seg != NULL);
        duplicated_seg->seg_of_words = (uint32_t*)calloc(length, 
                                                         sizeof(uint32_t));
        
        /* gets the information of the segment passed in and duplicates it */
        uint32_t *segment0 = segment->seg_of_words;
        for (uint32_t word = 0; word < length; word++) {
                duplicated_seg->seg_of_words[word] = segment0[word];
        }
        duplicated_seg->seg_length = segment->seg_length;
        
        return duplicated_seg;
}