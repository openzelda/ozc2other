/***********************************************
 * Copyright © Luke Salisbury
 *
 * You are free to share, to copy, distribute and transmit this work
 * You are free to adapt this work
 * Under the following conditions:
 *  You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
 *  You may not use this work for commercial purposes.
 * Full terms of use: http://creativecommons.org/licenses/by-nc/3.0/
 * Changes:
 *     2010/12/11 [luke]: new file.
 ***********************************************/

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>

#define STBI_HEADER_FILE_ONLY
#define STBI_NO_HDR 1
#include "stb_image.c"

struct position
{
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
	uint32_t z;
};

struct rectangle
{
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
};

struct sprite
{
	std::string name;
	std::string mask;
	std::string parent;
	double timer;
	uint32_t flag;
	position rect;
};

struct sheet
{
	std::string name;
	std::stringstream contents;

};


struct object
{
	std::string name;
	std::string _sprite;
	std::string _sheet;
	std::string entity;
	uint32_t flag;
	position rect;
};

int32_t file_copy(std::string src, std::string dest);
int32_t file_exist(std::string src);
bool str_has_suffix(const char * str, const char * suffix);
void path_create(std::string folder);
size_t file_write( std::string file, char * contents, size_t length );
size_t file_append( std::string filename, char * contents, size_t length );
uint8_t * loadRLEBMP( const char *file, int &w, int &h );
std::string GetNameFromPath( std::string path );

extern std::string root;

namespace oz
{
	extern std::string name;
	extern std::list<sprite> sprites;

	bool load(std::string path, std::string name);
	uint32_t hasMaps();
	bool mapDetails( std::list<object> & objects, uint32_t &x, uint32_t & y, uint32_t &w, uint32_t &h  );
	uint32_t * getImage(std::string name, int & width, int & height, int scale );


	bool hasEntities();
	bool nextEntities( std::string & name);

}

namespace mokoi
{
	bool save(std::string path, std::string name);
	bool saveImage(std::string name);
	bool copyEntities();
}
