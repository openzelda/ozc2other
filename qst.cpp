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
 *	 2010/12/11 [luke]: new file.
 ***********************************************/
#include "header.hpp"
#include "elix_path.h"

enum {
	QSS_INTRO,
	QSS_SCREENCOUNT,
	QSS_SCREEN,
	QSS_GROUPCOUNT,
	QSS_GROUP,
	QSS_SETTING
};

enum {
	SPT_INTRO,
	SPT_TYPE,
	SPT_INFO,
	SPT_ANIMATEDINFO
};

enum {
	LND_SPRITECOUNT,
	LND_SPRITE,
	LND_TILECOUNT,
	LND_TILE,
	LND_EXIT
};

namespace oz
{
	std::string path;
	std::string name;

	std::list<sprite> sprites;
	std::list<rectangle> maps;
	std::list<rectangle>::iterator maps_iter;

	/* Settings */
	uint32_t startx = 0;
	uint32_t starty = 0;
	uint32_t useaudio = 0;
	std::string title;

	std::string sprite_match_name;
	bool sprite_match( sprite i )
	{
		return sprite_match_name.compare(i.name) == 0;
	}

	sprite find_sprite( std::string name )
	{
		sprite s;
		if ( name.length() )
		{
			sprite_match_name = name;
			std::list<sprite>::iterator iter;
			iter = std::find_if( oz::sprites.begin(), oz::sprites.end(), sprite_match );
			if( iter != oz::sprites.end() )
			{
				s = *iter;
			}
		}
		return s;
	}

	void ScanQSS()
	{
		std::string file_name = oz::path + "save/" + oz::name + ".qss";
		std::string buffer;
		std::ifstream file( file_name.c_str());

		uint32_t line_count = 0;
		uint32_t screen_count = 0;
		uint32_t group_count = 0;
		uint32_t mode = 0;

		rectangle group;
		std::string current_group;
		uint32_t temp_convert;

		do
		{
			std::getline (file, buffer);
			switch(mode)
			{
				case QSS_INTRO:
					mode = QSS_SCREENCOUNT;
				break;
				case QSS_SCREENCOUNT:
					screen_count = atoi(buffer.c_str());
					mode = QSS_SCREEN;
				break;
				case QSS_SCREEN:
					if ( line_count == 0)
					{
						line_count++;
					}
					else if ( line_count == 1)
					{
						line_count++;
					}
					else
					{
						screen_count--;
						line_count = 0;
						if (!screen_count)
							mode = QSS_GROUPCOUNT;
					}
				break;
				case QSS_GROUPCOUNT:
					group_count = atoi(buffer.c_str());
					mode = QSS_GROUP;
				break;
				case QSS_GROUP:
					if ( line_count == 0)
					{
						current_group = buffer;
						line_count++;
					}
					else if ( line_count == 1)
					{
						group.x = atoi(buffer.c_str()) / 128;
						line_count++;
					}
					else if ( line_count == 2)
					{
						group.y = atoi(buffer.c_str()) / 96;
						line_count++;
					}
					else if ( line_count == 3)
					{
						group.w = atoi(buffer.c_str()) / 128;
						if (group.w < group.x)
						{
							temp_convert = group.x;
							group.x = group.w;
							group.w = temp_convert - group.x;
						}
						else
						{
							group.w = (atoi(buffer.c_str())/128) - group.x;
						}
						line_count++;
					}
					else if ( line_count == 4)
					{
						group.h = atoi(buffer.c_str())/96;
						if (group.h < group.y)
						{
							temp_convert = group.y;
							group.y = group.h;
							group.h = temp_convert - group.y;
						}
						else
						{
							group.h = (atoi(buffer.c_str())/96) - group.y;
						}
						line_count++;
					}
					else if ( line_count == 5)
					{
						line_count++;
					}
					else
					{
						oz::maps.push_back(group);
						group_count--;
						line_count = 0;
						if (!group_count)
							mode = QSS_SETTING;
					}

				break;
				case QSS_SETTING:
					if (line_count == 0)
						oz::startx = atoi(buffer.c_str());
					else if (line_count == 1)
						oz::starty = atoi(buffer.c_str());
					else if (line_count == 2)
						oz::title = buffer;
					else if (line_count == 3)
					{
					}
					else if (line_count == 4)
						oz::useaudio = atoi(buffer.c_str());

					line_count++;
				break;
			}
		} while (! file.eof());
		std::cout << "oz::scanGroups: " << oz::maps.size() << std::endl;
	}

