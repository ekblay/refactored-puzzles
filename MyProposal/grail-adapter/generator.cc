#include "generator.h"

#include <ctime>
#include <cmath>
#include <iostream>

namespace grail
{

char Generator::char_alphabet[] = { 'a', 'b' ,'c', 'd', 'e','f','g','h','i','j','k','l','m','n','o','p',
									'q', 'r' ,'s', 't', 'u','v','w','x','y','z'};
Generator::~Generator(){

	for(int i = 0; i <  getTotalFinals(); i++){
                delete[] finals[i];
        }
	delete[] finals;
	delete[] flags;
	delete[] canonical_str;
	gmp_randclear(gmp_rand_state);
	//std::cout << "Deleted" << std::endl;

}

Generator::Generator(int n, int k, GeneratorType type): n_states(n),k_alphabets(k),g_type(type){
	flags 			= new int[getN()];
	canonical_str 	= new int[getN() * getK()];

	//Initilizaing possible finals
	f_finals =  (unsigned long)pow(2,getN());
	finals			= new int*[getTotalFinals()];
	for(int i = 0; i <  getTotalFinals(); i++){
		finals[i] = new int[getN()];
	}
	current_final = 1;
	init_flags();
	init_finals();
	reset();

	gmp_randinit_default (gmp_rand_state);
	gmp_randseed_ui(gmp_rand_state, (long) time(NULL));
	srand ( time(NULL) );

}

void Generator::init_finals(){
	for(int i =1; i < (int)pow(2,getN()); i++){
		decimal_to_binary(i);
	}
}

void Generator::decimal_to_binary(int decimal){
	int index = decimal;
	int i = 0;
	do{
	  getFinals()[index][i++] =decimal % 2;
	  decimal = decimal /2;

	}while(decimal> 0);

}

bool	Generator::is_last_final(){
	return getCurrentFinalStatesIndex() >= getTotalFinals() -1;
}

void Generator::compute_n(mpz_t result, long m,long j){
	if ( m == getN()-1 ){
		mpz_ui_pow_ui(result, getN(), getK()*getN()-1-j);
		//return (int)pow(n,     k*n-1-j);
	}else{
		mpz_t result1,spare;
		mpz_init_set_ui(result1,0);
		mpz_init_set_ui(spare,0);
		//mpz_long sum = 0;
		mpz_set_ui(result,0);
		for(long i = 0; i<= (m+1) * getK() - j -2; i++){
			compute_n(result1, m+1,j+i+1);
			mpz_set_ui(spare,0);
			mpz_ui_pow_ui(spare,m+1,i);
			mpz_mul(result1,result1,spare);
			mpz_add(result,result1,result);
			//sum += (long)pow((m+1),i) * computeN(m+1,j+i+1);
		}
		//return sum;
		mpz_clear(result1);
		mpz_clear(spare);

	}
}

/*
 * The initialization of flag to start with is in the paper, given by a formula
 * for farther information, please visit the paper in Enumeration of ICDFA
 */
void Generator::init_flags(){
	flags[0] = 0;
	for(int i = 1; i < getN(); i++){
		flags[i] = (getK() * i) - 1;
	}
}

/*
 * After initilization of each flag, the first concanical string should be created
 * based on the flag, the reset method, is what that will set them
 */
void Generator::reset(){
	//This can be written with only one for-loop, try it
	for(int i = 0; i < getN()*getK(); i++)
		getCanonicalString()[i] = 0;

	for(int i = 0; i< getN(); i++)
		getCanonicalString()[flags[i]] = i;

}

int Generator::is_flagged(int j){
    for(int i = 1; i < getN(); i++){
            if(j == getFlags()[i]) return i;
    }
    return -1;
}

void Generator::next_icdfa(unsigned long a, unsigned long b){
	int i = a * getK() + b;
	if(a < getN()-1){
		while(is_flagged(i) >= 0){
			for(int kk = i + 1; kk <= getK()*getN()-1; kk++)
				if(is_flagged(kk)< 0)
					getCanonicalString()[kk] = 0;

	        b = b - 1;
	        i = i - 1;
		}
	}

	// f[j] = the nearest flag not exceeding i
	int j=1;
	for(int x = getN()-1; x >= 0; x--){
		if(flags[x] <= i ){
	    	j = x;
	        break;
	    }
	}

	if(getCanonicalString()[i] == getCanonicalString()[ flags[j] ] ){
		getCanonicalString()[i] = 0;
	    if(b == 0) next_icdfa(a-1,getK()-1);
		else  next_icdfa(a,b-1);
	}else{
		getCanonicalString()[i] = getCanonicalString()[i] + 1;
	}

}

bool Generator::is_full(){
	for(int j = 1; j < getN()-1; j++){
		for(int l = flags[j]+1; l <= flags[j+1]-1; l++){
	    	if(getCanonicalString()[l] != j) return false;
	    }
    }

	for( int l = flags[getN()-1]; l < getK()*getN(); l++){
		if(getCanonicalString()[l] != getN()-1) return false;
	}

    return true;
}


bool Generator::is_last(){
	int index = 0;
	int i;
	for(i= 1; i <= getN()-1; i++){
		if(getCanonicalString()[index] != i) return false;
		index++;
	}
	for(i = getK()-1; i <= (getK()-1) * (getN()+1); i++){
		if(getCanonicalString()[index] != getN()-1) return false;
		index++;
	}
	return true;

}


void Generator::next_flags(int i){

    if(i == 1){
    	flags[i] = flags[i]-1;
    }else{
    	if(flags[i] -1 == flags[i-1]){
            flags[i] = getK() * i -1;
            next_flags(i-1);
    	}else
    		flags[i] = flags[i]-1;

    }

}


fm<char> 	Generator::next(){
	if(this->g_type == Generator::RANDOM)
	{
		//Generate a random canonical string
		//Read the max
		// generate something between 0 and max
		// genreate the ICDFA according to the random number genreated
		mpz_t max;
		getSize(max);


		mpz_t chose;
		mpz_init(chose);

		mpz_urandomm (chose, gmp_rand_state ,max);
		//mpz_set_ui(chose,215);
		//mpz_init_set_ui(chose,0)
		//gmp_printf("ICDF #=%Zd has been chosen.\n", chose);
		generate_from_index(chose);


		setCurrentFinalStatesIndex ( rand() % (getTotalFinals()-1) + 1);


	}else{
		//Generate in the seuqntional mode
		if(!is_last_final()){
					setCurrentFinalStatesIndex(getCurrentFinalStatesIndex()+1);
		}else if(is_last()){
			return fm<char>();//empty fm
		}else if(is_full()){
			next_flags(getN()-1);
			reset();
			setCurrentFinalStatesIndex(1);
		}else{
			next_icdfa(getN()-1,getK()-1);
			setCurrentFinalStatesIndex(1);
		}

	}

	//return the generated (either random, or sequential) fm
	return canonicalstring_to_fm();
}

fm<char>* Generator::next(mpz_t m){

}

void Generator::print_flags(){
	for(int i=0; i< getN(); i++){
		std::cout << getFlags()[i] ;
	}

}

void Generator::print_finalstates_configuration(){
	for(int i=0; i < getN(); i++){
		std::cout << getFinals()[getCurrentFinalStatesIndex()][i];
	}

}

void Generator::print_canonical_string(){
	for(int i=0; i < getN() * getK(); i++){
		std::cout << getCanonicalString()[i];
	}
}
fm<char>   Generator::canonicalstring_to_fm(){
/*	//Translate the array of canonical string into
	// grail+ format
	fm<char> *generated_fm = new fm<char>();

	inst<char>		t;
	state s1, s2;
	char r;

	list<inst<char> >	l;

	generated_fm->clear();
	for(int i = 0; i < getN(); i++){
		for(int j = 0; j<getK(); j++){
			//printf("{state(%d) R(%d) State(%d)} , ",i,j,s[i+j]);
			s1 = i;
			s2 = getCanonicalString()[i+j];
			r  = char_alphabet[j];
			t.assign(s1,r,s2);
			l+= t;
		}
		if( getFinals()[getCurrentFinalStatesIndex()][i] == 1)
			generated_fm->final_states += s1;
	}
	s1 = 0;
	generated_fm->start_states = s1;
	//getFinals()[][]   getTotalFinals  getCurrentFinalStatesIndex

	generated_fm->arcs.from_list(l);
	//generated_fm->subset();
	return generated_fm;
*/

	fm<char> generated_fm;

	inst<char>		t;
	state s1, s2;
	char r;

	list<inst<char> >	l;

	generated_fm.clear();


	for(int i = 0; i < getN(); i++){
		for(int j = 0; j<getK(); j++){
			//printf("{state(%d) R(%d) State(%d)} , ",i,j,s[i+j]);
			s1 = i;
			s2 = getCanonicalString()[i+j];
			r  = char_alphabet[j];
			t.assign(s1,r,s2);
			l+= t;
		}
		if( getFinals()[getCurrentFinalStatesIndex()][i] == 1)
			generated_fm.final_states += s1;
	}

	s1 = 0;
	generated_fm.start_states = s1;
	//getFinals()[][]   getTotalFinals  getCurrentFinalStatesIndex

	generated_fm.arcs.from_list(l);
	//generated_fm->subset();
	return generated_fm;

}

fm<char>	Generator::current(){
	return canonicalstring_to_fm();
}

bool		Generator::hasNext(){
	return !is_last() || !is_last_final();
}

void Generator::getSize(mpz_t max){
	mpz_init_set_ui(max, 0);

	mpz_t spare;
	mpz_init(spare);

	for(int i = 0; i < getK(); i++){
		mpz_set_ui(spare,0);
		compute_n(spare,1,i);
		mpz_add(max,max,spare);
	}
	//gmp_printf("Maximum Number of ICDFs:%Zd\n",max);
	mpz_clear(spare);
}


void 	Generator::generate_from_index(mpz_t m){

	mpz_t ss,mult;
	mpz_init_set_ui(ss,1);
	mpz_init_set_ui(mult,1);

	mpz_t p,result;
	mpz_init(p);
	mpz_init_set_ui(result,0);
	getFlags()[0] = 0;
	for(long i = 1; i < getN(); i++){
		long j = getK() * i - 1;
		//int p = (long)pow(i,j-f[i-1]-1);
		mpz_ui_pow_ui(p,i,j-getFlags()[i-1]-1);
		compute_n(result,i,j);
		mpz_mul(mult,p,ss); // p * s
		//gmp_printf("p=%Zd,p*ss=%Zd|",p,mult);
		mpz_mul(mult,mult,result); //
		while( j >= i-1 && mpz_cmp(m,mult) >= 0){
			mpz_sub(m,m,mult);
			j = j - 1;
			mpz_tdiv_q_ui(p,p,i);
			compute_n(result,i,j);
			mpz_mul(mult,p,ss);
			mpz_mul(mult,mult,result);
		}
		mpz_ui_pow_ui(mult,i,j-getFlags()[i-1] -1);
		mpz_mul(ss,ss,mult);
		getFlags()[i] = j;
	}
	//the rest

	long i  = getK() * getN() - 1;
	long j  = getN() - 1;

	while(j > 0){
		while(i >= getFlags()[j]){
			//s[i] = m %(j+1);
			mpz_tdiv_r_ui(result,m,j+1);

			getCanonicalString()[i] = mpz_get_si(result);
			//m = m / (j+1);
			mpz_tdiv_q_ui(m,m,j+1);
			i = i -1;

		}
		i = i - 1;
		j = j - 1;
	}

	//now we have to apply the flags (sum them up)
	for(i = 0; i < getN(); i++){
		getCanonicalString()[getFlags()[i]] = i;
	}


}

}
