#include <stdlib.h>
#include "string-utils.h"
#include "frequency-utils.h"

/**
 * Function: FrequencyCompare
 * ----------------------
 * Piggybacks on StringCompare to compare two frequencies. Frequencies are
 * compared on the basis of their key.
 */
int FrequencyCompare(const void * elemAddr1, const void * elemAddr2) {
    frequency * fqy1 = (frequency *) elemAddr1;
    frequency * fqy2 = (frequency *) elemAddr2;
    return StringCompare(&(fqy1->key), &(fqy2->key));
}

/**
 * Function: FrequencyHash
 * -------------------
 * Piggybacks on StringHash to hash an frequency into hashset.
 */
int FrequencyHash(const void * elemAddr, int numBuckets) {
    frequency * fqy = (frequency *) elemAddr;
    return StringHash(&(fqy->key), numBuckets);
}

/**
 * Function: FrequencyFree
 * -------------------
 * Frees the memory associated with a single frequency struct stored
 * in a generic container.
 */
void FrequencyFree(void * elemAddr) {
    frequency * fqy = (frequency *) elemAddr;
    free(fqy->key);
}
