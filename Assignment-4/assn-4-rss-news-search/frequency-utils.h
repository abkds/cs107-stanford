/**
 * File: frequency-utils.h
 * -------------------
 * Contains type information for a single word frequency in an article. A
 * frequency contains key, and the value which is a count of how many times
 * the word appears in the article. To store the frequency in a hashset,
 * hash the frequency based on the key value.
 */
#ifndef _frequencyutils_
#define _frequencyutils_

typedef struct {
    char * key;
    int count;
} frequency;

/**
 * Function: FrequencyCompare
 * --------------------------
 * Piggybacks on StringCompare to compare two frequencies. Frequencies are
 * compared on the basis of their key.
 */
int FrequencyCompare(const void * elemAddr1, const void * elemAddr2);

/**
 * Function: FrequencyHash
 * -----------------------
 * Piggybacks on StringHash to hash an frequency into hashset.
 */
int FrequencyHash(const void * elemAddr, int numBuckets);

/**
 * Function: FrequencyFree
 * -----------------------
 * Frees the memory associated with a single frequency struct stored
 * in a generic container.
 */
void FrequencyFree(void * elemAddr);

#endif
