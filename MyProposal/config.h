//
// Created by @ekblay on 12/1/21.
//

#ifndef REFACTORED_PUZZLES_CONFIG_H
#define REFACTORED_PUZZLES_CONFIG_H

#endif //REFACTORED_PUZZLES_CONFIG_H

//Puzzle strength
int MIN_ITERATIONS = 5000;
int MAX_ITERATIONS = 10000;
int PUZZLE_STRENGTH = 8;

//File Configuration
int PUZZLES_PER_FILE = 5;   //Anything above 20-50 will take a pretty long time to enumerate, my friend
int TOTAL_NUMBER_OF_PUZZLES = 10000;//======> in essence there will be (TOTAL_NUMBER_OF_PUZZLES/PUZZLES_PER_FILE) files
std::string FILENAME_PREFIX = std::string("puzzles/puzzle_fm_");

//Index secrecy configuration
int MIN_COEFFICIENT_VALUE =45;
int MAX_COEFFICIENT_VALUE = 90;
int COEFFICIENT = 1; //default coefficient value
int OFFSET_VALUE = 34;



