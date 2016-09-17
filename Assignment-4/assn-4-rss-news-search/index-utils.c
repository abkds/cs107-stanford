#include <stdlib.h>
#include "index-utils.h"
#include "string-utils.h"
#include "vector.h"


/**
 * Function: IndexCompare
 * ----------------------
 * Piggybacks on StringCompare to compare two indexes. Indexes are
 * compared on the basis of their key.
 */
int IndexCompare(const void * elemAddr1, const void * elemAddr2) {
    index * index1 = (index *) elemAddr1;
    index * index2 = (index *) elemAddr2;
    return StringCompare(&(index1->key), &(index2->key));
}

/**
 * Function: IndexHash
 * -------------------
 * Piggybacks on StringHash to hash an index into hashset.
 */
int IndexHash(const void * elemAddr, int numBuckets) {
    index * idx = (index *) elemAddr;
    return StringHash(&(idx->key), numBuckets);
}

/**
 * Function: IndexFree
 * -------------------
 * Frees the memory associated with a single index struct stored
 * in a generic container.
 */
void IndexFree(const void * elemAddr) {
    index * idx = (index *) elemAddr;
    free(idx->key);
    VectorDispose(idx->articles);
}
