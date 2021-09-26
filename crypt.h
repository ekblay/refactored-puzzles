//
// Created by kali on 9/22/21.
//

#ifndef CLIENTPUZZLE_ORIGINAL_CRYPT_H
#define CLIENTPUZZLE_ORIGINAL_CRYPT_H
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <ctime>

class ClientPuzzle {
private:
    unsigned char serverSideSecret[32];
    char date[9];
    const unsigned char * input;
    unsigned char out[SHA256_DIGEST_LENGTH];
    int computeHash(int rounds);
    int generateServerSideSecret();
    int generateDateStamp();

public:
    void init_clientPuzzle();

};


#endif //CLIENTPUZZLE_ORIGINAL_CRYPT_H
