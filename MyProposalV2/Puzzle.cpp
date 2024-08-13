//
// Created by @ekblay on 8/9/24.
//

#include <iostream>
#include <string>
#include <fstream>

#include "config.h"

class Puzzle {
private:
    char preImageMessage[64]; // 64 bytes for SHA-256 hash
    char messageDigest[64];   // 64 bytes for SHA-256 hash
    static const int maxIteration = MAX_ITERATIONS; //maximum iterations

public:
    // Constructor
    Puzzle(const std::string& preI, const std::string& md) {
        std::copy(preI.begin(), preI.end(), preImageMessage);
        std::copy(md.begin(), md.end(), messageDigest);
    }

    // Default constructor
    Puzzle() {}



    std::string getPreImageMessage() const {
        return std::string(preImageMessage, 64);
    }

    std::string getMessageDigest() const {
        return std::string(messageDigest, 64);
    }

    // Display puzzle details
    void displayPuzzleInfo() const {
        std::cout << "PreImageMessage: " << getPreImageMessage() << std::endl;
        std::cout << "MessageDigest: " << getMessageDigest() << std::endl;
    }

    // Serialization: Write the object to a binary file
    void serialize(std::ofstream& out) const {
        out.write(preImageMessage, 64);
        out.write(messageDigest, 64);
    }

    // Deserialization: Read the object from a binary file
    void deserialize(std::ifstream& in) {
        in.read(preImageMessage, 64);
        in.read(messageDigest, 64);
    }
};


