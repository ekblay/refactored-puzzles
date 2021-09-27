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
#define HASH_ROUNDS  3

class ClientPuzzle {
private:
    std::string serverSideSecret;
    std::string date;
    std::string hashOutput;
    std::string puzzleToSolve;


    std::string computeHash(int rounds);
    std::string computePuzzleToSolve();
    std::string generateServerSideSecret();
    std::string generateDateStamp();

    //Helper
    std::string hash256(const std::string& string);

public:
    void init_clientPuzzle();
    const std::string &getHashOutput() const;


};


#endif //CLIENTPUZZLE_ORIGINAL_CRYPT_H

#define HASH_ROUNDS  3

void ClientPuzzle::init_clientPuzzle() {
    serverSideSecret = generateServerSideSecret();
    date = generateDateStamp();
    hashOutput = computeHash(HASH_ROUNDS);

//    std::cout << "Server Side secret : " + serverSideSecret << std::endl;
//    std::cout << "Date : " + date << std::endl;
//    std::cout << "Hash : " + hashOutput << std::endl;
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

std::string computePuzzleToSolve() {
    return "";
}
std::string ClientPuzzle::computeHash(int rounds) {
    /*
     * Generate initial hash value from:
     * SERVER_SIDE_SECRET and
     * TIMESTAMP
     */
    std::string inputString = serverSideSecret + date;
    std::string hash = hash256(inputString);


    //hash based on number of rounds
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


const std::string &ClientPuzzle::getHashOutput() const {
    return hashOutput;
}


