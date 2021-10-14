//
// Created by kali on 9/22/21.
//

#ifndef CLIENTPUZZLE_ORIGINAL_CRYPT_H
#define CLIENTPUZZLE_ORIGINAL_CRYPT_H
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
#include "payload.h"
#include "messages.h"
#include "grail-adapter/include.h"
#define MAX_ITERATIONS  10
#define PUZZLE_STRENGTH 16
#define PUZZLES_PER_FILE 10    //Anything above 20-50 will take a pretty long time to enumerate, my friend
#define TOTAL_NUMBER_OF_PUZZLES 1000 //======> in essence there will be (TOTAL_NUMBER_OF_PUZZLES/PUZZLES_PER_FILE) files
using namespace std;
class ClientPuzzle {
private:
    array<grail::fm<char>, (TOTAL_NUMBER_OF_PUZZLES/PUZZLES_PER_FILE) > nfas;
    array< grail::set<grail::string<char>>, (TOTAL_NUMBER_OF_PUZZLES/PUZZLES_PER_FILE) > results;
    Payload fetchRandomPuzzle();
    void loadNFAFromFile();
    /*
     * Functions used to generate Finite Automata files
     */
    static void runner();
    static void printProgressBar(float);
    static void  generateAutomataFiles(int, int);
    static string generateEntry();
    static void createGrailString(grail::string<char>& target, string word);
    static void convertGrailString(grail::string<char> grailString, string &target);

    int generateClientPuzzle(string puzzleHex, string &maskedPuzzle);
    static string generateServerSideSecret();
    static string generateMessageDigest(string inputHash);

    /**
    * utility function for computing SHA_256 of a given string
    * returns the message digest as a hexadecimal string literal
    */
    static string hash256(const string& string);

public:
    static string filenamePrefix;


    void init_clientPuzzle();
    string getPuzzlePayload();
    int verifySolution(const string& solvedPuzzle, const string& index);
};


#endif //CLIENTPUZZLE_ORIGINAL_CRYPT_H

