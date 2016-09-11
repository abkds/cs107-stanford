#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn) {
	assert(elemSize > 0 && numBuckets > 0);
	assert(hashfn != NULL && comparefn != NULL);
	h->hashtable = malloc(numBuckets * sizeof(vector *));
	h->numBuckets = numBuckets;
	h->elemSize = elemSize;
	h->size = 0;
	h->hashFn = hashfn;
	h->compareFn = comparefn;
	h->freeFn = freefn;

	// initialize all vectors;
	for (int i = 0; i < h->numBuckets; i++) {
		vector * v = malloc(sizeof(vector));
		VectorNew(v, elemSize, h->freeFn, 0);
		h->hashtable[i] = v;
	}
}

void HashSetDispose(hashset *h) {
	if (h->freeFn != NULL) {
		for (int i = 0; i < h->numBuckets; i++) {
			VectorDispose(h->hashtable[i]);
			free(h->hashtable[i]);
		}
	}
	free(h->hashtable);
}

int HashSetCount(const hashset *h) {
	return h->size;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData) {
	assert(mapfn != NULL);
	for (int i = 0; i < h->numBuckets; i++) {
		VectorMap(h->hashtable[i], mapfn, auxData);
	}
}

void HashSetEnter(hashset *h, const void *elemAddr) {
	assert(elemAddr != NULL);
	int bucket = h->hashFn(elemAddr, h->numBuckets);
	assert(bucket >= 0 && bucket < h->numBuckets);

	/* position to enter in the selected vector to keep sorted */
	int position = VectorSearch(h->hashtable[bucket], elemAddr, h->compareFn, 0, true);

	position = VectorSearch(h->hashtable[bucket], elemAddr, h->compareFn, 0, true);

	if (position == -1) {
		VectorAppend(h->hashtable[bucket], elemAddr);
		VectorSort(h->hashtable[bucket], h->compareFn);
		h->size++;
	} else {
		VectorReplace(h->hashtable[bucket], elemAddr, position);
	}
}

void *HashSetLookup(const hashset *h, const void *elemAddr) {
	assert(elemAddr != NULL);
	int bucket = h->hashFn(elemAddr, h->numBuckets);
	assert(bucket >= 0 && bucket < h->numBuckets);

	int position = VectorSearch(h->hashtable[bucket], elemAddr, h->compareFn, 0, true);
	if (position != -1) return VectorNth(h->hashtable[bucket], position);
	return NULL;
}
