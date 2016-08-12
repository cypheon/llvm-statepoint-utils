#ifndef __LLVM_STATEPOINT_UTILS_TABLE__
#define __LLVM_STATEPOINT_UTILS_TABLE__

#include "stdint.h"
#include "stddef.h"

/**** Types ****/

// TODO use smaller types where possible?
typedef struct {
    uint64_t retAddr;
    uint64_t frameSize;     // in bytes
    uint64_t numOffsets;
    int32_t offsets[];      // are offsets relative to the stack pointer
} frame_info_t;


typedef struct {
    uint16_t numEntries;
    frame_info_t* entries;
} table_bucket_t;

typedef struct {
    uint64_t tableSize; 
    table_bucket_t* buckets;
} statepoint_table_t;



/**** Public Functions ****/

/**
 * An amortized O(1) return address lookup function for garbage collectors.
 *
 * table - table generated by generate_table
 * retAddr - the key (a return address) corresponding to the frame you need info about.
 *
 */
frame_info_t* lookup_return_address(statepoint_table_t *table, uint64_t retAddr);

/**
 * Given an LLVM generated Stack Map, will return a table suitable for
 * efficient return address -> pointer location lookups by a garbage collector.
 *
 */
statepoint_table_t* generate_table(void* llvm_stack_map);


/**
 * Frees the memory allocated for the table.
 */
void destroy_table(statepoint_table_t* table);




/**** Private Functions ****/

/**
 * The hash function used to distribute keys uniformly across the table.
 * The implementation is one round of the xorshift64* algorithm.
 * Code Source: Wikipedia
 */
__attribute__((always_inline)) inline uint64_t hashFn(uint64_t x) {
    x ^= x >> 12; // a
	x ^= x << 25; // b
	x ^= x >> 27; // c
	return x * UINT64_C(2685821657736338717);
}


#endif /* __LLVM_STATEPOINT_UTILS_TABLE__ */
