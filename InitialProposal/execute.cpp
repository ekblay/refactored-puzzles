
//
// Created by @ekblay
//

#include <chrono>
#include "crypt.h"
#include "clientCrypto.h"

using namespace std;

void run(int);

void runWithVerifications(Payload, ClientPuzzle, int);

/**
 * We will be testing the time performance on specific components i.e
 * 1) The time it takes to generate the client puzzle and serve it during run time
 * 2) How much time does it take to verify the solutions provided by the client
 *
 */
int main(int argc, char const *argv[]) {
    int times[6] = {100, 500, 1000, 5000, 10000, 50000};

    cout << "\nAnalysis for generating and serving client puzzles\n" << endl;
    for (int k = 0; k < 6; k++)
        run(times[k]);

    for (int k = 0; k < 6; k++)
        run(times[k]);

    for (int k = 0; k < 6; k++)
        run(times[k]);

    for (int k = 0; k < 6; k++)
        run(times[k]);

    for (int k = 0; k < 6; k++)
        run(times[k]);
    /********************************************************************************/
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
    /********************************************************************************/

    cout << "\nAnalysis for verifying client puzzle solution\n" << endl;
    for (int k = 0; k < 6; k++)
        runWithVerifications(clientPayload, server, times[k]);

    for (int k = 0; k < 6; k++)
        runWithVerifications(clientPayload, server, times[k]);

    for (int k = 0; k < 6; k++)
        runWithVerifications(clientPayload, server, times[k]);

    for (int k = 0; k < 6; k++)
        runWithVerifications(clientPayload, server, times[k]);

    for (int k = 0; k < 6; k++)
        runWithVerifications(clientPayload, server, times[k]);
}

void run(int times) {
    auto start = std::chrono::high_resolution_clock::now();
    //===========================================================================
    for (int i = 0; i < times; i++) {
        ClientPuzzle server = {};
        server.payload();
    }
    //===========================================================================
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << to_string(times) + "," + to_string(microseconds) << endl; //convert to seconds
}

void runWithVerifications(Payload solvedPayload, ClientPuzzle server, int times) {
    auto start = std::chrono::high_resolution_clock::now();
    //===========================================================================
    for (int i = 0; i < times; i++) {
        server.verifySolution(solvedPayload.solvedPuzzle, solvedPayload.messageDigest, solvedPayload.iterations);
    }
    //===========================================================================
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << to_string(times) + "," + to_string(microseconds) << endl; //convert to seconds
}