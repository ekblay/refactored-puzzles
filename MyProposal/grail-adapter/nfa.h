#ifndef lisanfa_h
#define lisanfa_h

namespace grail
{

class NFA{
//Legend :
// " // " = implemented
// " * " =  has been inserted into symbol table
// " + " =  the function has been installed in the compiler

public:
	static int 			size(const fm<char>& nfa); //*+
	static int  		maxstring(fm<char> nfa);//*+ // Grail Has error in it

	static fm<char> 	reverse(const fm<char>& nfa);//*+
	static fm<char> 	unionnfa(const fm<char>& nfa1,const fm<char>& nfa2); //*+
	static fm<char>		plus(const fm<char>& nfa);//*+
	static fm<char>		star(const fm<char>& nfa);//*+
	static fm<char>		reachable(const fm<char>& nfa);//*+
	static fm<char>		reduce(const fm<char>& nfa);//*+
	static fm<char>		complete(const fm<char>& nfa);
	static fm<char>		cartesian(const fm<char>& nfa); //?ask question - not implemented
	static fm<char>		cross(const fm<char>& nfa1, const fm<char>& nfa2); //
	static fm<char>		shuffle(const fm<char>& nfa1,const fm<char>& nfa2);//
	static fm<char>		dfatonfa(const fm<char>& nfa);//
	static fm<char>		concat(const fm<char>& nfa1, const fm<char>& nfa2);

	static bool			isDeterministic(const fm<char>& nfa);//*+
	static bool			isFinite(const fm<char>& nfa);//*+
	static bool			isComplete(const fm<char>& nfa); //*+
	static bool			isUniversal(const fm<char>& nfa); //*+
};


}

#endif
