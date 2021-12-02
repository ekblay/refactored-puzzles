//
// Created by ekblay on 9/9/21.
//
#pragma once

#include <iostream>
#include <string>
#include <iostream>
#include <cstring>
#include "payload.h"

#ifndef CLIENTPUZZLE_ORIGINAL_MESSAGES_H
#define CLIENTPUZZLE_ORIGINAL_MESSAGES_H
#endif //CLIENTPUZZLE_ORIGINAL_MESSAGES_H

//HEADERS
std::string MESSAGE_HEADER = "!";
std::string DATA = "#"; //Used to separate the header from the data stream
std::string DELIMITER = "~"; //Used to separate parts of a data stream

//Messages
std::string CLIENT_HELLO = "CLIENT_HELLO"; //Sent on starting connection with a server
std::string SERVER_HELLO = "SERVER_HELLO";  //Sent in response to a client hello
std::string ACK_HELLO = "ACK_HELLO";
std::string HANDSHAKE_COMPLETE = "HANDSHAKE_COMPLETE";

//client puzzle extensions
std::string CLIENT_PUZZLE = "CLIENT_PUZZLE"; //Similar to HelloRetryRequest message
std::string CLIENT_PUZZLE_SOLUTION = "CLIENT_PUZZLE_SOLUTION";
std::string CLIENT_PUZZLE_RETRY = "CLIENT_PUZZLE_RETRY";


//normal interaction
std::string GET_RESOURCE = "GET_RESOURCE"; //Send resource
std::string RESOURCE = "RESOURCE";
std::string END_SESSION = "END_SESSION";    //This is responsible for ending the session
std::string INVALID_REQUEST = "INVALID_REQUEST";