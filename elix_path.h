#ifndef _ELIX_PATH_H_
#define _ELIX_PATH_H_

#define ELIX_DIR_SEPARATOR '/'
#define ELIX_DIR_SSEPARATOR "/"

#include <list>
#include <string>
namespace elix {
	namespace path {
		std::string GetBase( std::string path, bool trailing );
		std::string GetName( std::string path );
		bool Valid( std::string path );
		bool Documents( std::string * dir, bool shared );
		std::string User( std::string path );
		std::string Cache();
		std::string Create( std::string path );
		bool Children( std::string path, std::string subpath, std::list<char *> * list, bool deep, bool storepath, bool storedirectories );
	}
}

#endif
