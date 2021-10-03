//
// Created by kali on 9/27/21.
//
#include "crypt.h"
int main(int argc, char const *argv[]) {
    ClientPuzzle cr = {};
    cr.init_clientPuzzle();
    std::string ans = cr.calculateSolution("0000a22732b1dd546450a35ead5dac49b5927ca5829016d625ad417e79e74362",
                         "a876655189325c12948feb8b920d12e788f1832d39d8ed663d49e00f5ab77829",4);
    cout<<ans<<endl;
}