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
#include <string>
#include <cstring>
#include <cmath>
#include <bitset>
#include <sstream>

#define HASH_ROUNDS  1
#define PUZZLE_STRENGTH 16

char SEPARATOR = '~';
class ClientPuzzle {
private:
    std::string serverSideSecret;
    std::string date;
    std::string hashOutput; //result of the hash function after computation
    std::string rawHashedPuzzleHex; //The unmasked puzzle in hex
    std::string rawHashedPuzzleBin;  //The unmasked puzzle in binary
    std::string clientPuzzleHex;  //The masked puzzle to be given to client
    int numberOfMissingCharacters; //The number of characters masked


    std::string computeHash(int rounds);
    std::string generateClientPuzzle();
    std::string generateServerSideSecret();
    std::string generateDateStamp();

    //Helper
    std::string hash256(const std::string& string);

public:
    void init_clientPuzzle();
    //Getters
    const std::string &getHashOutput() const;
    const std::string &getClientPuzzleHex() const;
    const std::string &getDate() const;
    int getNumberOfMissingCharacters() const;

    std::string getPuzzlePayload();

};


#endif //CLIENTPUZZLE_ORIGINAL_CRYPT_H


void ClientPuzzle::init_clientPuzzle() {
    serverSideSecret = generateServerSideSecret();
    date = generateDateStamp();
    hashOutput = computeHash(HASH_ROUNDS);
    clientPuzzleHex = generateClientPuzzle();

    std::cout << "Server Side secret : " + serverSideSecret << std::endl;
    std::cout << "Date : " + date << std::endl;
    std::cout << "Hash : " + hashOutput << std::endl;
    std::cout << "Client Puzzle raw hex: " + rawHashedPuzzleHex << std::endl;
    std::cout << "Client puzzle masked: " + clientPuzzleHex<< std::endl;
}

std::string ClientPuzzle::generateDateStamp() {
    time_t t = time(0);
    struct tm *tm;

    char formatDate[16];
    tm = gmtime(&t);
    //Format according to month/day/year/hour/minute/second
    strftime(formatDate, sizeof(formatDate), "%m%d%Y%H%M%S", tm);
    return std::string(formatDate);
}
std::string ClientPuzzle::generateServerSideSecret() {

    unsigned char secret[32];
    int rc = RAND_bytes(secret, sizeof(secret));

    if(rc != 0 && rc != 1) {
        printf("Error");
        return 0;
    }
    char mdString[sizeof(secret)*2+1];
    for (int i = 0; i < sizeof(secret); i++) {
        std::sprintf(&mdString[i * 2], "%02x", (unsigned int) secret[i]);
    }
    return std::string(mdString);

}
std::string ClientPuzzle::generateClientPuzzle() {
    std::string str;
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

std::string ClientPuzzle::computeHash(int rounds) {
    /*
     * Generate initial hash value from:
     * SERVER_SIDE_SECRET and
     * TIMESTAMP
     */
    //concatenate serverSideSecret and date
    std::string inputString = serverSideSecret + date;
    //Hash the value
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(inputString.c_str()), std::strlen(inputString.c_str()), digest);

    //Convert the digest from decimal values to binary
    for(int k = 0; k < SHA256_DIGEST_LENGTH; k++) {
        rawHashedPuzzleBin = rawHashedPuzzleBin + (std::bitset<8> {digest[k]}).to_string();
    }

    //Convert the decimal digest to a stringed hex value
    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        std::sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);
    }
    rawHashedPuzzleHex = std::string(mdString);
    std::string hash = hash256(rawHashedPuzzleHex);


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
std::string ClientPuzzle::hash256(const std::string &string) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(string.c_str()), std::strlen(string.c_str()), digest);

    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        std::sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    return std::string(mdString);
}

//**********************************************************
/*
 * Getters
 */
const std::string &ClientPuzzle::getHashOutput() const {
    return hashOutput;
}

const std::string &ClientPuzzle::getClientPuzzleHex() const {
    return clientPuzzleHex;
}

int ClientPuzzle::getNumberOfMissingCharacters() const {
    return numberOfMissingCharacters;
}

const std::string &ClientPuzzle::getDate() const {
    return date;
}

std::string ClientPuzzle::getPuzzlePayload() {
   return hashOutput + SEPARATOR +
    clientPuzzleHex +  SEPARATOR +
    std::to_string(numberOfMissingCharacters) +  SEPARATOR +
    date;
}