	void ParseSPT( std::string file )
	{
		std::ifstream sptfile(file.c_str());

		uint32_t line_count = 0;
		uint32_t anim_frames, anim_frame_count = 0;
		uint32_t mode = SPT_INTRO;
		uint32_t qq;
		sprite s;

		std::string buffer;
		std::stringstream anim_buffer;
		do
		{
			std::getline( sptfile, buffer );
			switch(mode)
			{
				case SPT_INTRO:
					mode = SPT_TYPE;
					break;
				case SPT_TYPE:
					if ( buffer == "[ENTITY]")
					{
						s.flag = 1;
					}
					else if ( buffer == "[BACKGROUND]")
					{
					}
					else
					{
						s.name = buffer;

						s.parent = elix::path::GetName(file);
						s.parent = s.parent.substr( 0, s.parent.find_last_of( '.', s.parent.length() ) );
						mode = SPT_INFO;
					}
					break;
				case SPT_INFO:
					qq = atoi(buffer.c_str());
					if (qq == 10 )
						std::cout << "Error:" <<__LINE__ << std::endl;

					if (buffer == "ANIMATED")
					{
						line_count = 0;
						mode = SPT_ANIMATEDINFO;
						break;
					}
					if ( line_count == 0)
					{
						s.rect.x = atoi(buffer.c_str()) * 2;
						line_count++;
					}
					else if ( line_count == 1)
					{
						s.rect.y = atoi(buffer.c_str()) * 2;
						line_count++;
					}
					else if ( line_count == 2)
					{
						s.rect.w = atoi(buffer.c_str()) * 2;
						line_count++;
					}
					else if ( line_count == 3)
					{
						s.rect.h = atoi(buffer.c_str()) * 2;
						line_count++;
					}
					else if ( line_count == 4)
					{
						if (buffer != "[FILL]")
						{
							s.mask = (buffer.length() ? buffer : "0");
							line_count++;
						}
					}
					else if ( line_count == 5)
					{
						if (buffer == "TRUE")
						{
							s.flag += 2;
						}
						line_count++;
					}
					else if ( line_count == 6)
					{
						//don't care (alpha)
						line_count++;
					}
					else
					{
						if (buffer == "[EE]")
						{
							s.timer = 0.0;
							oz::sprites.push_back(s);
							line_count = 0;
							s.flag = 0;
							mode = SPT_TYPE;
						}
					}
				break;
				case SPT_ANIMATEDINFO:
					if ( line_count == 0)
					{
						anim_frame_count = 0;
						anim_frames = atoi(buffer.c_str());
						line_count++;
					}
					else if ( line_count == 1)
					{
						s.timer = atof(buffer.c_str());
						line_count++;
					}
					else if ( line_count == 2)
					{
						s.rect.x = atoi(buffer.c_str()) * 2;
						line_count++;
					}
					else if ( line_count == 3)
					{
						s.rect.y = atoi(buffer.c_str()) * 2;
						line_count++;
					}
					else if ( line_count == 4)
					{
						s.rect.w = atoi(buffer.c_str()) * 2;
						line_count++;
					}
					else if ( line_count == 5)
					{
						s.rect.h = atoi(buffer.c_str()) * 2;
						anim_frame_count++;
						//std::cout << name << "_f" << anim_frame_count << "\t" << obj.rect.x << "\t" << obj.rect.y << "\t" << obj.rect.w << "\t" << obj.rect.h  << "\t" << (0) << "\t0" << std::endl;



						if (anim_frame_count != anim_frames)
							line_count = 2;
						else
							line_count = 6;
					}
					else if ( line_count == 6)
					{
						s.mask = (buffer.length() ? buffer : "0");
						line_count++;
					}
					else if ( line_count == 7)
					{
						if (buffer == "TRUE")
						{
							s.flag += 2;
						}
						line_count++;
					}
					else if ( line_count == 8)
					{
						//don't care (alpha)
						line_count++;
					}
					else  if ( line_count == 9)
					{
						if (buffer == "[EE]")
						{
							/*
							if (anim_frames > 0)
							{
								for(int n = 1; n <= anim_frames; n++)
								{
									outfile << name << "_f" << n << "\t";
								}
							}
							else
							{
								outfile << "NULL\t";
							}
							outfile << (0) << "\t0\n";
							*/
							s.parent = elix::path::GetName(file);
							s.parent = s.parent.substr( 0, s.parent.find_last_of( '.', s.parent.length() ) );
							oz::sprites.push_back(s);


							anim_frames = 0;
							line_count = 0;
							s.flag = 0;
							mode = SPT_TYPE;
						}
					}
					else
					{
						line_count = 0;
					}
				break;
			}
		} while (! sptfile.eof());
	}

