//
// Created by kali on 10/14/21.
//

#ifndef REFACTORED_PUZZLES_PAYLOAD_H
#define REFACTORED_PUZZLES_PAYLOAD_H

#endif //REFACTORED_PUZZLES_PAYLOAD_H
class Payload {
public:
    int fileNumber;
    std::string puzzle;
    std::string messageDigest;

    Payload(int num, std::string puz, std::string md) {     // Constructor
        fileNumber = num;
        puzzle = puz;
        messageDigest = md;
    }
};
