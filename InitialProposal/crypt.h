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
#include "messages.h"
#include <chrono>
#define MAX_ITERATIONS  10
#define PUZZLE_STRENGTH 16
using namespace std;
class ClientPuzzle {
private:
    string serverSideSecret;
    string date;
    string hashOutput; //result of the hash function after computation
    string rawHashedPuzzleHex; //The unmasked puzzle in hex
    string rawHashedPuzzleBin;  //The unmasked puzzle in binary
    string clientPuzzleHex;  //The masked puzzle to be given to client
    int numberOfMissingCharacters; //The number of characters masked



    string computeHash();
    string generateClientPuzzle();
    string generateServerSideSecret();
    string generateDateStamp();

    //Helper
    string hash256(const string& string);

public:
    void init_clientPuzzle();
    //Getters
    const string &getHashOutput() const;
    const string &getClientPuzzleHex() const;
    const string &getDate() const;
    int getNumberOfMissingCharacters() const;


    string getPuzzlePayload();
    int verifySolution(const string& solvedPuzzle, const string& date);
};


#endif //CLIENTPUZZLE_ORIGINAL_CRYPT_H