	void ScanSprites()
	{
		std::list<char *> list;
		elix::path::Children( oz::path, "sprite sheets/", &list, false, true, false );
		for ( std::list<char *>::iterator it = list.begin(); it != list.end(); it++)
		{
			if ( str_has_suffix(*it, ".spt") )
			{
				std::string file(*it);
				oz::ParseSPT(file);
			}
		}
		std::cout << "oz::scanSprites: " << oz::sprites.size() << std::endl;
		//for ( std::list<sprite>::iterator q = oz::sprites.begin(); q != oz::sprites.end(); q++)
		//	std::cout << "sprite: " << (*q).parent << ":" << (*q).name << std::endl;
	}

	void ParseLND(std::list<object> & objects, uint8_t x, uint8_t y, uint8_t offset_x, uint8_t offset_y)
	{
		std::stringstream name_stream;
		name_stream << oz::path << "questdata/" << oz::name << "/screens/" << (int)x << "-" << (int)y << ".lnd";

		std::string buffer;
		std::ifstream lndfile( name_stream.str().c_str());
		uint32_t sprite_count = 0;
		uint32_t fill_count = 0;
		uint32_t line_count = 0;
		uint32_t mode = LND_SPRITECOUNT;
		uint32_t qq =0;
		object obj;
		do
		{
			std::getline(lndfile, buffer);
			switch(mode)
			{
				case LND_SPRITECOUNT:
					sprite_count = atoi(buffer.c_str());
					mode = sprite_count > 0 ? LND_SPRITE : LND_TILECOUNT;
				break;
				case LND_SPRITE:
					qq = atoi(buffer.c_str());
					if (qq == 10 )
						std::cout << "Error:" <<__LINE__ << std::endl;
					if ( line_count == 0)
					{
						obj.rect.x = atoi(buffer.c_str()) + (offset_x*640);
						line_count++;
					}
					else if ( line_count == 1)
					{
						obj.rect.y = atoi(buffer.c_str()) + (offset_y*480);
						line_count++;
					}
					else if ( line_count == 2)
					{
						obj.entity = "";
						obj.name = buffer;
						line_count++;
					}
					else
					{
						obj.rect.w = obj.rect.h = 0;
						obj.rect.z = 1;
						sprite s = oz::find_sprite(obj.name);
						if ( s.name.length() )
						{
							obj._sprite = s.name;
							obj._sheet = s.parent + ".png";
							if ( s.flag >= 2 )
								obj.rect.z = 5;
							if ( s.flag == 1 || s.flag == 3 )
								obj.entity = s.name;
							obj.rect.w = s.rect.w;
							obj.rect.h = s.rect.h;
						}
						else
						{
							obj._sprite = obj.name;
							obj._sheet = "unknown.png";
							obj.rect.w = 0;
							obj.rect.h = 0;
						}

						objects.push_back(obj);
						sprite_count--;
						line_count = 0;
						if (!sprite_count)
							mode = LND_TILECOUNT;
					}
				break;
				case LND_TILECOUNT:
					fill_count = atoi(buffer.c_str());
					mode = fill_count > 0 ? LND_TILE : LND_EXIT;
				break;
				case LND_TILE:
					if ( line_count == 0)
					{
						obj.rect.x = atoi(buffer.c_str()) + (offset_x*640);
						line_count++;
					}
					else if ( line_count == 1)
					{
						obj.rect.y = atoi(buffer.c_str()) + (offset_y*480);
						line_count++;
					}
					else if ( line_count == 2)
					{
						obj.rect.w = (atoi(buffer.c_str()) + (offset_x*640)) - obj.rect.x;
						line_count++;
					}
					else if ( line_count == 3)
					{
						obj.rect.h = (atoi(buffer.c_str()) + (offset_y*480)) - obj.rect.y;
						line_count++;
					}
					else
					{
						obj.entity = "";
						obj.name = buffer;
						obj.rect.z = 0;
						sprite s = oz::find_sprite(obj.name);
						if ( s.name.length() )
						{
							obj._sprite = s.name;
							obj._sheet = s.parent + ".png";
							if ( s.flag >= 2 )
								obj.rect.z = 5;
							if ( s.flag == 1 || s.flag == 3 )
								obj.entity = s.name;
						}
						else
						{
							obj._sprite = obj.name;
							obj._sheet = "unknown.png";
							obj.rect.w = 0;
							obj.rect.h = 0;
						}

						objects.push_back(obj);
						fill_count--;
						line_count = 0;
						if (!fill_count)
							mode = LND_EXIT;
					}
					break;
				default:
					break;
			}
		} while (! lndfile.eof());

	}

