#ifndef lisadfa_h
#define lisadfa_h

namespace grail
{

class DFA{
//Legend :
// " // " = implemented
// " * " =  has been inserted into symbol table
// " + " =  the function has been installed in the compiler

public:
	static int 			size(const fm<char>& dfa); //*+
	static int  		maxstring(fm<char> dfa);//*+ // Grail Has error in it

	static fm<char> 	reverse(const fm<char>& dfa);//*+
	static fm<char> 	uniondfa(const fm<char>& dfa1,const fm<char>& dfa2); //*+
	static fm<char>		plus(const fm<char>& dfa);//*+
	static fm<char>		star(const fm<char>& dfa);//*+
	static fm<char>		reachable(const fm<char>& dfa);//*+
	static fm<char>		reduce(const fm<char>& dfa);//*+
	static fm<char>		Hopminimize(const fm<char>& dfa);//
	static fm<char>		complete(const fm<char>& dfa);//*+
	static fm<char>		cartesian(const fm<char>& dfa); //?ask question - not implemented
	static fm<char>		cross(const fm<char>& dfa1, const fm<char>& dfa2);//*+
	static fm<char>		shuffle(const fm<char>& dfa1,const fm<char>& dfa2);//*+
	static fm<char>		nfatodfa(const fm<char>& dfa);//
	static fm<char>		concat(const fm<char>& dfa1, const fm<char>& dfa2);

	static bool			isDeterministic(const fm<char>& dfa);//*+
	static bool			isFinite(const fm<char>& dfa);//*+
	static bool			isComplete(const fm<char>& dfa); //*+
	static bool			isUniversal(const fm<char>& dfa); //*+

};

}
#endif
