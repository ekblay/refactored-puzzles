
//
// Created by @ekblay
//

#include <chrono>
#include "crypt.h"
#include "clientCrypto.h"

using namespace std;

void executeGeneration(int);
void runWithGeneration(int);

void executeVerification(int n);
void runWithVerifications(Payload, ClientPuzzle, int);
int times[5] = {100, 500, 1000, 5000, 10000};
/**
 * We will be testing the time performance on specific components i.e
 * 1) The time it takes to generate the client puzzle and serve it during run time
 * 2) How much time does it take to verify the solutions provided by the client
 *
 */
int main(int argc, char const *argv[]) {

    cout << "\nAnalysis for generating and serving client puzzles\n" << endl;
    executeGeneration(5);

    /********************************************************************************/

    /********************************************************************************/

    cout << "\nAnalysis for verifying client puzzle solution\n" << endl;
    executeVerification(5);
}

void executeGeneration(int n) {
    for(int i =0; i < n; i++) {
        for (int k = 0; k < 5; k++)
            runWithGeneration(times[k]);
    }
}
void runWithGeneration(int t) {
    auto start = std::chrono::high_resolution_clock::now();
    //===========================================================================
    for (int i = 0; i < t; i++) {
        ClientPuzzle server = {};
        cout<< server.getPuzzlePayload() << endl;
    }
    //===========================================================================
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << to_string(t) + "," + to_string(microseconds) << endl; //convert to seconds
}

void executeVerification(int n) {
    /*
    * We will create a sample puzzle and use the client crypto utility to solve it. The solvedPuzzle will be used to
    * test the verification process of the design
    * */
    ClientPuzzle server = {};
    ClientCrypto client = {};
    Payload serverPayload = server.payload();
    client.calculateSolution(
            serverPayload.puzzle,
            serverPayload.messageDigest,
            serverPayload.numberOfMissingCharacters,
            serverPayload.maxIterations);
    Payload clientPayload = client.payload();


    for(int i = 0; i < n; i++) {
        for (int k = 0; k < 5; k++)
            runWithVerifications(clientPayload, server, times[k]);
    }
}
void runWithVerifications(Payload solvedPayload, ClientPuzzle server, int t) {
    //create random values to use as the iteration value
    int randomValues[t] = {};
    srand((unsigned) time(0));
    for(int k = 0; k < t; k++) {
        randomValues[k] = MIN_ITERATIONS + (rand() % MAX_ITERATIONS);
    }
    //*********************************************************************
    auto start = std::chrono::high_resolution_clock::now();
    //======================================================================
    for (int i = 0; i < t; i++) {
        server.verifySolution(solvedPayload.solvedPuzzle, solvedPayload.messageDigest, randomValues[t]);
    }
    //===========================================================================
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << to_string(t) + "," + to_string(microseconds) << endl; //convert to seconds
}