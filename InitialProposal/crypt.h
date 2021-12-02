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
#include "config.h"
using namespace std;
class ClientPuzzle {
private:
    Payload init_clientPuzzle();
    int generateClientPuzzle(string puzzleHex, string &maskedPuzzle);
    string generateMessageDigest(string preImage);
    string generateServerSideSecret();
    string generateDateStamp();
    string iterativelyHash(string firstPreImage, int iterations);
    //Helper
    string hash256(const string& preImage);

public:

    int verifySolution(string solvedPuzzle, string solution,  int iterations);
    string getPuzzlePayload();
    Payload payload();
};


#endif //CLIENTPUZZLE_ORIGINAL_CRYPT_H


Payload ClientPuzzle::init_clientPuzzle() {
    string secret = generateServerSideSecret();
    string date = generateDateStamp();
    string puzzle = hash256(secret + date);
    string  messageDigest = generateMessageDigest(puzzle);
    string maskedPuzzle;

    int numberOfMissingCharacters = generateClientPuzzle(puzzle, maskedPuzzle);
    Payload payload  = Payload(maskedPuzzle, numberOfMissingCharacters,messageDigest, MAX_ITERATIONS);
    return  payload;
}

string ClientPuzzle::generateDateStamp() {
    time_t t = time(0);
    struct tm *tm;

    char formatDate[16];
    tm = gmtime(&t);
    //Format according to month/day/year/hour/minute/second
    strftime(formatDate, sizeof(formatDate), "%m%d%Y%H%M%S", tm);
    return string(formatDate);
}

string ClientPuzzle::generateServerSideSecret() {

    unsigned char secret[32];
    int rc = RAND_bytes(secret, sizeof(secret));

    if(rc != 0 && rc != 1) {
        printf("Error");
        return 0;
    }
    char mdString[sizeof(secret)*2+1];
    for (int i = 0; i < sizeof(secret); i++) {
        sprintf(&mdString[i * 2], "%02x", (unsigned int) secret[i]);
    }
    return string(mdString);

}

int ClientPuzzle::generateClientPuzzle(string puzzleHex, string &maskedPuzzle) {

    //Cut off portion to be shown to be given to client
    int indexLastByte = puzzleHex.length() - floor((PUZZLE_STRENGTH/4));
    int numberOfMissingCharacters = puzzleHex.length() - indexLastByte;
    maskedPuzzle.assign(puzzleHex.substr(numberOfMissingCharacters));
    //create a mask for n = numberOfMissingCharacters
    for(int n = 0; n < numberOfMissingCharacters; n++) {
        maskedPuzzle ='0' + maskedPuzzle;
    }
    return numberOfMissingCharacters;
}

string ClientPuzzle::generateMessageDigest(string preImage) {
    srand((unsigned)time(0) );
    int rounds = MIN_ITERATIONS + (rand() % MAX_ITERATIONS);
    //hash based on number of iteration rounds
    return iterativelyHash(preImage, rounds);
}

string ClientPuzzle::iterativelyHash(string firstPreImage, int iterations) {
    string hash = hash256(firstPreImage);
    int k  = 1;
    while (k < iterations) {
        hash = hash256(hash);
        k++;
    }
    return hash;
}

/*
 * Define all the possible hash functions to be used
 */
string ClientPuzzle::hash256(const string &preImage) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(preImage.c_str()), strlen(preImage.c_str()), digest);

    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    return std::string(mdString);
}

Payload ClientPuzzle::payload() {
    Payload payload  = init_clientPuzzle();
    return payload;
}

int ClientPuzzle::verifySolution(string solvedPuzzle, string solution,  int iterations) {
    //We just check by recomputing
    string targetSolution = iterativelyHash(solvedPuzzle, iterations);
    return (solution == targetSolution);
}

string ClientPuzzle::getPuzzlePayload() {
    Payload payload = init_clientPuzzle();
    return to_string(4) + DELIMITER + //Number of data items coming in
           payload.messageDigest + DELIMITER +  //solution
           payload.puzzle + DELIMITER + //puzzle_to_solve
           to_string(payload.numberOfMissingCharacters) + DELIMITER + //maskLength
           to_string(payload.maxIterations); //Max hash iterations
}
