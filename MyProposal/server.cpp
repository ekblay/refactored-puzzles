#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include<pthread.h>


//utils
#include "crypt.h"

#define PORT 8080
using namespace std;



void * socketThread(void *socket) {
    int newSocket = *((int *)socket);

    int not_done = 1;
    //TODO Set the appropriate values
    int issued_puzzle = 0;
    int overloaded = 0;
    int authenticated =0;
    int retry = 0;

    int numBytes;
    char buffer[1024];
    string stringBuf;
    string dataBuf;
    int hasData =0;
    string prefix;
    ClientPuzzle cr = {};
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
            //clear
            buffer[numBytes] = '\0';
            stringBuf = "";
        } else {
            //DATA pieces may together with the message header so check for that
            if(stringBuf.find(DATA) != -1) {
                dataBuf = stringBuf.substr( stringBuf.find(DATA));
                hasData =1;
            }
            //Read the message after the MESSAGE HEADER
            unsigned long length = (stringBuf.find(DATA) != -1) ? stringBuf.find(DATA) -1 : stringBuf.length();
            stringBuf = stringBuf.substr(1, length);
        }

        if (stringBuf == CLIENT_HELLO) { //CLIENT_HELLO
            cout<<"CLIENT: CLIENT HELLO"<<endl;
            send(newSocket, (MESSAGE_HEADER + SERVER_HELLO).c_str(), (MESSAGE_HEADER + SERVER_HELLO).length(), 0);
            cout<<"SENT: SENT SERVER HELLO"<<endl;
        }

        //ACK after sending the SERVER HELLO to acknowledge receipt and ready for request
        if (stringBuf == ACK_HELLO) {
            cout<<"CLIENT: ACK HELLO"<<endl;
            if(1) { // if system is overloaded send CLIENT_PUZZLE
                //Calculate the puzzle and solution
                send(newSocket, (MESSAGE_HEADER + CLIENT_PUZZLE).c_str(), (MESSAGE_HEADER + CLIENT_PUZZLE).length(), 0);
                //Send target hash
                cr.init_clientPuzzle();
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
            //TODO parse extra piece

                // Check solution if correct send HANDSHAKE_COMPLETE
                //else send CLIENT_PUZZLE_RETRY
                if (issued_puzzle) {
                    //Retrieve all the data from the output stream if it has not been collected already
                    if(!hasData) {
                        char puzzleBuffer[2048] = {0};
                        int bytesReceived;
                        //target hash
                        bytesReceived = recv(newSocket, buffer, 2048, 0);
                        buffer[bytesReceived] = '\0';
                        dataBuf = string(buffer);
                        hasData = 0;
                    }
                    //Strip off header
                    dataBuf.erase(0,MESSAGE_HEADER.length());

                    size_t pos = 0;
                    string token;
                    array<string,2> dat; int i =0;
                    while ((pos = dataBuf.find(DELIMITER)) != string::npos) {
                        token = dataBuf.substr(0, pos);
                        dat[i] = token;
                        i++;
                        dataBuf.erase(0, pos + DELIMITER.length());
                    }
                    dat[i] = dataBuf;

                    string solution = dat[0];
                    string date = dat[1];

                   if( cr.verifySolution(solution, date) ==0) { //If solution not correct retry
                       if(retry < 3) {
                           send(newSocket, (MESSAGE_HEADER + CLIENT_PUZZLE_RETRY).c_str(),
                                (MESSAGE_HEADER + CLIENT_PUZZLE_RETRY).length(), 0);
                           //Send target hash
                           cr.init_clientPuzzle();
                           send(newSocket, (DATA + cr.getPuzzlePayload()).c_str(),
                                (DATA + cr.getPuzzlePayload()).length(), 0);
                           retry++;
                           cout<<"SENT: CLIENT PUZZLE RETRY"<<endl;
                       } else {
                           //send end session
                           send(newSocket, (MESSAGE_HEADER + END_SESSION).c_str(),(MESSAGE_HEADER + END_SESSION).length(), 0);
                           cout<<"SENT: END SESSION"<<endl;
                           not_done = 0;
                       }
                       continue;
                   }

                   //if verified send handshake complete
                    send(newSocket, (MESSAGE_HEADER + HANDSHAKE_COMPLETE).c_str(),
                         (MESSAGE_HEADER + HANDSHAKE_COMPLETE).length(), 0);
                    authenticated = 1;
                    issued_puzzle = 0;
                    cout<<"SENT: HANDSHAKE COMPLETE"<<endl;
                } else {
                    send(newSocket, (MESSAGE_HEADER + INVALID_REQUEST).c_str(), (MESSAGE_HEADER + INVALID_REQUEST).length(),
                         0);
                    cout << "SENT: INVALID REQUEST\n" << endl;
                    //clear
                    buffer[numBytes] = '\0';
                    stringBuf = "";
                }
        }

        //Client asks for resource, wait then respond
        if ( stringBuf == GET_RESOURCE) {
            cout <<"CLIENT: GET RESOURCE"<<endl;
            if(authenticated) {
                //send some random stuff
               // sleep(2);
                send(newSocket, (MESSAGE_HEADER + RESOURCE).c_str(),(MESSAGE_HEADER + RESOURCE).length() , 0);
                cout << "SENT: RESOURCE" << endl;
            } else {
                send(newSocket, (MESSAGE_HEADER + INVALID_REQUEST).c_str(), (MESSAGE_HEADER + INVALID_REQUEST).length(),
                     0);
                cout << "SENT: INVALID REQUEST\n" << endl;
                //clear
                buffer[numBytes] = '\0';
                stringBuf = "";
            }
        }

        if(stringBuf == INVALID_REQUEST) {
            cout<<"CLIENT: INVALID REQUEST"<<endl;
            //clear buffer
            buffer[0] = '\0';
            stringBuf = "";
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
