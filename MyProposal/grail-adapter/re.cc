#include "re.h"

namespace grail
{

re<char> *RE::re_fromstring(std::string &str){
	re<char> *r = new re<char>();
	std::istringstream iss(str,std::istringstream::in);
	iss >> *r;
	return r;
}

}
