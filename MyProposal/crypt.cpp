
#include "crypt.h"

using namespace std;
string ClientPuzzle::filenamePrefix = string("puzzles/puzzle_fm_");


void ClientPuzzle::init_clientPuzzle() {
    runner();
}

void ClientPuzzle:: runner() {
    // auto start = std::chrono::high_resolution_clock::now();
    cout << " *********Creating "<< to_string(TOTAL_NUMBER_OF_PUZZLES)<<" puzzles ********* "<<endl;
    int remainder = TOTAL_NUMBER_OF_PUZZLES;
    int iteration = 1;
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
    //Write to file
    grail::IO::write_dfa_file(dfa, filenamePrefix + to_string(iteration));

    //Release all temp objects
    grailString.clear_all();
    fm.clear_all();
}

string ClientPuzzle::generateEntry() {
    string secret = generateServerSideSecret();

    string inputHash = hash256("SHA256" + secret);
    string output = generateMessageDigest(inputHash);

    //The entry would be the concatenation of the input has hash and its message digest
    //Size is for the hash input 64

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


Payload ClientPuzzle::loadDFAFromFile() {
    // auto start = std::chrono::high_resolution_clock::now();
    int fileCount = 0;
    std::string path = "./puzzles";
    experimental::filesystem::directory_iterator dir = experimental::filesystem::directory_iterator(path);
    for (const auto &entry : dir)
        fileCount++;

    //Select a random file
    srand((unsigned)time(0) );
    int fileNumber = 1 + (rand() % MAX_ITERATIONS);
    grail::fm<char> dfa = grail::IO::read_dfa_file(filenamePrefix+ to_string(fileNumber));
    grail::set<grail::string<char> > results;
    int size = dfa.enumerate(1, results);

//Time analysis
//    auto elapsed = std::chrono::high_resolution_clock::now() - start;
//    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
//    cout <<"Time elapsed: " + to_string(microseconds/1000000)<< endl; //convert to seconds

    srand((unsigned)time(0) );
    int puzzleNumber = 1 + (rand() % size);
    std::string  str;
    convertGrailString(results[puzzleNumber], str);

    Payload payload = Payload(fileNumber, str.substr(0,64), str.substr(65));
    return payload;
}

/*********************************************************************
 * Public functions
 **********************************************************************/

string ClientPuzzle::generateClientPuzzle(string puzzleHex) {
    string str;
    //Cut off portion to be shown to be given to client
    int indexLastByte = puzzleHex.length() - floor((PUZZLE_STRENGTH/4));
    int numberOfMissingCharacters = puzzleHex.length() - indexLastByte;
    str.assign(puzzleHex.substr(numberOfMissingCharacters));
    //create a mask for n = numberOfMissingCharacters
    for(int n = 0; n < numberOfMissingCharacters; n++) {
        str ='0' + str;
    }
    return str;
}

int ClientPuzzle::verifySolution(const string& solvedPuzzle, const string& returnedDate) {
    //We just check against the stored hash or we can recompute
    //Check if word belongs to the language.
    return 0;
}

string ClientPuzzle::getPuzzlePayload() {

    //TODO loadPuzzle from DFA and return it

    return ""; //client puzzle + solution + file_index + numberOfMissingCharacters
}


int main(int argc, char const *argv[]) {

    ClientPuzzle cr  = {};
    //cr.init_clientPuzzle();

    cr.loadDFAFromFile();

    return 1;
}
