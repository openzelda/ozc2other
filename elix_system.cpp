#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include "elix_path.h"
#include <cstdio>

#ifdef __GNUWIN32__
	#define _WIN32_IE 0x0400
	#include <io.h>
	#include <shlobj.h>
#endif

namespace elix {
	namespace string {
		bool RemovePrefix( std::string & str, std::string find, std::string::size_type max_remove  )
		{
			std::string::size_type found = str.find(find, 0);

			if ( found < max_remove )
			{
				if ( found == 0 )
				{
					max_remove = 1;
				}
				str.erase(0, find.size() >= max_remove ? find.size() : max_remove );
				return true;
			}
			return false;
		}
		void Replace( std::string & str, std::string find, std::string replace )
		{
			std::string::size_type look = 0;
			std::string::size_type found;

			while((found = str.find(find, look)) != std::string::npos)
			{
				str.replace(found, find.size(), replace);
				look = found + replace.size();
			}
		}
		void Split( std::string str, std::string delim, std::vector<std::string> * results )
		{
			std::string::size_type lastPos = str.find_first_not_of(delim, 0);
			std::string::size_type pos = str.find_first_of(delim, lastPos);

			while ( std::string::npos != pos || std::string::npos != lastPos )
			{
				results->push_back(str.substr(lastPos, pos - lastPos));
				lastPos = str.find_first_not_of(delim, pos);
				pos = str.find_first_of(delim, lastPos);
			}
		}
		void Trim( std::string * source )
		{
			if ( source->size() )
			{
				source->erase(source->find_last_not_of("\r\n")+1);
				source->erase(0, source->find_first_not_of("\r\n"));
			}
		}
		uint32_t Hash( std::string str )
		{
			uint32_t hash = 0;
			size_t i;

			for (i = 0; i < str.length(); i++)
			{
				hash += str[i];
				hash += (hash << 10);
				hash ^= (hash >> 6);
			}
			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);
			return hash;
		}
	}
	namespace path {
		#ifdef __GNUWIN32__
			char * _user = (char*)"\\luxengine\\";
		#else
			char * _user = (char*)"/.luxengine/";
		#endif
		std::string GetBase( std::string path, bool trailing )
		{
			elix::string::Replace( path, "\\", "/" );
			int lastslash = path.find_last_of( ELIX_DIR_SEPARATOR, path.length() );
			if ( lastslash == -1 )
			{
				return "./";
			}
			if ( trailing )
			{
				lastslash += 1;
			}
			if ( lastslash >= 2 )
			{
				return path.substr( 0, lastslash );
			}
			return path;
		}

		std::string GetName( std::string path )
		{
			elix::string::Replace( path, "\\", "/" );
			int lastslash = path.find_last_of( ELIX_DIR_SEPARATOR, path.length() );
			if ( lastslash )
			{
				return path.substr( lastslash + 1 );
			}
			return path;
		}

		bool Valid( std::string path )
		{
			struct stat directory;
			if ( !stat( path.c_str(), &directory ) )
			{
				return S_ISDIR(directory.st_mode);
			}
			return false;
		}


		std::string User( std::string path )
		{
			std::string full_directory = "./";
			bool valid = true;

			#ifdef __GNUWIN32__
			char directory[MAX_PATH];
			valid = SHGetSpecialFolderPath(NULL, directory, CSIDL_APPDATA, 0);
			if ( valid)
				full_directory = directory;
			#elif defined(__NDS__)
			full_directory = "/MokoiGaming/";
			#else
			char * home_path = NULL;
			home_path = getenv( "XDG_DATA_HOME" );
			if ( !home_path )
			{
				home_path = getenv( "HOME" );
			}
			if ( home_path )
				full_directory = home_path;
			else
				full_directory = "~";
			#endif

			full_directory += _user;

			if ( valid )
			{
				if ( !Valid( full_directory ) )
				{
					#ifdef __GNUWIN32__
						mkdir(full_directory.c_str());
					#else
						mkdir(full_directory.c_str(), 0744);
					#endif
				}
				if ( path.length() )
				{
					size_t pos;
					while( (pos = path.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_")) != std::string::npos )
						path.erase( pos, 1 );
					full_directory += path;
					full_directory.append(1, ELIX_DIR_SEPARATOR);
				}
				if ( !Valid( full_directory ) )
				{
				#ifdef __GNUWIN32__
					mkdir(full_directory.c_str());
				#else
					mkdir(full_directory.c_str(), 0744);
				#endif
				}
			}

			return full_directory;
		}

		bool Documents( std::string * dir, bool shared )
		{
			bool valid = true;
			#ifdef __GNUWIN32__
			char directory[MAX_PATH];
			//valid = SHGetFolderPath(NULL, FOLDERID_Documents, NULL, 0,  directory);
			valid = SHGetSpecialFolderPath(NULL, directory, (shared ? CSIDL_COMMON_DOCUMENTS : CSIDL_PERSONAL), 0);
			if ( valid )
				dir->assign(directory);
			#else
			char * home_path = NULL;
			home_path = getenv( "XDG_DATA_HOME" );
			if ( !home_path )
			{
				home_path = getenv( "HOME" );
			}
			if ( home_path )
				dir->assign(home_path);
			else
				dir->assign("~/");
			#endif

			return valid;
		}

		std::string Cache()
		{
			#ifdef __NDS__
				std::string full_directory = "/MokoiGaming/";
				return full_directory;
			#else
				std::string full_directory = User("cache");
				return full_directory;
			#endif
		}

		std::string Create( std::string path )
		{
			size_t pos;
			while( (pos = path.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_")) != std::string::npos )
				path.erase( pos, 1 );

			std::string full_directory = User("") + path + ELIX_DIR_SSEPARATOR;
			#ifdef __GNUWIN32__
				mkdir(full_directory.c_str());
			#else
				mkdir(full_directory.c_str(), 0744);
			#endif
			return full_directory;
		}

		bool Children( std::string path, std::string subpath, std::list<char *> * list, bool deep, bool storepath, bool storedirectories )
		{
			dirent * entry = NULL;
			std::string dir_path = path + ELIX_DIR_SSEPARATOR + subpath;
			std::string file_path = path;
			DIR * dir = opendir( dir_path.c_str() );

			if ( !dir )
			{
				std::cerr << __FUNCTION__ << " Can't open '" << dir_path << "'." << std::endl;
				return false;
			}

			while ( (entry = readdir(dir)) != NULL )
			{
				if ( (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0) )
				{
					file_path = dir_path + ELIX_DIR_SSEPARATOR + entry->d_name;
					if ( elix::path::Valid( file_path ) )
					{
						if ( deep )
						{
							std::string dir_store(entry->d_name);
							Children( path, dir_store, list, false, storepath, storedirectories );
						}
						if ( storedirectories )
						{
							uint8_t flen;
							char * name_store = NULL;
							if ( storepath )
							{
								flen = snprintf( NULL, 0, "%s%c", file_path.c_str(), ELIX_DIR_SEPARATOR )+1;
								name_store = new char[flen];
								snprintf( name_store, flen, "%s%c", file_path.c_str(), ELIX_DIR_SEPARATOR);
							}
							else if ( subpath.length() )
							{
								flen = snprintf( NULL, 0, "%s%c%s%c", subpath.c_str(), '/', entry->d_name, '/' )+1;
								name_store = new char[flen];
								snprintf( name_store, flen, "%s%c%s%c", subpath.c_str(), '/', entry->d_name, '/' );
							}
							else
							{
								flen = strlen( entry->d_name )+1;
								name_store = new char[flen];
								snprintf( name_store, flen, "%s%c",  entry->d_name, '/' );
							}
							list->push_back( name_store );
						}
					}
					else
					{
						uint8_t flen;
						char * name_store = NULL;
						if ( storepath )
						{
							flen = snprintf( NULL, 0, "%s", file_path.c_str() )+1;
							name_store = new char[flen];
							snprintf( name_store, flen, "%s", file_path.c_str());
						}
						else if ( subpath.length() )
						{
							flen = snprintf( NULL, 0, "%s%c%s", subpath.c_str(), '/', entry->d_name )+1;
							name_store = new char[flen];
							snprintf( name_store, flen, "%s%c%s", subpath.c_str(), '/', entry->d_name );
						}
						else
						{
							flen = strlen( entry->d_name )+1;
							name_store = new char[flen];
							snprintf( name_store, flen, "%s",  entry->d_name );
						}
						list->push_back( name_store );
					}
				}
			}
			closedir(dir);
			return true;
		}
	}


}
