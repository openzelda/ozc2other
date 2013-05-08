#ifndef _ELIX_STRING_H_
#define _ELIX_STRING_H_

#include "elix_intdef.h"

namespace elix {
	namespace string {
		void Replace( std::string & str, std::string find, std::string replace );
		void Split( std::string str, std::string delim, std::list<std::string> * results );
		void Trim( std::string * source );
		uint32_t Hash( std::string str );
	}
}

#endif

