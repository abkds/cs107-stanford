/**
 * File: string-utils.h
 * --------------------
 * Provides utility functions for handling strings with
 * container types like hashset and vector.
 */
#ifndef _stringutils_
#define _stringutils_

/**
 * StringHash
 * ----------
 * This function adapted from Eric Roberts' "The Art and Science of C"
 * It takes a string and uses it to derive a hash code, which
 * is an integer in the range [0, numBuckets).  The hash code is computed
 * using a method called "linear congruence."  A similar function using this
 * method is described on page 144 of Kernighan and Ritchie.  The choice of
 * the value for the kHashMultiplier can have a significant effect on the
 * performance of the algorithm, but not on its correctness.
 * This hash function has the additional feature of being case-insensitive,
 * hashing "Peter Pawlowski" and "PETER PAWLOWSKI" to the same code.
 */
int StringHash(const void * elemAddr, int numBuckets);

/**
 * Function: StringFree
 * --------------------
 * Free the memory allocated for the string, whose
 * pointer is stored in the element.
 */
void StringFree(void * elemAddr);

/**
 * Function: StringCompare
 * -----------------------
 * Wrapper function over C strcmp function, for usage
 * in generic containers.
 */
int StringCompare(const void * elemAddr1, const void * elemAddr2);

/**
 * Function: ToLower
 * -----------------------
 * Converts a mixed-case string to all lower case.
 */
void ToLower(char * word);

#endif
