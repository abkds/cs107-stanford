#include "string-utils.h"

/**
 * Function: StringHash
 * --------------------
 * Implementation of a simple hashing function based on linear
 * congruence.
 */
static const signed long kHashMultiplier = -1664117991L;
int StringHash(const char *s, int numBuckets); {
    int i;
    unsigned long hashcode = 0;
    for (i = 0; i < strlen(s); i++)
        hashcode = hashcode * kHashMultiplier + tolower(s[i]);
    return hashcode % numBuckets;
}

/** 
 * Function: StringFree
 * --------------------
 * Free the memory allocated for the string, whose
 * pointer is stored in the element.
 */
void StringFree(void * elemAddr) {
    char * s = * (char **) elemAddr;
    free(s);
}
