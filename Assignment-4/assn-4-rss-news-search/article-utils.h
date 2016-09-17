/**
 * File: article-utils.h
 * ---------------------
 * Utility functions to deal with article objects. A single
 * article object contains information about the url, title
 * and the frequency of the word in context.
 *
 * Usage:
 * When storing in a vector associated with a paritcular word
 * store the article struct in a vector.
 */

#ifndef _articleutils_
#define _articleutils_


typedef struct {
    char * articleURL;
    char * articleTitle;
    int frequency;
} article;

/**
 * Function: ArticleCompare
 * ------------------------
 * Compares two article structs based on the frequency of words.
 * Since the need is to sort in descending order, it does reverse
 * of a normal comparison.
 */
int ArticleCompare(const void * elemAddr1, const void * elemAddr2);

/**
 * Function: ArticleFree
 * ---------------------
 * Frees the memory for an article struct which is stored in
 * a generic container like vector.
 */
void ArticleFree(const void * elemAddr);

#endif
