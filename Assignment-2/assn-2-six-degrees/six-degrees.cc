#include <vector>
#include <list>
#include <set>
#include <string>
#include <iostream>
#include <iomanip>
#include "imdb.h"
#include "path.h"
using namespace std;

/**
 * Using the specified prompt, requests that the user supply
 * the name of an actor or actress.  The code returns
 * once the user has supplied a name for which some record within
 * the referenced imdb existsif (or if the user just hits return,
 * which is a signal that the empty string should just be returned.)
 *
 * @param prompt the text that should be used for the meaningful
 *               part of the user prompt.
 * @param db a reference to the imdb which can be used to confirm
 *           that a user's response is a legitimate one.
 * @return the name of the user-supplied actor or actress, or the
 *         empty string.
 */

static string promptForActor(const string& prompt, const imdb& db)
{
    string response;
    while (true) {
        cout << prompt << " [or <enter> to quit]: ";
        getline(cin, response);
        if (response == "") return "";
        vector<film> credits;
        if (db.getCredits(response, credits)) return response;
        cout << "We couldn't find \"" << response << "\" in the movie database. "
             << "Please try again." << endl;
    }
}

/**
 * Finds the shortest path of movies between two actors.
 * Algorithm: Breadth First Search
 *
 * @param source name of the starting actor
 * @param target name of the target actor
 * @param db refernce to movie database
 * @return shortest path of movies between two actors
 */
void generateShortestPath(string source, string target, const imdb& db);

/**
 * Serves as the main entry point for the six-degrees executable.
 * There are no parameters to speak of.
 *
 * @param argc the number of tokens passed to the command line to
 *             invoke this executable.  It's completely ignored
 *             here, because we don't expect any arguments.
 * @param argv the C strings making up the full command line.
 *             We expect argv[0] to be logically equivalent to
 *             "six-degrees" (or whatever absolute path was used to
 *             invoke the program), but otherwise these are ignored
 *             as well.
 * @return 0 if the program ends normally, and undefined otherwise.
 */

int main(int argc, const char *argv[])
{
    imdb db(determinePathToData(argv[1])); // inlined in imdb-utils.h
    if (!db.good()) {
        cout << "Failed to properly initialize the imdb database." << endl;
        cout << "Please check to make sure the source files exist and that you have permission to read them." << endl;
        exit(1);
    }

    while (true) {
        string source = promptForActor("Actor or actress", db);
        if (source == "") break;
        string target = promptForActor("Another actor or actress", db);
        if (target == "") break;
        if (source == target) {
            cout << "Good one.  This is only interesting if you specify two different people." << endl;
        } else {
            generateShortestPath(source, target, db);
        }
    }

    cout << "Thanks for playing!" << endl;
    return 0;
}

void generateShortestPath(string source, string target, const imdb& db) {
    list<path> partialPaths;
    set<string> previouslySeenActors;
    set<film> previouslySeenFilms;

    path initialPath(source);
    partialPaths.push_back(initialPath);

    while (!partialPaths.empty() && partialPaths.front().getLength() <= 5) {
        path currentPath = partialPaths.front();
        partialPaths.pop_front();

        string latestPlayer = currentPath.getLastPlayer();
        vector<film> credits;
        db.getCredits(latestPlayer, credits);
        for (vector<film>::iterator filmIt = credits.begin();
            filmIt != credits.end();
            ++filmIt) {
            if (previouslySeenFilms.find(*filmIt) == previouslySeenFilms.end()) {
                previouslySeenFilms.insert(*filmIt);
                vector<string> cast;
                db.getCast(*filmIt, cast);

                for(vector<string>::iterator playerIt = cast.begin();
                    playerIt != cast.end();
                    ++playerIt) {
                    if (previouslySeenActors.find(*playerIt) == previouslySeenActors.end()) {
                        previouslySeenActors.insert(*playerIt);
                        path clonedPath = currentPath;
                        clonedPath.addConnection(*filmIt, *playerIt);
                        if (*playerIt == target) {
                            cout << clonedPath;
                            return;
                        }
                        partialPaths.push_back(clonedPath);
                    }
                }
            }
        }
    }

    cout << endl << "No path between those two people could be found." << endl << endl;
}
