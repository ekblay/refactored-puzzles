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
#include "messages.h"
#include "grail-adapter/include.h"
#define MAX_ITERATIONS  20
#define PUZZLE_STRENGTH 16
#define PUZZLES_PER_FILE 5    //Anything above 20-50 will take a pretty long time to enumerate, my friend
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
    Payload payload();
};

#endif //CLIENTPUZZLE_ORIGINAL_CRYPT_H

/**
 * Implementation of class
 */

string ClientPuzzle::filenamePrefix = string("puzzles/puzzle_fm_");


void ClientPuzzle::init_clientPuzzle() {
    srand((unsigned)time(0));
    runner();
    loadNFAFromFile();
}

void ClientPuzzle:: runner() {
    // auto start = std::chrono::high_resolution_clock::now();
    cout << " *********Creating "<< to_string(TOTAL_NUMBER_OF_PUZZLES)<<" puzzles ********* "<<endl;
    int remainder = TOTAL_NUMBER_OF_PUZZLES;
    int iteration = 0;
    printProgressBar(0.0);
    while(remainder>0) {
        //Update the progress bar
        float progress = (iteration*PUZZLES_PER_FILE)/(float)TOTAL_NUMBER_OF_PUZZLES;
        if(iteration%(PUZZLES_PER_FILE/4) == 0)
            printProgressBar(progress);

        /*
         * We would like to create ths puzzles in portions
         * So for each file we have at most PUZZLES_PER_FILE puzzles
         */
        if((remainder-PUZZLES_PER_FILE) >= PUZZLES_PER_FILE) {
            //
            remainder = remainder - PUZZLES_PER_FILE;
            generateAutomataFiles(PUZZLES_PER_FILE, iteration);
        } else {
            generateAutomataFiles(remainder, iteration);
            remainder =0;
        }
        iteration++;
    }
    std::cout << std::endl;
    //Time analysis
//    auto elapsed = std::chrono::high_resolution_clock::now() - start;
//    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
//    cout <<"Time elapsed: " + to_string(microseconds/1000000)<< endl; //convert to seconds
}


void ClientPuzzle::generateAutomataFiles(int end, int iteration) {
    grail::fm<char> dfa;

    //Reusable vars...must be cleared on each iteration
    grail::fl<char> finiteLanguage;
    grail::fm<char> fm;
    grail::string<char> grailString;
    string entry;

    for(int k = 1; k <= end; k++) {
        entry = generateEntry();
        createGrailString(grailString, entry);
        finiteLanguage += grailString;
        fm.fltofm(finiteLanguage);
        dfa += fm;
        //clear
        grailString.clear();
        finiteLanguage.clear();
        fm.clear();
    }
    grail::fm<char> nfa  =grail::NFA::dfatonfa(dfa);
    //Write to file
    grail::IO::write_nfa_file(nfa, filenamePrefix + to_string(iteration));

    //Release all temp objects
    grailString.clear_all();
    fm.clear_all();
}


string ClientPuzzle::generateEntry() {
    string secret = generateServerSideSecret();
    string inputHash = hash256("SHA256" + secret);
    string output = generateMessageDigest(inputHash);
    //The entry would be the concatenation of the input has hash and its message digest
    //Size for the hash input is 64
    return inputHash + output;
}
string ClientPuzzle::generateMessageDigest(string inputHash) {
    string hash = hash256(inputHash);
    srand((unsigned)time(0) );
    int rounds = 1 + (rand() % MAX_ITERATIONS);
    //hash based on number of iteration rounds
    int k  = 1;
    while (k < rounds) {
        hash = hash256(hash);
        k++;
    }
    return hash;
}
void ClientPuzzle::convertGrailString(grail::string<char> grailString, string &target) {
    for(int index = 0;  index < grailString.size(); index++)
        target += grailString[index];
}
void ClientPuzzle::createGrailString(grail::string<char>& target, string word) {
    for(int index = 0;  index < word.length(); index++)
        target += word[index];
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
string ClientPuzzle::hash256(const string &string) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(string.c_str()), strlen(string.c_str()), digest);

    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    return std::string(mdString);
}
void ClientPuzzle::printProgressBar(float progress) {
    int barWidth = 80;
    std::cout << "[";
    int pos = barWidth * progress;
    for(int i = 0; i < barWidth; i++) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << "+";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}


void ClientPuzzle::loadNFAFromFile() {
    cout<<"***************Loading automata into memory ****************"<<endl;
    int fileCount = (TOTAL_NUMBER_OF_PUZZLES/PUZZLES_PER_FILE) ;
    std::string path = "./puzzles";
    printProgressBar(0.0);
    float progress;
    grail::set<grail::string<char>> set;
    grail::fm<char> nfa;
    for(int i =0; i < fileCount; i++) {
        nfa = grail::IO::read_nfa_file(filenamePrefix + to_string(i));
        nfas[i] = nfa;
        nfa.enumerate(1, set);
        results[i] = set;
        progress = (i/(float )fileCount);
        printProgressBar(progress);

        nfa.clear();
        set.clear();
    }
    cout<< endl;
}
Payload ClientPuzzle::fetchRandomPuzzle() {
    int puzzleNumber =  (rand() % PUZZLES_PER_FILE);
    int fileNumber = (rand() % results.size());
    std::string  str;
    convertGrailString(results[fileNumber][puzzleNumber], str);

    string maskedPuzzle;
    string clientPuzzle = str.substr(0, 64);
    string solution = str.substr(64, 64);
    int numMissingCharacters = generateClientPuzzle(clientPuzzle, maskedPuzzle);
    Payload payload = Payload(fileNumber, maskedPuzzle, numMissingCharacters, solution);
    return payload;
}
/*********************************************************************
 * Public functions
 **********************************************************************/

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

int ClientPuzzle::verifySolution(const string& solvedPuzzleAndSolution, const string& index) {
    //using the index as a pointer to the FM the solution belongs to, we
    //retrieve that and check for membership
    grail::string<char> str;
    createGrailString(str,solvedPuzzleAndSolution);
    return nfas[stoi(index)].member_of_language(str,0);
}

string ClientPuzzle::getPuzzlePayload() {

    //TODO loadPuzzle from DFA and return it
    Payload payload  = fetchRandomPuzzle();
    return to_string(5) + DELIMITER + //Number of data items coming in
           payload.messageDigest + DELIMITER +  //solution
           payload.puzzle + DELIMITER + //puzzle_to_solve
           to_string(payload.numberOfMissingCharacters) + DELIMITER + //maskLength
           to_string(MAX_ITERATIONS) + DELIMITER +  //Max hash iterations
           to_string(payload.fileNumber); //file number
    //client puzzle + solution + file_index + numberOfMissingCharacters
}

/*
 * For testing
 */
Payload ClientPuzzle::payload() {
    Payload payload  = fetchRandomPuzzle();
    payload.maxIterations = MAX_ITERATIONS;
    return payload;
}

