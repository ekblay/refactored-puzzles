// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#define PORT 8080

//utils
#include "messages.h"
int main(int argc, char const *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    int numBytes;
    char buffer[1024] = {0};
    send(sock,CLIENT_HELLO, sizeof(CLIENT_HELLO), 0);
    while (1) {
        numBytes = recv(sock, buffer, 1024,0);
        buffer[numBytes] = '\0';
        printf("%s\n", buffer);

        //SERVER_HELLO
        if (strcmp(buffer, SERVER_HELLO) == 0) {
            //send SERVER_HELLO
            printf("Got a Server hello\n");
            send(sock, ACK_HELLO, strlen(ACK_HELLO), 0);

        }

        //ACK after sending the SERVER HELLO to acknowledge receipt and ready for request
        if (strcmp(buffer,HANDSHAKE_COMPLETE) == 0) {
            printf("Got handshake_complete\n");
            send(sock, GET_RESOURCE, strlen(GET_RESOURCE), 0);

        }

        if(strcmp(buffer,RESOURCE) == 0) {
            printf("Got resource\n");
            send(sock, END_SESSION, strlen(END_SESSION), 0);
            return 0;
        }

        if((strcmp(buffer, CLIENT_PUZZLE) == 0) || ((strcmp(buffer, CLIENT_PUZZLE_RETRY))) {
            //Solve puzzle and send out
            send(sock, CLIENT_PUZZLE_SOLUTION, strlen(CLIENT_PUZZLE_SOLUTION), 0);
        }

        if(strcmp(buffer, INVALID_REQUEST) == 0) {

        }

    }
    return 0;
}
