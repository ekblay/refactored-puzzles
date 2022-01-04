//
// Created by @ekblay
//

#include "crypt.h"
#include "clientCrypto.h"

using namespace std;
ClientPuzzle server = {};
int times[6] = {100, 500, 1000, 5000, 10000, 50000};

void executeGeneration(int);
void runWithGeneration(int);

void executeVerification(int n);
void runWithVerifications(Payload, int);

/**
 * We will be testing the time performance on specific components i.e
 * 1) The time it takes to generate the client puzzle and serve it during run time
 * 2) How much time does it take to verify the solutions provided by the client
 *
 */
int main(int argc, char const *argv[]) {
    server.init_clientPuzzle();

    cout << "\nAnalysis for generating and serving client puzzles\n" << endl;
    executeGeneration(5);

    /********************************************************************************/
    /*
   * We will create a sample puzzle and use the client crypto utility to solve it. The solvedPuzzle will be used to
   * test the verification process of the design
   * */
    /********************************************************************************/

    cout << "\nAnalysis for verifying client puzzle solution\n" << endl;
    executeVerification(5);

}

void executeGeneration(int n) {
    for(int i =0; i < n; i++) {
        for (int k = 0; k < 6; k++)
            runWithGeneration(times[k]);
    }
}
void runWithGeneration(int t) {
    auto start = std::chrono::high_resolution_clock::now();
    //===========================================================================
    for (int i = 0; i < t; i++)
        server.getPuzzlePayload();
    //===========================================================================
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << to_string(t) + "," + to_string(microseconds) << endl; //convert to seconds

    server.clearVerificationBucket();
}

void executeVerification(int n) {
    ClientCrypto client = {};
    Payload serverPayload = server.payload();
    client.calculateSolution(
            serverPayload.puzzle,
            serverPayload.messageDigest,
            serverPayload.numberOfMissingCharacters,
            serverPayload.maxIterations);
    client.setIndex(serverPayload.index);
    Payload clientPayload = client.payload();

    for(int i =0; i < n; i++) {
        for (int k = 0; k < 6; k++)
            runWithVerifications(clientPayload, times[k]);
    }
}

void runWithVerifications(Payload solvedPayload, int t) {
    auto start = std::chrono::high_resolution_clock::now();
    //===========================================================================
    for (int i = 0; i < t; i++) {
        server.verifySolution(solvedPayload.solvedPuzzle + solvedPayload.messageDigest, solvedPayload.index);
    }
    //===========================================================================
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << to_string(t) + "," + to_string(microseconds) << endl; //convert to seconds
}