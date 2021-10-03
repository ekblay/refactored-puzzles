#include "dfa.h"

namespace grail
{


int DFA::size(const fm<char>& dfa){
        return dfa.number_of_states();
}
int DFA::maxstring(fm<char> dfa){
		return dfa.find_max();
}






fm<char> DFA::reverse(const fm<char>& dfa){
	fm<char> retfm(dfa);
	retfm.reverse();
	if(!retfm.is_deterministic())retfm.subset(); //We should keep it deterministic too
	return retfm;
}

fm<char> DFA::complete(const fm<char>& dfa){
    fm<char> retfm(dfa);
    retfm.complete();
    if(!retfm.is_deterministic())retfm.subset();
	return retfm;
}


fm<char> DFA::uniondfa(const fm<char>& dfa1,const fm<char>& dfa2){
    fm<char> retfm(dfa1);
	retfm+=dfa2;
	if(!retfm.is_deterministic())retfm.subset();
	return retfm;
}

fm<char> DFA::plus(const fm<char>& dfa){
    fm<char> retfm(dfa);
	retfm.plus();
	if(!retfm.is_deterministic())retfm.subset(); //We should keep it deterministic too
	return retfm;
}

fm<char> DFA::star(const fm<char>& dfa){
    fm<char> retfm(dfa);
	retfm.star();
	if(!retfm.is_deterministic())retfm.subset(); //We should keep it deterministic too
	return retfm;
}

fm<char> DFA::reachable(const fm<char>& dfa){
    fm<char> retfm(dfa);
	retfm.reachable_fm();
	if(!retfm.is_deterministic())retfm.subset(); //We should keep it deterministic too
	return retfm;
}

fm<char> DFA::reduce(const fm<char>& dfa){
    fm<char> retfm(dfa);
	retfm.reduce();
	if(!retfm.is_deterministic())retfm.subset(); //We should keep it deterministic too
	return retfm;
}


fm<char> DFA::Hopminimize(const fm<char>& dfa){
        fm<char> retfm(dfa);
	retfm.min_by_partition();
	return retfm;
}


fm<char>	DFA::cross(const fm<char>& dfa1, const fm<char>& dfa2){
    fm<char> retfm;
	retfm.cross_product(dfa1,dfa2);
	if(!retfm.is_deterministic())retfm.subset(); //We should keep it deterministic too
	return retfm;
}

fm<char>	DFA::shuffle(const fm<char>& dfa1, const fm<char>& dfa2){
    fm<char> retfm;
	retfm.shuffle(dfa1,dfa2);
	if(!retfm.is_deterministic())retfm.subset(); //We should keep it deterministic too
	return retfm;
}

fm<char>	DFA::nfatodfa(const fm<char>& dfa){
    fm<char> retfm(dfa);
	if(!retfm.is_deterministic())retfm.subset(); //We should keep it deterministic too
	return retfm;

}


fm<char>	DFA::concat(const fm<char>& dfa1, const fm<char>& dfa2){
    fm<char> retfm(dfa1);
	retfm^=dfa2;
	if(!retfm.is_deterministic())retfm.subset(); //We should keep it deterministic too
	return retfm;
}





bool	DFA::isDeterministic(const fm<char>& dfa){

	return dfa.is_deterministic() == 1;
}

bool	DFA::isFinite(const fm<char>& dfa){

	return dfa.is_finite() == 1;
}

bool	DFA::isComplete(const fm<char>& dfa){

	return dfa.is_complete() == 1;

}

bool	DFA::isUniversal(const fm<char>& dfa){

	return dfa.is_universal() == 1;
}


}
