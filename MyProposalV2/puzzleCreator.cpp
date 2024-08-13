//
// Created by @ekblay on 8/8/24.
//

#include "puzzleCreator.h"
#include "config.h"
#include <fstream>

using namespace std;

int main() {
    PuzzleCreator pc = {};

    pc.init_clientPuzzle();

    cout<< "random*****" <<endl;
    pc.getRandomPuzzleFromFile();
    pc.getRandomPuzzleFromFile();
    return 0;
}

void PuzzleCreator::init_clientPuzzle() {
    srand((unsigned) time(0));
    generatePuzzleFiles();
}


void PuzzleCreator::generatePuzzleFiles() {
    auto start = std::chrono::high_resolution_clock::now();
    cout << " *********Creating " << to_string(NUM_PUZZLES) << " puzzles ********* " << endl;

    int total_puzzles = NUM_PUZZLES;

    Puzzle entry;
    for (int k = 1; k <= total_puzzles; k++) {
        entry = generatePuzzleEntry();
        writePuzzleToFile(entry);
    }

    cout << "****************************files created ***********************" << endl;
}

Puzzle PuzzleCreator::generatePuzzleEntry() {
    string secret = generateServerSideSecret();
    string inputHash = hash256("SHA256" + secret);
    string output = generateMessageDigest(inputHash);
    //The entry would be the concatenation of the input hash and its message digest
    //Size for the hash input is 64
    return Puzzle(inputHash, output);
}

int PuzzleCreator::generateClientPuzzle(string puzzleHex, string &maskedPuzzle) {
    return 1;
}

string PuzzleCreator::generateServerSideSecret() {
    unsigned char secret[32];
    int rc = RAND_bytes(secret, sizeof(secret));

    if (rc != 0 && rc != 1) {
        printf("Error");
        return 0;
    }
    char mdString[sizeof(secret) * 2 + 1];
    for (int i = 0; i < sizeof(secret); i++) {
        sprintf(&mdString[i * 2], "%02x", (unsigned int) secret[i]);
    }
    return string(mdString);
}

string PuzzleCreator::generateMessageDigest(string inputHash) {
    string hash = hash256(inputHash);
    int rounds = MIN_ITERATIONS + (rand() % MAX_ITERATIONS);
    //hash based on number of iteration rounds
    int k = 1;
    while (k < rounds) {
        hash = hash256(hash);
        k++;
    }
    return hash;
}

string PuzzleCreator::hash256(const string &string) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(string.c_str()), strlen(string.c_str()), digest);

    char mdString[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);

    return std::string(mdString);
}

void PuzzleCreator::printProgressBar(float progress) {
    int barWidth = 80;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; i++) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << "+";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}


void PuzzleCreator::writePuzzleToFile(Puzzle &puzzle) {
    std::ofstream myfile;
    myfile.open(FILENAME.c_str(), std::ios::binary | std::ios::app);
    if (myfile.is_open()) {
        puzzle.serialize(myfile);
    } else {
        std::cerr << "Error opening or creating file: " << FILENAME << endl;
    }
    myfile.close();
}

Puzzle PuzzleCreator::getRandomPuzzleFromFile() {
    Puzzle puzzle;
    std::ifstream inFile(FILENAME, std::ios::binary);

    if (!inFile) {
        std::cerr << "Error opening file for reading" << std::endl;
        return puzzle;
    }

    // Seek to the start the random record
    size_t recordIndex = rand() % (NUM_PUZZLES + 1);;  // zero-based index, so 1 means the second record
    inFile.seekg(recordIndex * 64*2, std::ios::beg);

    puzzle.deserialize(inFile);

    std::cout << "Puzzle deserialized from file:\n";
    puzzle.displayPuzzleInfo();

    inFile.close();

    return puzzle;
}
/**
 * ================================================
 * */