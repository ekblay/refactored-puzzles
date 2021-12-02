//
// Created by ekblay on 10/14/21.
//

#ifndef REFACTORED_PUZZLES_PAYLOAD_H
#define REFACTORED_PUZZLES_PAYLOAD_H

#endif //REFACTORED_PUZZLES_PAYLOAD_H

#include <string>

class Payload {
public:
    std::string messageDigest;

    //Server specific
    int maxIterations;
    std::string puzzle;
    int numberOfMissingCharacters;


    //Client specific payload
    std::string solvedPuzzle;
    int iterations;


    //Server payload
    Payload(std::string puz, int missing, std::string md, int maxIter) {     // Constructor for my proposal
        puzzle.assign(puz);
        messageDigest.assign(md);
        numberOfMissingCharacters = missing;
        maxIterations = maxIter;
    }

    //Client payload
    Payload(std::string solved, std::string sol, int iter) {
        messageDigest.assign(sol);
        solvedPuzzle.assign(solved);
        iterations = iter;
    }
};
