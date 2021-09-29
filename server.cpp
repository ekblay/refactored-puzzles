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
using namespace std;
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
    string stringBuf;
    string prefix;
//Initial read
    cout <<"********************************" <<endl;
    while (not_done) {
        //
        numBytes = recv(newSocket, buffer, 1024,0);
        //Read up to the end of the size of the string read
        buffer[numBytes] = '\0';

        stringBuf = string (buffer);
        prefix = stringBuf.substr(0,1);
         if (prefix != MESSAGE_HEADER) {
             send(newSocket, (MESSAGE_HEADER + INVALID_REQUEST).c_str(), (MESSAGE_HEADER + INVALID_REQUEST).length(),
                  0);
             cout << "SENT: INVALID REQUEST\n" << endl;
         }

         //Read the message after the MESSAGE HEADER
         unsigned long length = (stringBuf.find(DATA) != -1) ? stringBuf.find(DATA) -1 : stringBuf.length();
         stringBuf = stringBuf.substr(1, length);

        if (stringBuf == CLIENT_HELLO) { //CLIENT_HELLO
            cout<<"CLIENT: CLIENT HELLO"<<endl;
            send(newSocket, (MESSAGE_HEADER + SERVER_HELLO).c_str(), (MESSAGE_HEADER + SERVER_HELLO).length(), 0);
            cout<<"SENT: SENT SERVER HELLO"<<endl;
        }

        //ACK after sending the SERVER HELLO to acknowledge receipt and ready for request
        if (stringBuf == ACK_HELLO) {
            cout<<"CLIENT: ACK HELLO"<<endl;
            if(1) { // if system is overloaded send CLIENT_PUZZLE
                send(newSocket, (MESSAGE_HEADER + CLIENT_PUZZLE).c_str(), (MESSAGE_HEADER + CLIENT_PUZZLE).length(), 0);

                //Send target hash
                send(newSocket, (DATA + cr.getPuzzlePayload()).c_str(), (DATA + cr.getPuzzlePayload()).length(), 0);
                issued_puzzle = 1;
                cout<<"SENT: CLIENT PUZZLE"<<endl;
            } else { //else send HANDSHAKE_COMPLETE
                send(newSocket, (MESSAGE_HEADER + HANDSHAKE_COMPLETE).c_str(), (MESSAGE_HEADER + HANDSHAKE_COMPLETE).length(), 0);
                authenticated = 1;
                cout<<"SENT: HANDSHAKE COMPLETE"<<endl;
            }
        }

        if (stringBuf == CLIENT_PUZZLE_SOLUTION) {
            cout<<"CLIENT: CLIENT PUZZLE SOLUTION"<<endl;
            if(authenticated)  {
                // Check solution if correct send HANDSHAKE_COMPLETE
                //else send CLIENT_PUZZLE_RETRY
                if (issued_puzzle) {
                    send(newSocket, (MESSAGE_HEADER + HANDSHAKE_COMPLETE).c_str(),(MESSAGE_HEADER + HANDSHAKE_COMPLETE).length(), 0);
                    //TODO if puzzle is correct send retry else authenticate
                    authenticated = 1;
                    issued_puzzle = 0;
                    cout<<"SENT: HANDSHAKE COMPLETE"<<endl;
                }
            }
            cout<<"SENT: INVALID REQUEST"<<endl;
        }

        //Client asks for resource, wait 5s as then respond
        if ( stringBuf == GET_RESOURCE) {
            cout <<"CLIENT: GET RESOURCE"<<endl;
            if(authenticated) {
                //send some random stuff
                sleep(2);
                send(newSocket, (MESSAGE_HEADER + RESOURCE).c_str(),(MESSAGE_HEADER + RESOURCE).length() , 0);
                cout << "SENT: RESOURCE" << endl;
            } else {

                send(newSocket,(MESSAGE_HEADER + INVALID_REQUEST).c_str(), (MESSAGE_HEADER + INVALID_REQUEST).length(), 0);
                cout << "SENT: INVALID REQUEST\n" << endl;
            }
        }

        if(stringBuf == INVALID_REQUEST) {

            //TODO check if authenticated
            cout<<"CLIENT: INVALID REQUEST"<<endl;
            send(newSocket, (MESSAGE_HEADER + SERVER_HELLO).c_str(), (MESSAGE_HEADER + SERVER_HELLO).length(), 0);
            cout<<"SENT: INVALID REQUEST"<<endl;
        }

        if (stringBuf == END_SESSION) {
            cout<<"CLIENT: END SESSION"<<endl;
            not_done = 0;
            cout<<"Server has ended session with client"<<endl;
        }

    }
    cout<<"********************************\n"<<endl;
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
    cout<<"Listening"<<endl;
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        } else {
            cout<<"\nNew Client has been accepted"<<endl;
        }

        if( pthread_create(&tid[i++], NULL, socketThread, &new_socket) != 0 ) {
            printf("Failed to create thread\n");
            exit(EXIT_FAILURE);
        }

        cout<<"Creating new thread for client"<<endl;
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
