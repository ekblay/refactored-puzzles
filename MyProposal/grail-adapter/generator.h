#ifndef lisagenerator_h
#define lisagenerator_h

//class fm<char>;
#include <gmp.h>

namespace grail
{



class Generator{

public:

	enum GeneratorType{ RANDOM, SEQUENTIAL };
	static char char_alphabet[] ;

	///////////////////////////////////////////////////////////////////
	//Constructor and deconstructor
	///////////////////////////////////////////////////////////////////
	Generator(int n, int k, GeneratorType type);
	~Generator();
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//Public methods
	///////////////////////////////////////////////////////////////////
	fm<char> 				next();		   //Sequenctional generator
	fm<char>*				next(mpz_t n); //Random generator
	fm<char>				current();
	bool					hasNext();
	void					reset();
	void 					getSize(mpz_t m);
	fm<char>				canonicalstring_to_fm();
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//Public inline methods
	///////////////////////////////////////////////////////////////////
	inline unsigned int 	getN()				 		{ return n_states; 		}
	inline unsigned int 	getK() 						{ return k_alphabets; 	}
	inline unsigned long	getTotalFinals()			{ return f_finals;		}
	inline unsigned long	getCurrentFinalStatesIndex(){ return current_final; }
	inline void				setCurrentFinalStatesIndex(unsigned long i) { current_final = i; }
	inline int*				getCanonicalString() 		{ return canonical_str; }
	inline int*				getFlags()					{ return flags; 		}
	inline int** 			getFinals()					{ return finals;		}
	///////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////
	////	Print methods: only for debugging purposes
	///////////////////////////////////////////////////////////////////
	void print_flags();
	void print_finalstates_configuration();
	void print_canonical_string();
	///////////////////////////////////////////////////////////////////



private:

	///////////////////////////////////////////////////////////////////
	// Private instance variables
	///////////////////////////////////////////////////////////////////
	unsigned int n_states; // Total number of states
	unsigned int k_alphabets; //Total number of alphabet
	unsigned long f_finals; //possible final states configuration
	unsigned long current_final; //current final configuration
	GeneratorType g_type;
	int *flags;
	int *canonical_str;
	int **finals; //Multi dimentional array

	gmp_randstate_t gmp_rand_state;
	///////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////
	// Private methods
	///////////////////////////////////////////////////////////////////
	void 	init_flags();
	void 	init_finals();
	void 	next_icdfa(unsigned long a, unsigned long b);
	void 	next_flags(int i);
	int 	is_flagged(int j);
	bool	is_last_final();
	bool 	is_full();
	bool 	is_last();
	void 	decimal_to_binary(int decimal);
	void 	compute_n(mpz_t result, long m,long j);
	void 	generate_from_index(mpz_t m);
	///////////////////////////////////////////////////////////////////


};

}
#endif
