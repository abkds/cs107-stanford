#include "article-utils.h"

/**
 * Function: ArticleCompare
 * ------------------------
 * Compares two article structs based on the frequency of words.
 * Since the need is to sort in descending order, it does reverse
 * of a normal comparison.
 */
int ArticleCompare(const void * elemAddr1, const void * elemAddr2) {
    return (* (article *) elemAddr2).frequency - (* (article *) elemAddr1).frequency;
}

/**
 * Function: ArticleFree
 * ---------------------
 * Frees the memory for an article struct which is stored in
 * a generic container like vector.
 */
void ArticleFree(const void * elemAddr) {
    article * ar = (article *) elemAddr;
    free(ar->articleURL);
    free(ar->articleTitle);
}
