#include "io.h"
#include <iostream>

#include <cstdio>

namespace grail
{

fm<char> IO::read_dfa_file(const std::string& str){

	fm<char> mfm;
	std::ifstream file;
	file.open(str.c_str());
	while(!file.eof()){
		file >> mfm;
	}
	file.close();
	mfm.subset(); //We should keep the fm<char> deterministic too
	return mfm;
}

void IO::write_dfa_file(const fm<char>& fmachine,const std::string& str){
	std::ofstream myfile;
	myfile.open (str.c_str());
	myfile << fmachine;
	myfile.close();

}



fm<char> IO::read_nfa_file(const std::string& str){

	fm<char> mfm;
	std::ifstream file;
	file.open(str.c_str());
	while(!file.eof()){
		file >> mfm;
	}
	file.close();
	return mfm;
}

void IO::write_nfa_file(const fm<char>& fmachine, const std::string& str){
	std::ofstream myfile;
	myfile.open (str.c_str());
	myfile << fmachine;
	myfile.close();

}



void IO::print_string(const std::string& str){
	std::cout << str;
}

void IO::print_int(int value){
	std::cout << value << std::endl;
}

void IO::print_dfa(const fm<char>& fmachine){
	std::cout << fmachine << std::endl;

}

void IO::print_nfa(const fm<char>& fmachine){
	std::cout << fmachine << std::endl;

}

void IO::print_regex(const re<char>& regex){
	std::cout << regex << std::endl;
}


}
