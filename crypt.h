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
#include <array>

#define HASH_ROUNDS  1
#define PUZZLE_STRENGTH 16
using namespace std;
char SEPARATOR = '~';
class ClientPuzzle {
private:
    string serverSideSecret;
    string date;
    string hashOutput; //result of the hash function after computation
    string rawHashedPuzzleHex; //The unmasked puzzle in hex
    string rawHashedPuzzleBin;  //The unmasked puzzle in binary
    string clientPuzzleHex;  //The masked puzzle to be given to client
    int numberOfMissingCharacters; //The number of characters masked

    array<char,16> hexValues = {'a','b','c','d','e','f','0','1','2','3','4','5','6','7','8','9'};



    string computeHash(int rounds);
    string generateClientPuzzle();
    string generateServerSideSecret();
    string generateDateStamp();

    //Helper
    string hash256(const string& string);
    string bruteForceSearch( string word, int currentPosition, int lastIndex, string puzzle, string solution,int rounds);
    int hashCheck(string guess, string solution, int rounds);

public:
    void init_clientPuzzle();
    //Getters
    const string &getHashOutput() const;
    const string &getClientPuzzleHex() const;
    const string &getDate() const;
    int getNumberOfMissingCharacters() const;



    string getPuzzlePayload();
    string calculateSolution(string puzzle, string solution, int numOfMissingCharacters);

};


#endif //CLIENTPUZZLE_ORIGINAL_CRYPT_H


void ClientPuzzle::init_clientPuzzle() {
    serverSideSecret = generateServerSideSecret();
    date = generateDateStamp();
    hashOutput = computeHash(HASH_ROUNDS);
    clientPuzzleHex = generateClientPuzzle();

    cout << "Server Side secret : " + serverSideSecret << endl;
    cout << "Date : " + date << endl;
    cout << "Hash : " + hashOutput << endl;
    cout << "Client Puzzle raw hex: " + rawHashedPuzzleHex << endl;
    cout << "Client puzzle masked: " + clientPuzzleHex<< endl;
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

string ClientPuzzle::computeHash(int rounds) {
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
    //TODO add number of items coming in, and total rounds
    return hashOutput + SEPARATOR +
           clientPuzzleHex +  SEPARATOR +
           to_string(numberOfMissingCharacters) +  SEPARATOR +
           date;
}

string ClientPuzzle::calculateSolution(string puzzle, string solution, int numOfMissingCharacters) {
    //Generate base word
    string word;
    for(int n = 0; n < numberOfMissingCharacters; n++) {
        word ='0' + word;
    }
    //chop of puzzle
    puzzle = puzzle.substr(numberOfMissingCharacters);
    string result = bruteForceSearch(word,0,numOfMissingCharacters,puzzle,solution,1);
    return  result + puzzle;
}

string ClientPuzzle::bruteForceSearch(string word, int currentPosition, int lastIndex, string puzzle, string solution,int rounds) {
    if(currentPosition < lastIndex) {
        for (int i = 0; i < hexValues.size(); i++) {
            word[currentPosition] = hexValues[i];
            if(hashCheck(word+puzzle,solution,rounds)) {
                return word;
            } else{
             string res =  bruteForceSearch(word, currentPosition + 1, lastIndex, puzzle, solution, rounds);
             if(res!="0") {
                 return res;
             }
            }
        }
        return "0";
    }
    return "0";
}

int ClientPuzzle::hashCheck(string guess, string solution, int rounds) {
    int current = 1;

    //hash first iteration
    string result = hash256(guess);
    do {
        if(result.compare(solution) ==0) { //Check if result is equal to solution
            return 1;
        }
        result = hash256(result);  //compute another digest
        current++;
    } while (current <= rounds);

    return 0; //return 0 if solution is not found here
}


