//
// Created by kali on 9/9/21.
//
#include <iostream>
#include <string>
#ifndef CLIENTPUZZLE_ORIGINAL_MESSAGES_H
#define CLIENTPUZZLE_ORIGINAL_MESSAGES_H
#endif //CLIENTPUZZLE_ORIGINAL_MESSAGES_H

//HEADERS
char MESSAGE_HEADER= '!';
char DATA = '#';
char SEPARATOR = '~';

//Messages
char CLIENT_HELLO[80] =  "CLIENT_HELLO"; //Sent on starting connection with a server
char SERVER_HELLO [80]= "SERVER_HELLO";  //Sent in response to a client hello
char ACK_HELLO[80]  = "ACK_HELLO";
char ACK_RES[80]  = "ACK_RES";
char HANDSHAKE_COMPLETE[80]= "HANDSHAKE_COMPLETE";

        //Client puzzle extensions
char CLIENT_PUZZLE [80]= "CLIENT_PUZZLE"; //Similar to HelloRetryRequest message
char CLIENT_PUZZLE_SOLUTION [80]= "CLIENT_PUZZLE_SOLUTION";
char CLIENT_PUZZLE_RETRY [80] = "CLIENT_PUZZLE_RETRY";


//Normal interaction
char GET_RESOURCE [80]= "GET_RESOURCE"; //Send resource
char RESOURCE[80] = "RESOURCE";
char END_SESSION [80] =  "END_SESSION";    //This is responsible for ending the session
char INVALID_REQUEST [80] = "INVALID_REQUEST";