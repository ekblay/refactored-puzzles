// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include<pthread.h>
#include "crypt.h"

//utils
#include "messages.h"
#define PORT 8080

void * socketThread(void *socket)
{
    int newSocket = *((int *)socket);

    int not_done = 1;
    //TODO Set the appropriate values
    int issued_puzzle = 0;
    int overloaded = 0;
    int authenticated =1;

    int numBytes;
    char buffer[1024];
//Initial read
    std::cout <<"********************************" <<std::endl;
    while (not_done) {
        //
        numBytes = recv(newSocket, buffer, 1024,0);
        //Read up to the end of the size of the string read
        buffer[numBytes] = '\0';

        if (strcmp(buffer, CLIENT_HELLO) == 0) { //CLIENT_HELLO
            std::cout<<"CLIENT: CLIENT HELLO"<<std::endl;
            send(newSocket, SERVER_HELLO, sizeof(SERVER_HELLO), 0);
            std::cout<<"SENT: SENT SERVER HELLO"<<std::endl;
        }

        //ACK after sending the SERVER HELLO to acknowledge receipt and ready for request
        if (strcmp(buffer,ACK_HELLO) == 0) {
            ClientPuzzle cr = {};
            cr.init_clientPuzzle();
            std::cout<<"CLIENT: ACK HELLO"<<std::endl;
            if(1) { // if system is overloaded send CLIENT_PUZZLE
                send(newSocket, CLIENT_PUZZLE, strlen(CLIENT_PUZZLE), 0);
              //TODO Concatenate the client puzzle into one huge string and send in one swoop
                //Send target hash
                sleep(2);
                send(newSocket, cr.getPuzzlePayload().c_str(), strlen(cr.getPuzzlePayload().c_str()), 0);
                issued_puzzle = 1;
                std::cout<<"SENT: CLIENT PUZZLE"<<std::endl;
            } else { //else send HANDSHAKE_COMPLETE
                send(newSocket, HANDSHAKE_COMPLETE, strlen(HANDSHAKE_COMPLETE), 0);
                authenticated = 1;
                std::cout<<"SENT: HANDSHAKE COMPLETE"<<std::endl;
            }
        }

        if (strcmp(buffer, CLIENT_PUZZLE_SOLUTION) == 0) {
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
        if (strcmp(GET_RESOURCE,buffer) == 0) {
            std::cout <<"CLIENT: GET RESOURCE"<<std::endl;
            if(authenticated) {
                //send some random stuff
                sleep(5);
                send(newSocket, RESOURCE, strlen(RESOURCE), 0);
                std::cout << "SENT: RESOURCE" << std::endl;
            } else {

                send(newSocket, INVALID_REQUEST, strlen(INVALID_REQUEST), 0);
                std::cout << "SENT: INVALID REQUEST\n" << std::endl;
            }
        }

        if (strcmp(buffer, END_SESSION) == 0) {
            std::cout<<"CLIENT: END SESSION"<<std::endl;
            not_done = 0;
            std::cout<<"Server has ended session with client"<<std::endl;
        }

    }
    std::cout<<"********************************\n"<<std::endl;
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

    std::cout<<"Listening"<<std::endl;
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        } else {
            std::cout<<"\nNew Client has been accepted"<<std::endl;
        }

        if( pthread_create(&tid[i++], NULL, socketThread, &new_socket) != 0 ) {
            printf("Failed to create thread\n");
            exit(EXIT_FAILURE);
        }

        std::cout<<"Creating new thread for client"<<std::endl;
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