	uint32_t hasMaps()
	{
		oz::maps_iter = oz::maps.begin();
		return oz::maps.size();
	}

	bool mapDetails( std::list<object> & objects, uint32_t &x, uint32_t & y, uint32_t &w, uint32_t &h )
	{
		if ( oz::maps_iter == oz::maps.end())
			return false;
		objects.clear();


		rectangle group = *oz::maps_iter;
		//std::cout << "oz::mapDetails: " << group.x << "x" << group.x << " " << group.w<< "x" << group.h << std::endl;
		for( uint8_t _x = 0; _x < group.w; _x++)
		{
			for( uint8_t _y = 0; _y < group.h; _y++)
			{
				ParseLND(objects, group.x+_x, group.y+_y, _x, _y);
			}
		}
		x = group.x;
		y = group.y;
		w = group.w;
		h = group.h;
		oz::maps_iter++;
		return true;
	}

	bool load(std::string path, std::string name)
	{
		oz::path = path;
		oz::name = name.substr( 0, name.find_last_of( '.', name.length() ) );
		std::cout << "oz::load: " << oz::path << " - " << oz::name << std::endl;
		oz::ScanQSS();
		oz::ScanSprites();
		return true;
	}

	/* Little Endian  AABBGGRR*/
	#define RGBA32(r, g, b, a) ( (r) | (a << 24) | ((b)<<16)|((g)<<8) )

	uint32_t getPixel( uint8_t * pixels, int32_t p )
	{
		uint8_t * pix = pixels;
		pix += (p*4);
		return (uint32_t)RGBA32(pix[0], pix[1], pix[2], pix[3]);
	}

	uint32_t * getImage(std::string name, int & width, int & height, int scale)
	{
		std::stringstream name_stream;
		name_stream << oz::path << "sprite sheets/" << name;
		stbi_convert_iphone_png_to_rgb(0);
		uint8_t * pixels = stbi_load(name_stream.str().c_str(), &width, &height, NULL, 4);
		if ( pixels == NULL )
		{
			pixels = loadRLEBMP(name_stream.str().c_str(), width, height);
		}

		if ( pixels == NULL )
		{
			std::cout << "oz::getImage" <<  "No Image Loaded" << std::endl;
			return NULL;
		}
		if ( width > 0 && height > 0)
		{
			int32_t c = 0;
			int32_t q = 0;
			int32_t size = width * height;
			int32_t line = width * 2;
			uint32_t * dpixels = new uint32_t[size*8];
			while ( c < size )
			{
				uint32_t pix = getPixel(pixels, c);
				if ( pix == 0xFF00FF00 ) /* Little Endian  AABBGGRR*/
				{
					pix = 0x00000000;
				}
				dpixels[q++] = pix;
				dpixels[q++] = pix;


				c++;
				if ( !(c % width) )
				{
					memcpy( dpixels + q, dpixels + q - line, line*4);
					q += line;
				}
			}
			width *=2;
			height *=2;
			return dpixels;
		}
		else
		{
			return NULL;
		}

	}

	static std::list<char *> entities_list;
	std::list<char *>::iterator entities_it;
	bool hasEntities()
	{

		elix::path::Children( oz::path, "entities/", &entities_list, false, true, false );
		entities_it = entities_list.begin();

		return !entities_list.empty();

	}
	bool nextEntities( std::string & name)
	{
		name.clear();
		if ( entities_it != entities_list.end() )
		{
			name.append(*entities_it);
			entities_it++;
			return true;
		}
		else
		{
			return false;
		}
	}



}





