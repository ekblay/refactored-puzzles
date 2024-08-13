//
// Created by @ekblay on 8/8/24.
//

#ifndef REFACTORED_PUZZLES_PUZZLECREATOR_H
#define REFACTORED_PUZZLES_PUZZLECREATOR_H
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <bitset>
#include <sstream>
#include <array>
#include <chrono>
#include <vector>
#include <string>
#include "Puzzle.cpp"

using namespace std;

class PuzzleCreator {
private:

    /*
     * Functions used to generate Puzzle files
     */

     void printProgressBar(float);

     void generatePuzzleFiles();

     Puzzle generatePuzzleEntry();

     int generateClientPuzzle(string puzzleHex, string &maskedPuzzle);

     string generateServerSideSecret();

     string generateMessageDigest(string inputHash);

    void writePuzzleToFile(Puzzle& puzzle);

    /**
    * utility function for computing SHA_256 of a given string
    * returns the message digest as a hexadecimal string literal
    */
     string hash256(const string &string);

public:
    void init_clientPuzzle();

    Puzzle getRandomPuzzleFromFile();

};
#endif //REFACTORED_PUZZLES_PUZZLECREATOR_H
