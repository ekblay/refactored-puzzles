//
// Created by kali on 10/14/21.
//

#ifndef REFACTORED_PUZZLES_PAYLOAD_H
#define REFACTORED_PUZZLES_PAYLOAD_H

#endif //REFACTORED_PUZZLES_PAYLOAD_H

#include <string>
class Payload {
public:
    int fileNumber;
    std::string puzzle;
    int  numberOfMissingCharacters;
    std::string messageDigest;

    Payload(int num, std::string puz,int missing, std::string md) {     // Constructor
        fileNumber = num;
        puzzle.assign(puz);
        numberOfMissingCharacters = missing;
        messageDigest.assign(md);
    }
};
