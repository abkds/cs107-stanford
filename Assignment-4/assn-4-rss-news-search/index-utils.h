/**
 * File: index-utils.h
 * -------------------
 * Contains type information for a single index. An index contains
 * key, and the value is a vector that contains the list of "article"
 * with the respective frequencies. To store the index in a hashset,
 * hash the index based on the key value.
 */
#ifndef _indexutils_
#define _indexutils_

#include "string-utils.h"
#include "vector.h"

typedef struct {
    char * key;
    vector * articles;
} index;

/**
 * Function: IndexCompare
 * ----------------------
 * Piggybacks on StringCompare to compare two indexes. Indexes are
 * compared on the basis of their key.
 */
int IndexCompare(const void * elemAddr1, const void * elemAddr2);

/**
 * Function: IndexHash
 * -------------------
 * Piggybacks on StringHash to hash an index into hashset.
 */
int IndexHash(const void * elemAddr, int numBuckets);

/**
 * Function: IndexFree
 * -------------------
 * Frees the memory associated with a single index struct stored
 * in a generic container.
 */
void IndexFree(const void * elemAddr);

#endif
