using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "imdb.h"

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";

imdb::imdb(const string &directory) {
    const string actorFileName = directory + "/" + kActorFileName;
    const string movieFileName = directory + "/" + kMovieFileName;

    actorFile = acquireFileMap(actorFileName, actorInfo);
    movieFile = acquireFileMap(movieFileName, movieInfo);
}

bool imdb::good() const {
    return !((actorInfo.fd == -1) ||
             (movieInfo.fd == -1));
}

// you should be implementing these two methods right here...
bool imdb::getCredits(const string &player, vector <film> &films) const {
    int nElements = * (int *) (actorFile);
    const char * const cplayer = player.c_str();

    /* actual locations calculated from offsets */
    const char * actualLocations[nElements];

    for (int i = 1; i <= nElements; i++) {
        actualLocations[i-1] = (char *) actorFile + * ((int *) actorFile + i);
    }

    void * found = bsearch(
                &cplayer,  // pointer to key
                actualLocations, // pointer to base
                nElements, // number of elements
                sizeof(char *), // size of element
                pstrcmp // compare two pointer to pointer to c string
            );

    if (found == NULL) return false;

    /* using facts about the binary actor file, if player has even
     * number of characters, it's followed by a extra '\0'. Then a short
     * is there to depict the number of movies the player has acted
     * in. If sum of bytes of actor name and two bytes of number of
     * movies is not divisible by 4, then assume a 2 byte pad and go
     * next appropriate location.
     */

    /* pointer to actor location in file */
    char * record = * (char **) found;

    /* keep count of bytes from start of record to short */
    int nBytesFromStart = 0;

    /* move to location pointing to number of movies */
    if (player.size() % 2 == 0) {
        record += (player.size() + 2);
        nBytesFromStart += (player.size() + 2);
    } else {
        record += player.size() + 1;
        nBytesFromStart += player.size();
    }

    short nMovies =  * (short *) record;
    nBytesFromStart += 2;
    record += 2;

    /* move to start of movie names */
    if (nBytesFromStart % 4 != 0) record += 2;

    for (short i = 0; i < nMovies; i++) {
        struct film _film;

        /* calculate offset into movie file in memory */
        char * movieRecord = (char *) movieFile + * (int *) record;
        record += 4;

        string title;
        while (*movieRecord != '\0') {
            title += *movieRecord;
            movieRecord++;
        }

        /* Skip \0 at end of string */
        movieRecord++;

        /* year byte contains an offset from 1900 */
        int year = *movieRecord + 1900;

        _film.title = title;
        _film.year = year;

        films.push_back(_film);
    }
    return true;
}

bool imdb::getCast(const film &movie, vector <string> &players) const { return false; }

imdb::~imdb() {
    releaseFileMap(actorInfo);
    releaseFileMap(movieInfo);
}

// utility to compare two c strings (whose pointer is given)
int imdb::pstrcmp(const void * s1, const void * s2) {
    return strcmp( * (char **) s1, * (char **) s2 );
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM..
const void *imdb::acquireFileMap(const string &fileName, struct fileInfo &info) {
    struct stat stats;
    stat(fileName.c_str(), &stats);
    info.fileSize = stats.st_size;
    std::cout <<  "File size: " << info.fileSize << std::endl;
    info.fd = open(fileName.c_str(), O_RDONLY);
    return info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
}

void imdb::releaseFileMap(struct fileInfo &info) {
    if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
    if (info.fd != -1) close(info.fd);
}
