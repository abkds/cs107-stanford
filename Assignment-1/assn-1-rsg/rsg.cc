/**
 * File: rsg.cc
 * ------------
 * Provides the implementation of the full RSG application, which
 * relies on the services of the built-in string, ifstream, vector,
 * and map classes as well as the custom Production and Definition
 * classes provided with the assignment.
 */
#include <algorithm>
#include <cassert>
#include <map>
#include <fstream>
#include "definition.h"
#include "production.h"
#include <list>
#include <string>
using namespace std;

/**
 * Takes a reference to a legitimate infile (one that's been set up
 * to layer over a file) and populates the grammar map with the
 * collection of definitions that are spelled out in the referenced
 * file.  The function is written under the assumption that the
 * referenced data file is really a grammar file that's properly
 * formatted.  You may assume that all grammars are in fact properly
 * formatted.
 *
 * @param infile a valid reference to a flat text file storing the grammar.
 * @param grammar a reference to the STL map, which maps nonterminal strings
 *                to their definitions.
 */

static void readGrammar(ifstream& infile, map<string, Definition>& grammar)
{
    while (true) {
        string uselessText;
        getline(infile, uselessText, '{');
        if (infile.eof()) return;  // true? we encountered EOF before we saw a '{': no more productions!
        infile.putback('{');
        Definition def(infile);
        grammar[def.getNonterminal()] = def;
    }
}

void printProduction(const Production& production) {
    int i = 1;
    for (Production::const_iterator it = production.begin();
         it != production.end();
         ++it) {
        cout << *it << " ";
        if (i % 11 == 0) cout << endl;
        i++;
    }
    cout << endl;
}

/**
 * Returns whether a string is terminal or not
 */
bool isTerminal(const string& word) {
    if (word[0] == '<' && word[word.size()-1] == '>') return true;
    return false;
}

/**
 * Takes a reference to the grammar map and generates a production.
 * Recursively replaces the non-terminals, with production until only
 * non-terminals remain in the production
 *
 * @param grammar a STL map which maps terminals to their productions
 * @param words sentence containing a list of non-terminals and terminals to expand
 */
vector<string> generateSentenceUtil(map<string, Definition>& grammar, vector<string> words) {
    vector<string> sentence;

    for (vector<string>::iterator it = words.begin();
         it != words.end();
         ++it) {

        string word = *it;
        if (isTerminal(word)) {
            assert(grammar.find(word) != grammar.end());
            Definition definition = grammar[word];
            Production production = definition.getRandomProduction();
            for (Production::iterator it_ = production.begin();
                 it_ != production.end();
                 ++it_)
            sentence.push_back(*it_);

        } else {
            sentence.push_back(word);
        }
    }

    bool containsTerminal = false;

    for (vector<string>::iterator it = words.begin();
         it != words.end();
         ++it) {
        string word = *it;
        if (isTerminal(word)) {
            containsTerminal = true;
            break;
        }
    }

    if (containsTerminal) {
        return generateSentenceUtil(grammar, sentence);
    }

    return sentence;
}

/**
 * Takes a reference to the grammar map and generates a sentence.
 * Initializes the production with <start> and calls a helper
 * function to produce the production.
 */
void generateSentence(map<string, Definition>& grammar) {
    /* Initialize sentence with <start> terminal */
    vector<string> words;
    words.push_back("<start>");

    /* Call recursion utility */
    words = generateSentenceUtil(grammar, words);

    /* Create final production to print */
    Production production(words);

    printProduction(production);
}

/**
 * Performs the rudimentary error checking needed to confirm that
 * the client provided a grammar file.  It then continues to
 * open the file, read the grammar into a map<string, Definition>,
 * and then print out the total number of Definitions that were read
 * in.  You're to update and decompose the main function to print
 * three randomly generated sentences, as illustrated by the sample
 * application.
 *
 * @param argc the number of tokens making up the command that invoked
 *             the RSG executable.  There must be at least two arguments,
 *             and only the first two are used.
 * @param argv the sequence of tokens making up the command, where each
 *             token is represented as a '\0'-terminated C string.
 */

int main(int argc, char *argv[])
{
    if (argc == 1) {
        cerr << "You need to specify the name of a grammar file." << endl;
        cerr << "Usage: rsg <path to grammar text file>" << endl;
        return 1; // non-zero return value means something bad happened
    }

    ifstream grammarFile(argv[1]);
    if (grammarFile.fail()) {
        cerr << "Failed to open the file named \"" << argv[1] << "\".  Check to ensure the file exists. " << endl;
        return 2; // each bad thing has its own bad return value
    }

    // things are looking good...
    map<string, Definition> grammar;
    readGrammar(grammarFile, grammar);

    const int versions = 3;
    for (int i = 1; i <= versions; i++) {
        cout << "Version #" << i << ": ---------------------" << endl;
        generateSentence(grammar);
        cout << endl;
    }

    return 0;
}
