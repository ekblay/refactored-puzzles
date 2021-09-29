#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#define PORT 8080

//utils
#include "messages.h"
#include "clientCrypto.h"
using namespace std;

int main(int argc, char const *argv[]) {
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
    int hasData =0;
    char buffer[1024] = {0};
    string stringBuf;
    string dataBuf;
    string prefix;
    send(sock,(MESSAGE_HEADER + CLIENT_HELLO).c_str(), (MESSAGE_HEADER + CLIENT_HELLO).length(), 0);
    cout<<"SENT: CLIENT HELLO"<<endl;
    while (1) {
        numBytes = recv(sock, buffer, 1024,0);
        buffer[numBytes] = '\0';

        stringBuf = string (buffer);
        prefix = stringBuf.substr(0,1);
        if (prefix != MESSAGE_HEADER) {
            send(sock, (MESSAGE_HEADER + INVALID_REQUEST).c_str(), (MESSAGE_HEADER + INVALID_REQUEST).length(),0);
            cout << "SENT: INVALID REQUEST\n" << endl;
        }

        //check for data
        if(stringBuf.find(DATA) != -1) {
            dataBuf = stringBuf.substr( stringBuf.find(DATA));
            hasData =1;
        }

        //Read the message after the MESSAGE HEADER
        unsigned long length = (stringBuf.find(DATA) != -1) ? stringBuf.find(DATA)-1 : stringBuf.length();
        stringBuf = stringBuf.substr(1, length);


        //SERVER_HELLO
        if (stringBuf.compare(SERVER_HELLO) == 0) {
            cout<<"SERVER: SERVER HELLO"<<endl;
            send(sock, (MESSAGE_HEADER + ACK_HELLO).c_str(), (MESSAGE_HEADER + ACK_HELLO).length(), 0);
            cout<<"SENT: ACK HELLO"<<endl;
        }

        //ACK after sending the SERVER HELLO to acknowledge receipt and ready for request
        if (stringBuf.compare(HANDSHAKE_COMPLETE) == 0) {
            cout<<"SERVER: HANDSHAKE COMPLETE"<<endl;
            send(sock, (MESSAGE_HEADER + GET_RESOURCE).c_str(), (MESSAGE_HEADER + GET_RESOURCE).length(), 0);
            cout<<"SENT: GET RESOURCE"<<endl;
        }

        if(stringBuf.compare(RESOURCE) == 0) {
            cout<<"SERVER: RESOURCE"<<endl;
            send(sock, (MESSAGE_HEADER + END_SESSION).c_str(), (MESSAGE_HEADER + END_SESSION).length(), 0);
            cout<<"*************SESSION ENDED*************"<<endl;
            return 0;
        }

        if((stringBuf.compare(CLIENT_PUZZLE)) == 0 || (stringBuf.compare(CLIENT_PUZZLE_RETRY)) == 0) {
            cout<<"SENT: " + stringBuf<<endl;
            //Retrieve all the data for the puzzle
            if(!hasData) {
                char puzzleBuffer[2048] = {0};
                int bytesReceived;
                //target hash
                bytesReceived = recv(sock, puzzleBuffer, 2048, 0);
                puzzleBuffer[bytesReceived] = '\0';
                dataBuf = string(puzzleBuffer);
                hasData = 0;
            }
            //Strip off header
            dataBuf.erase(0,MESSAGE_HEADER.length());

            //TODO include the number of data elements coming in
            size_t pos = 0;
            string token;
            array<string,4> dat; int i =0;
            while ((pos = dataBuf.find(DELIMITER)) != string::npos) {
                token = dataBuf.substr(0, pos);
                dat[i] = token; i++;
                dataBuf.erase(0, pos + DELIMITER.length());
            }
            dat[i] = dataBuf;
            string solution = dat[0];
            string clientPuzzle =dat[1] ;
            int charactersMissing = stoi(dat[2]);
            string date = dat[3];
            ClientCrypto clientCrypto = {};
            clientCrypto.calculateSolution(clientPuzzle,solution, charactersMissing);
//            cout<<"solution: " + solution<<endl;
//            cout<<"puzzle: " + clientPuzzle<<endl;
//            cout<<"rounds: " + to_string(rounds)<<endl;
//            cout<<"date: " + date<<endl;


            //Solve puzzle and send out
            send(sock,(MESSAGE_HEADER + CLIENT_PUZZLE_SOLUTION).c_str(),(MESSAGE_HEADER + CLIENT_PUZZLE_SOLUTION).length() , 0);
            cout<<"SENT: CLIENT PUZZLE"<<endl;
        }

        if(stringBuf.compare(INVALID_REQUEST) == 0) {
            cout<<"SERVER: INVALID REQUEST"<<endl;
            //On an invalid request restart the client hello
            send(sock,(MESSAGE_HEADER + CLIENT_HELLO).c_str(), (MESSAGE_HEADER + CLIENT_HELLO).length() , 0);
            cout<<"SENT: CLIENT HELLO"<<endl;
        }

    }
    return 0;
}
