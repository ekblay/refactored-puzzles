//
// Created by kali on 10/14/21.
//

#ifndef REFACTORED_PUZZLES_PAYLOAD_H
#define REFACTORED_PUZZLES_PAYLOAD_H

#endif //REFACTORED_PUZZLES_PAYLOAD_H

#include <string>
class Payload {
public:

    int maxIterations;
    std::string puzzle;
    int  numberOfMissingCharacters;
    std::string messageDigest;

    //Client solution payload
    std::string solvedPuzzle;
    std::string date;

    Payload(std::string d, std::string puz,int missing, std::string md) {     // Constructor for my proposal
        date = d;
        puzzle.assign(puz);
        numberOfMissingCharacters = missing;
        messageDigest.assign(md);
    }

    Payload(std::string solved, std::string sol, std::string d) {
        messageDigest = sol;
        solvedPuzzle = solved;
        date = d;
    }
};
