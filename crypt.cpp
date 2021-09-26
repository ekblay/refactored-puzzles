//
// Created by kali on 9/22/21.
//

#include "crypt.h"
#include <stdio.h>
#define HASH_ROUNDS  1

int ClientPuzzle::generateDateStamp() {
    time_t t = time(0);
    struct tm *tm;

    tm = gmtime(&t);
    strftime(date, sizeof(date), "%Y%m%d", tm);
   // printf("%s\n", date);

    return 1;
}
void ClientPuzzle::init_clientPuzzle() {
    generateServerSideSecret();
    //generateDateStamp();
    computeHash(HASH_ROUNDS);
}
int ClientPuzzle::generateServerSideSecret() {

    int rc = RAND_bytes(serverSideSecret, sizeof(serverSideSecret));

    if(rc != 0 && rc != 1) {
        printf("Error");
        return 0;
    }

    int i;
    for (i = 0; i < 20; i++) {
        printf("%02x", serverSideSecret[i]);
    }
    printf("\n");

    return 1;

}


int ClientPuzzle::computeHash(int rounds) {

    input = reinterpret_cast<const unsigned char *>( "Hello 123" );

    SHA256(input, sizeof (input), out);
    int i;
    for (i = 0; i < 20; i++) {
        printf("%02x", out[i]);
    }

    return 1;
}

int main(){
    ClientPuzzle cr{};
    //cr.computeHash(0);
    cr.init_clientPuzzle();
}