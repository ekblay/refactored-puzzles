#ifndef LISA_GRAIL_INCLUDE_H
#define LISA_GRAIL_INCLUDE_H

int  debug=0;
char* nfatodfca_name=""; //for special debug


#include "../../grail/char/include.h"
#include "../../grail/char/lexical.h"


#include "dfa.h"
#include "dfa.cc"


#include "nfa.h"
#include "nfa.cc"


#include "io.h"
#include "io.cc"

#include "generator.cc"
#include "generator.h"


#include "re.h"
#include "re.cc"


#endif
