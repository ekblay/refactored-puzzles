#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include<pthread.h>


//utils
#include "messages.h"
#include "crypt.h"
#define PORT 8080
ClientPuzzle cr = {};


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
    std::string stringBuf;
    std::string prefix;
//Initial read
    std::cout <<"********************************" <<std::endl;
    while (not_done) {
        //
        numBytes = recv(newSocket, buffer, 1024,0);
        //Read up to the end of the size of the string read
        buffer[numBytes] = '\0';

        stringBuf = std::string (buffer);
        prefix = stringBuf.substr(0,1);
         if (prefix != MESSAGE_HEADER) {
             send(newSocket, (MESSAGE_HEADER + INVALID_REQUEST).c_str(), (MESSAGE_HEADER + INVALID_REQUEST).length(),
                  0);
             std::cout << "SENT: INVALID REQUEST\n" << std::endl;
         }

         //Read the message after the MESSAGE HEADER
         unsigned long length = (stringBuf.find(DATA) != -1) ? stringBuf.find(DATA) -1 : stringBuf.length();
         stringBuf = stringBuf.substr(1, length);

        if (stringBuf == CLIENT_HELLO) { //CLIENT_HELLO
            std::cout<<"CLIENT: CLIENT HELLO"<<std::endl;
            send(newSocket, (MESSAGE_HEADER + SERVER_HELLO).c_str(), (MESSAGE_HEADER + SERVER_HELLO).length(), 0);
            std::cout<<"SENT: SENT SERVER HELLO"<<std::endl;
        }

        //ACK after sending the SERVER HELLO to acknowledge receipt and ready for request
        if (stringBuf == ACK_HELLO) {
            std::cout<<"CLIENT: ACK HELLO"<<std::endl;
            if(1) { // if system is overloaded send CLIENT_PUZZLE
                send(newSocket, (MESSAGE_HEADER + CLIENT_PUZZLE).c_str(), (MESSAGE_HEADER + CLIENT_PUZZLE).length(), 0);

                //Send target hash
                send(newSocket, (DATA + cr.getPuzzlePayload()).c_str(), (DATA + cr.getPuzzlePayload()).length(), 0);
                issued_puzzle = 1;
                std::cout<<"SENT: CLIENT PUZZLE"<<std::endl;
            } else { //else send HANDSHAKE_COMPLETE
                send(newSocket, (MESSAGE_HEADER + HANDSHAKE_COMPLETE).c_str(), (MESSAGE_HEADER + HANDSHAKE_COMPLETE).length(), 0);
                authenticated = 1;
                std::cout<<"SENT: HANDSHAKE COMPLETE"<<std::endl;
            }
        }

        if (stringBuf == CLIENT_PUZZLE_SOLUTION) {
            std::cout<<"CLIENT: CLIENT PUZZLE SOLUTION"<<std::endl;
            if(authenticated)  {
                // Check solution if correct send HANDSHAKE_COMPLETE
                //else send CLIENT_PUZZLE_RETRY
                if (issued_puzzle) {
                    send(newSocket, (MESSAGE_HEADER + HANDSHAKE_COMPLETE).c_str(),(MESSAGE_HEADER + HANDSHAKE_COMPLETE).length(), 0);
                    //TODO if puzzle is correct send retry else authenticate
                    authenticated = 1;
                    issued_puzzle = 0;
                    std::cout<<"SENT: HANDSHAKE COMPLETE"<<std::endl;
                }
            }
            std::cout<<"SENT: INVALID REQUEST"<<std::endl;
        }

        //Client asks for resource, wait 5s as then respond
        if ( stringBuf == GET_RESOURCE) {
            std::cout <<"CLIENT: GET RESOURCE"<<std::endl;
            if(authenticated) {
                //send some random stuff
                sleep(2);
                send(newSocket, (MESSAGE_HEADER + RESOURCE).c_str(),(MESSAGE_HEADER + RESOURCE).length() , 0);
                std::cout << "SENT: RESOURCE" << std::endl;
            } else {

                send(newSocket,(MESSAGE_HEADER + INVALID_REQUEST).c_str(), (MESSAGE_HEADER + INVALID_REQUEST).length(), 0);
                std::cout << "SENT: INVALID REQUEST\n" << std::endl;
            }
        }

        if(stringBuf == INVALID_REQUEST) {

            //TODO check if authenticated
            std::cout<<"CLIENT: INVALID REQUEST"<<std::endl;
            send(newSocket, (MESSAGE_HEADER + SERVER_HELLO).c_str(), (MESSAGE_HEADER + SERVER_HELLO).length(), 0);
            std::cout<<"SENT: INVALID REQUEST"<<std::endl;
        }

        if (stringBuf == END_SESSION) {
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

    cr.init_clientPuzzle();
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
