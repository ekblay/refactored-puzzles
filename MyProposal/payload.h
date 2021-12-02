//
// Created by kali on 10/14/21.
//

#ifndef REFACTORED_PUZZLES_PAYLOAD_H
#define REFACTORED_PUZZLES_PAYLOAD_H

#endif //REFACTORED_PUZZLES_PAYLOAD_H

#include <string>
class Payload {
public:

    int index;

    int maxIterations;
    std::string puzzle;
    int  numberOfMissingCharacters;
    std::string messageDigest;

    //Client solution payload
    std::string solvedPuzzle;
    std::string clientSolution;

    Payload(int num, std::string puz,int missing, std::string md) {     // Constructor for my proposal
        index = num;
        puzzle.assign(puz);
        numberOfMissingCharacters = missing;
        messageDigest.assign(md);
    }

    Payload(std::string solved, std::string sol, int fNumber) {
        messageDigest = sol;
        solvedPuzzle = solved;
        index = fNumber;
    }
};
