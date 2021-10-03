#ifndef IO_H
#define IO_H

namespace grail
{


class IO{

public:
	static fm<char> 	read_dfa_file(const std::string&);
	static void 		write_dfa_file(const fm<char>&,const std::string &);
	static fm<char> 	read_nfa_file(const std::string&);
	static void 		write_nfa_file(const fm<char>&,const std::string &);
	static void			print_string(const std::string &);
	static void			print_int(int);
	static void			print_dfa(const fm<char>&);
	static void			print_nfa(const fm<char>&);
	static void			print_regex(const re<char>&);
};

}


#endif
