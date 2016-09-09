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
    void * cplayer = (void *) player.c_str();

    struct Key playerKey;
    /* point to the starting of offsets */
    playerKey.file = (void *) actorFile;
    playerKey.pkey = cplayer;

    void * found = bsearch(
                &playerKey,  // pointer to key
                (int *) actorFile + 1, // pointer to base
                nElements, // number of elements
                sizeof(int), // size of element
                playercmp // compare two pointer to pointer to c string
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
    char * record = (char *) actorFile + * (int *) found;

    /* keep count of bytes from start of record to short */
    int nBytesFromStart = 0;

    /* move to location pointing to number of movies */
    if (player.size() % 2 == 0) {
        record += (player.size() + 2);
        nBytesFromStart += (player.size() + 2);
    } else {
        record += player.size() + 1;
        nBytesFromStart += player.size() + 1;
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

bool imdb::getCast(const film &movie, vector <string> &players) const {
    int nElements = * (int *) movieFile;

    struct Key movieKey;
    movieKey.file = (void *) movieFile;
    movieKey.pkey = (void *) &movie;

    void * found = bsearch(
                    &movieKey,  // pointer to key
                    (int *) movieFile + 1, // pointer to base
                    nElements, // number of elements
                    sizeof(int), // size of element
                    moviecmp // compare two pointer to pointer to c string
                );

    if (found == NULL) return false;

    /* point to movie record in memory */
    char * record = (char *) movieFile + * (int *) found;

    int numBytesFromStart = 0;
    /* skip the number of bytes for movie name and year, if the total
     * number of bytes is odd, skip one more byte
     */
    numBytesFromStart += movie.title.size() + 2;
    record += (movie.title.size() + 2);
    if (numBytesFromStart % 2 != 0) {
        numBytesFromStart++;
        record++;
    }

    short nPlayers =  * (short *) record;
    numBytesFromStart += 2;
    record += 2;

    /* move to start of movie names */
    if (numBytesFromStart % 4 != 0) record += 2;

    for (short i = 0; i < nPlayers; i++) {
        /* calculate offset into movie file in memory */
        char * actorRecord = (char *) actorFile + * (int *) record;
        record += 4;

        string name;
        while (*actorRecord != '\0') {
            name += *actorRecord;
            actorRecord++;
        }

        players.push_back(name);
    }

    return true;
}

imdb::~imdb() {
    releaseFileMap(actorInfo);
    releaseFileMap(movieInfo);
}

// utility to compare two c strings (whose pointer is given)
int imdb::playercmp(const void * s1, const void * s2) {
    struct Key * pKey = (struct Key *) s1;
    char * toCompare = (char *) pKey->file + * (int *) s2;

    return strcmp( (const char *) pKey->pkey, toCompare );
}

int imdb::moviecmp(const void * s1, const void * s2) {
    struct Key * pKey = (struct Key *) s1;
    char * toCompare = (char *) pKey->file + * (int *) s2;

    struct film * pfilmKey = (struct film *) pKey->pkey;

    string title;
    while (*toCompare != '\0') {
        title += *toCompare;
        toCompare++;
    }

    /* Skip \0 at end of string */
    toCompare++;

    /* year byte contains an offset from 1900 */
    int year = *toCompare + 1900;

    struct film filmToCompare;
    filmToCompare.title = title;
    filmToCompare.year = year;


    /* compare the two films using overlaoded operator */
    if (*pfilmKey < filmToCompare) return -1;
    else if (*pfilmKey == filmToCompare) return 0;
    else return 1;
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
