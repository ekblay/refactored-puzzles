#include "nfa.h"

namespace grail
{

int NFA::size(const fm<char>& nfa){
        return nfa.number_of_states();
}
int NFA::maxstring(fm<char> nfa){
		return nfa.find_max();
}


fm<char> NFA::reverse(const fm<char>& nfa){
	fm<char> retfm(nfa);
	retfm.reverse();
	return retfm;
}

fm<char> NFA::unionnfa(const fm<char>& nfa1,const fm<char>& nfa2){
	fm<char> retfm(nfa1);
	retfm+=nfa2;
	retfm.subset();
	return retfm;
}

fm<char> NFA::plus(const fm<char>& nfa){
	fm<char> retfm(nfa);
	retfm.plus();

	return retfm;
}

fm<char> NFA::star(const fm<char>& nfa){
	fm<char> retfm(nfa);
	retfm.star();

	return retfm;
}

fm<char> NFA::reachable(const fm<char>& nfa){
	fm<char> retfm(nfa);
	retfm.reachable_fm();

	return retfm;
}

fm<char> NFA::reduce(const fm<char>& nfa){
	fm<char> retfm(nfa);
	retfm.reduce();

	return retfm;
}



fm<char>	NFA::complete(const fm<char>& dfa){
	fm<char> retfm(dfa);
	retfm.complete();
	return retfm;
}

fm<char>	NFA::cross(const fm<char>& nfa1, const fm<char>& nfa2){
	fm<char> retfm;
	retfm.cross_product(nfa1,nfa1);
	return retfm;
}

fm<char>	NFA::shuffle(const fm<char>& nfa1,const fm<char>& nfa2){
	fm<char> retfm;
	retfm.shuffle(nfa1,nfa2);
	return retfm;
}

fm<char>	NFA::dfatonfa(const fm<char>& nfa){
	fm<char> retfm(nfa);
	return retfm;
}

fm<char>	NFA::concat(const fm<char>& nfa1, const fm<char>& nfa2){
	fm<char> retfm(nfa1);
	retfm^=nfa2;
	return retfm;
}




bool	NFA::isDeterministic(const fm<char>& nfa){

	return nfa.is_deterministic() == 1;
}

bool	NFA::isFinite(const fm<char>& nfa){

	return nfa.is_finite() == 1;
}

bool	NFA::isComplete(const fm<char>& nfa){

	return nfa.is_complete() == 1;

}

bool	NFA::isUniversal(const fm<char>& nfa){

	return nfa.is_universal() == 1;
}


}
