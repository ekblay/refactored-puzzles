// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<pthread.h>

//utils
#include "messages.h"
#define PORT 8080

void * socketThread(void *socket)
{
    int newSocket = *((int *)socket);

    int not_done = 1;
    int issued_puzzle = 0;
    int overloaded;
    int authenticated;

    int numBytes;
    char buffer[1024];
//Initial read
    while (not_done) {
        //
        numBytes = recv(newSocket, buffer, 1024,0);
        //Read up to the end of the size of the string read
        buffer[numBytes] = '\0';
        printf("\n********************************\n");

        if (strcmp(buffer, CLIENT_HELLO) == 0) { //CLIENT_HELLO
            printf("CLIENT: CLIENT HELLO\n");
            send(newSocket, SERVER_HELLO, sizeof(SERVER_HELLO), 0);
            printf("SENT: SENT SERVER HELLO\n");
        }

        //ACK after sending the SERVER HELLO to acknowledge receipt and ready for request
        if (strcmp(buffer,ACK_HELLO) == 0) {
            printf("CLIENT: ACK HELLO\n");
            if(overloaded) { // if system is overloaded send CLIENT_PUZZLE
                send(newSocket, HANDSHAKE_COMPLETE, strlen(CLIENT_PUZZLE), 0);
                issued_puzzle = 1;
                //TODO Send out client puzzle
                printf("SENT: CLIENT PUZZLE\n");
            } else { //else send HANDSHAKE_COMPLETE
                send(newSocket, HANDSHAKE_COMPLETE, strlen(HANDSHAKE_COMPLETE), 0);
                authenticated = 1;
                printf("SENT: HANDSHAKE COMPLETE\n");
            }
        }

        if (strcmp(buffer, CLIENT_PUZZLE_SOLUTION)) {
            if(authenticated)  {
                // Check solution if correct send HANDSHAKE_COMPLETE
                //else send CLIENT_PUZZLE_RETRY
                if (issued_puzzle) {

                    send(newSocket, HANDSHAKE_COMPLETE, strlen(HANDSHAKE_COMPLETE), 0);
                    //TODO if puzzle is correct send retry else authenticate
                    authenticated = 1;
                    issued_puzzle = 0;
                }

            }
        }

        //Client asks for resource, wait 5s as then respond
        if (strcmp(GET_RESOURCE,buffer)) {
            printf("CLIENT: GET RESOURCE\n");
            if(authenticated) {
                //send some random stuff
                sleep(5);
                send(newSocket, RESOURCE, strlen(RESOURCE), 0);
                printf("SENT: RESOURCE\n");
            }
        }

        if (strcmp(buffer, END_SESSION)) {
            printf("CLIENT: END SESSION\n");
            not_done = 0;
            printf("Server has ended session with client\n");
        }

    }
    printf("\n********************************\n");
    return 0;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 40) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    /*
     * Begin handling client requests
     */
    pthread_t tid[60];
    int i = 0;

    printf("Listening\n");
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        } else {
            printf("\n\n New Client has been accepted\n");
        }

        if( pthread_create(&tid[i++], NULL, socketThread, &new_socket) != 0 ) {
            printf("Failed to create thread\n");
        }

        printf("Creating new thread for client\n");
        if( i >= 50)
        {
            i = 0;
            while(i < 50) {
                pthread_join(tid[i++],NULL);
            }
            i = 0;
        }
    }
    return 0;
}
