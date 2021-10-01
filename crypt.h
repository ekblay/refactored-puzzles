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

#define MAX_ITERATIONS  5
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


void ClientPuzzle::init_clientPuzzle() {
    serverSideSecret = generateServerSideSecret();
    date = generateDateStamp();
    hashOutput = computeHash();
    clientPuzzleHex = generateClientPuzzle();
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
string ClientPuzzle::generateClientPuzzle() {
    string str;
    //Cut off portion to be shown to be given to client
    int indexLastByte = rawHashedPuzzleHex.length() - floor((PUZZLE_STRENGTH/4));
    numberOfMissingCharacters = rawHashedPuzzleHex.length() - indexLastByte;
    str.assign(rawHashedPuzzleHex.substr(numberOfMissingCharacters));
    //create a mask for n = numberOfMissingCharacters
    for(int n = 0; n < numberOfMissingCharacters; n++) {
        str ='0' + str;
    }
    return str;
}

string ClientPuzzle::computeHash() {
    /*
     * Generate initial hash value from:
     * SERVER_SIDE_SECRET and
     * TIMESTAMP
     */
    //concatenate serverSideSecret and date
    string inputString = serverSideSecret + date;
    //Hash the value
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(inputString.c_str()), strlen(inputString.c_str()), digest);

    //Convert the digest from decimal values to binary
    for(int k = 0; k < SHA256_DIGEST_LENGTH; k++) {
        rawHashedPuzzleBin = rawHashedPuzzleBin + (bitset<8> {digest[k]}).to_string();
    }
    //Convert the decimal digest to a stringed hex value
    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);
    }
    rawHashedPuzzleHex = string(mdString);
    string hash = hash256(rawHashedPuzzleHex);

    srand( (unsigned)time(0) );
    int rounds = 1 + (rand() % MAX_ITERATIONS);
    //hash based on number of iteration rounds
    int k  = 1;
    while (k < rounds) {
        hash = hash256(hash);
        k++;
    }

    return hash;
}

/*
 * Define all the possible hash functions to be used
 */
string ClientPuzzle::hash256(const string &string) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(string.c_str()), strlen(string.c_str()), digest);

    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    return std::string(mdString);
}

//**********************************************************
/*
 * Getters
 */
const string &ClientPuzzle::getHashOutput() const {
    return hashOutput;
}

const string &ClientPuzzle::getClientPuzzleHex() const {
    return clientPuzzleHex;
}

int ClientPuzzle::getNumberOfMissingCharacters() const {
    return numberOfMissingCharacters;
}

const string &ClientPuzzle::getDate() const {
    return date;
}

string ClientPuzzle::getPuzzlePayload() {
    return to_string(5) + DELIMITER + //Number of data items coming in
            hashOutput + DELIMITER +  //solution
           clientPuzzleHex + DELIMITER + //puzzle_to_solve
           to_string(numberOfMissingCharacters) + DELIMITER + //maskLength
            to_string(MAX_ITERATIONS) + DELIMITER +  //Max hash iterations
           date; //date
}

int ClientPuzzle::verifySolution(const string& solvedPuzzle, const string& returnedDate) {
    //We just check against the stored hash or we can recompute
    return cout <<(date == returnedDate) && (solvedPuzzle == rawHashedPuzzleHex);
}
