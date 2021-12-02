//
// Created by ekblay on 12/1/21.
//

#ifndef REFACTORED_PUZZLES_CONFIG_H
#define REFACTORED_PUZZLES_CONFIG_H

#endif //REFACTORED_PUZZLES_CONFIG_H

int MAX_ITERATIONS = 50;
int MIN_ITERATIONS = 20;
int PUZZLE_STRENGTH = 16;
int PUZZLES_PER_FILE = 5;   //Anything above 20-50 will take a pretty long time to enumerate, my friend
int TOTAL_NUMBER_OF_PUZZLES = 10000;//======> in essence there will be (TOTAL_NUMBER_OF_PUZZLES/PUZZLES_PER_FILE) files
std::string FILENAME_PREFIX = std::string("puzzles/puzzle_fm_");
