//
// Created by kali on 9/29/21.
//
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
#include "messages.h"
#ifndef REFACTORED_PUZZLES_CLIENTCRYPTO_H
#define REFACTORED_PUZZLES_CLIENTCRYPTO_H


using namespace std;
class ClientCrypto {
public:
    string calculateSolution(string puzzle, string solution, int searchIndex, int rounds);
    string getPuzzleSolutionPayload();
    void setIndex(const string &index);
    Payload payload();

private:
    string bruteForceSearch(string word, int currentPosition, int lastIndex, string puzzle, string solution, int rounds);
    int hashCheck(string guess, string solution, int rounds);
    string hash256(const string& string);

    //Array of all hexValues
    array<char,16> hexValues = {'a','b','c','d','e','f','0','1','2','3','4','5','6','7','8','9'};
    string solvedPuzzle;
    string index;
    string messageDigest;
};
#endif //REFACTORED_PUZZLES_CLIENTCRYPTO_H

string ClientCrypto::calculateSolution(string puzzle, string sol, int searchIndex, int rounds) {
    //Generate base word
    messageDigest  = sol;
    string word;
    for(int n = 0; n < searchIndex; n++) {
        word ='0' + word;
    }
    //chop of puzzle
    puzzle = puzzle.substr(searchIndex);
    string result = bruteForceSearch(word,0,searchIndex,puzzle,sol,rounds);
    return solvedPuzzle = result + puzzle;
}

string ClientCrypto::bruteForceSearch(string word, int currentPosition, int lastIndex, string puzzle, string solution, int rounds) {
    if(currentPosition < lastIndex) {
        for (char hexValue : hexValues) {
            word[currentPosition] = hexValue;
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

int ClientCrypto::hashCheck(string guess, string solution, int rounds) {
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
string ClientCrypto::hash256(const string &string) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(string.c_str()), strlen(string.c_str()), digest);

    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    return std::string(mdString);
}

string ClientCrypto::getPuzzleSolutionPayload() {
    return solvedPuzzle + DELIMITER +
           messageDigest + DELIMITER +
           index;
}

Payload ClientCrypto::payload() {
    return  Payload(solvedPuzzle, messageDigest, stoi(index));
}

void ClientCrypto::setIndex(const string &in) {
    ClientCrypto::index = in;
}
