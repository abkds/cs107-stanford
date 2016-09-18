#include "string-utils.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * Function: StringHash
 * --------------------
 * Implementation of a simple hashing function based on linear
 * congruence.
 */
static const signed long kHashMultiplier = -1664117991L;
int StringHash(const void * elemAddr, int numBuckets) {
    int i;
    unsigned long hashcode = 0;
    char * s = * (char **) elemAddr;
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

/**
 * Function: StringCompare
 * -----------------------
 * Wrapper function over C strcmp function, for usage
 * in generic containers.
 */
int StringCompare(const void * elemAddr1, const void * elemAddr2) {
    return strcmp(* (char **) elemAddr1, * (char **) elemAddr2);
}

/**
 * Function: ToLower
 * -----------------------
 * Converts a mixed-case string to all lower case.
 */
void ToLower(char * word) {
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
}
