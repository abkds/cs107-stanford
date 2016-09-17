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
    Index * index1 = (Index *) elemAddr1;
    Index * index2 = (Index *) elemAddr2;
    return StringCompare(&(index1->key), &(index2->key));
}

/**
 * Function: IndexHash
 * -------------------
 * Piggybacks on StringHash to hash an Index into hashset.
 */
int IndexHash(const void * elemAddr, int numBuckets) {
    Index * idx = (Index *) elemAddr;
    return StringHash(&(idx->key), numBuckets);
}

/**
 * Function: IndexFree
 * -------------------
 * Frees the memory associated with a single Index struct stored
 * in a generic container.
 */
void IndexFree(void * elemAddr) {
    Index * idx = (Index *) elemAddr;
    free(idx->key);
    VectorDispose(idx->articles);
}
