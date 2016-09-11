#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>

/**
 * Function: expandCapacity
 * ------------------------
 * Expands the capacity of the array representing the vector. Uses
 * the doubling strategy. Declared as static for internal usage
 * by Vector implementation.
 */
static void expandCapacity(vector *v) {
	v->capacity *= 2;
	v->array = realloc(v->array, v->capacity * v->elemSize);
	assert(v->array != NULL);
}

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation) {
    assert(elemSize > 0);
    v->elemSize = elemSize;
    v->freeFn = freeFn;
    assert(initialAllocation > 0);
    if (initialAllocation == 0) initialAllocation = 4;
    v->capacity = initialAllocation;
    v->size = 0;
    v->array = malloc(elemSize * initialAllocation);
    assert(v->array != NULL);
}

void VectorDispose(vector *v) {
    if (v->freeFn != NULL) {
        for (int i = 0; i < v->size; i++) {
            void * elemAddr = (char *) v->array + i * v->elemSize;
            v->freeFn(elemAddr);
        }
    }
}

int VectorLength(const vector *v) {
    return v->size;
}

void * VectorNth(const vector *v, int position) {
    assert(position >= 0 & position < v->size);
    return (char *) v->array + position * v->elemSize;
}

void VectorReplace(vector *v, const void *elemAddr, int position) {
    assert(position >= 0 && position < v->size);
    if (v->freeFn != NULL) {
        v->freeFn((char *) v->array + position * v->elemSize);
    }

    memcpy(
        (char *) v->array + position * v->elemSize,
        elemAddr,
        v->elemSize
    );
}

void VectorInsert(vector *v, const void *elemAddr, int position) {
    assert(position >= 0 && position <= v->size);
    if (v->capacity == v->size) expandCapacity(v);

    for (int i = v->size; i > position; i--) {
        memcpy(
            (char *) v->array + i * v->elemSize,
            (char *) v->array + (i - 1) * v->elemSize,
            v->elemSize
        );
    }

    memcpy(
        (char *) v->array + position * v->elemSize,
        elemAddr,
        v->elemSize
    );
    v->size++;
}

void VectorAppend(vector *v, const void *elemAddr) {
    VectorInsert(v, elemAddr, v->size);
}

void VectorDelete(vector *v, int position) {
    assert(position >= 0 && position < v->size);

    if (v->freeFn != NULL) {
        v->freeFn((char *) v->array + position * v->elemSize);
    }

    for (int i = position; i < v->size - 1; i++) {
        memcpy(
            (char *) v->array + i * v->elemSize,
            (char *) v->array + (i + 1) * v->elemSize,
            v->elemSize
        );
    }

    v->size--;
}

void VectorSort(vector *v, VectorCompareFunction compare) {
    assert(compare != NULL);
    qsort(v->array, v->size, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData) {
    assert(mapFn != NULL);
    for (int i = 0; i < v->size; i++) {
        mapFn(VectorNth(v, i), auxData);
    }
}

/*
 * Function: VectorSearch
 * ----------------------
 * Searches the specified vector for an element whose contents match
 * the element passed as the key.  Uses the comparator argument to test
 * for equality.  The startIndex parameter controls where the search
 * starts.  If the client desires to search the entire vector,
 * they should pass 0 as the startIndex.  The method will search from
 * there to the end of the vector.  The isSorted parameter allows the client
 * to specify that the vector is already in sorted order, in which case VectorSearch
 * uses a faster binary search.  If isSorted is false, a simple linear search is
 * used.  If a match is found, the position of the matching element is returned;
 * else the function returns -1.  Calling this function does not
 * re-arrange or change contents of the vector or modify the key in any way.
 *
 * An assert is raised if startIndex is less than 0 or greater than
 * the logical length (although searching from logical length will never
 * find anything, allowing this case means you can search an entirely empty
 * vector from 0 without getting an assert).  An assert is raised if the
 * comparator or the key is NULL.
 */
static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted) {
    assert(startIndex >= 0 && startIndex < v->size);
    assert(key != NULL && searchFn != NULL);
    void * found = NULL;
    if (isSorted) {
        found = bsearch(
            key,
            ((char *) v->array + v->elemSize * startIndex),
            v->size - startIndex + 1,
            v->elemSize,
            searchFn
        );
    } else {
        size_t nElements = v->size - startIndex + 1;
        found = lfind(
            key,
            ((char *) v->array + v->elemSize * startIndex),
            &nElements,
            v->elemSize,
            searchFn
        );
    }

    if (found == NULL) return kNotFound;
    return ((char *) found - (char *) v->array) / v->elemSize;
}
