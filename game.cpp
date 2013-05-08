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
#include <ctime>
#include <Windows.h>
#include <Rpc.h>
#include <sstream>
#include "elix_string.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void append_number(std::string & config, uint32_t value)
{
	std::ostringstream stream("");
	stream << value;
	config.append(stream.str());
}


namespace mokoi
{
	std::string path = "./test/"; 
	std::string name = "test"; 

    char * directories[] = {
    	"dialog",
    	"doc", 
    	"lang", 
    	"maps", 
    	"music", 
    	"masks", 
    	"scripts",
    	"scripts/maps", 
    	"scripts/routines", 
    	"sections", 
    	"soundfx",
    	"sprites",
    	"preload",
    	"paths",
    	NULL
    };

    char * files[] = {
		"lang/00.txt", "STR_HELLO",
    	"dialog/00.txt", "MSG_HELLO", 
    	"lang/en.txt", "Hello",
    	"dialog/en.txt", "Hello", 
    	"preload/audio.txt", "", 
    	"preload/entities.txt", "main", 
    	"scripts/main.mps", "main()\n{\n}",
    	"preload/spritesheet.txt", "",
   		NULL
    };

	std::string rename_sheet( std::string name )
	{
		elix::string::Replace( name, "_", "" );
		elix::string::Replace( name, "Sheet", "" );
		return name;
	}
	
	void new_copy()
	{
	    uint8_t count = 0;
		while ( mokoi::directories[count] != NULL )
		{
			path_create( mokoi::path + "/" + mokoi::directories[count] );
			count++;
		}
		
		count = 0;
    	while ( mokoi::files[count] != NULL)
		{
		    file_write( mokoi::path + "/" + mokoi::files[count++], mokoi::files[count], strlen(mokoi::files[count]) );
			count++;
		}

		std::string config = "[Mokoi]\n";
		config.append("project.title=" + mokoi::name + "\n");

		/* User Name */
        uint32_t len = 42;
		char buffer[43];

		if ( GetUserName(buffer, (LPDWORD)&len) )
    	{
    		config.append("project.author=");
    		config.append(buffer);
    		config.append("\n");
    	}
		else
			config.append("project.author=Unknown\n");
		
		time_t now = time(NULL);

		config.append("project.creation=");
		append_number(config, now);
		config.append("\n");

		UUID Uuid;
		UuidCreate( &Uuid );

		config.append("project.id=");
		append_number(config, Uuid.Data1);
		config.append("\n");

		config.append("map.width=640\n");
        config.append("map.height=480\n");
        config.append("screen.width=640\n");
        config.append("screen.height=480\n");
        config.append("display.width=640\n");
        config.append("display.height=480\n");
		config.append("display.height=480\n");
		config.append("sprite.shortname=true\n");
		

		file_write( mokoi::path + "/game.mokoi", (char*)config.c_str(), config.length() );
		file_write( mokoi::path + "/edit.zelda", (char*)config.c_str(), config.length() );
	}

	bool save(std::string path, std::string name)
	{
		mokoi::path = path;
		mokoi::name = name;
		std::cout << "mokoi::save: " << mokoi::path << " - " << mokoi::name << std::endl;
		mokoi::new_copy();

		std::stringstream world_buffer;
		if ( oz::hasMaps() )
		{
			std::list<object> objects;
			uint32_t x,y,w,h;
			uint32_t c = 0;
			while ( oz::mapDetails( objects, x, y, w, h) )
			{
				if (x > 63 || y > 63)
				{
					std::cout << "mokoi::save: mokoi games only support 64x64 section." << std::endl;
				}
				else
				{
					world_buffer << c << "\t" << x << "\t" << y << std::endl;
				}
				
			
				std::stringstream map_buffer;
                map_buffer << "<map xmlns=\"http://mokoi.info/projects/mokoi\">" << std::endl;
                map_buffer << "<settings>" << std::endl;
                map_buffer << "<dimensions width=\"" << w << "\" height=\"" << h << "\" />" << std::endl;
                map_buffer << "<color red=\"0\" blue=\"0\" green=\"0\" mode=\"0\" />" << std::endl;
                map_buffer << "</settings>" << std::endl;
				if ( objects.size() )
				{
    				for( std::list<object>::iterator objects_iter = objects.begin(); objects_iter != objects.end();objects_iter++ )
    				{
    					map_buffer << "<object value=\"" << rename_sheet((*objects_iter).name) << "\" type=\"sprite\">" << std::endl;
						map_buffer << "<color red=\"255\" blue=\"255\" green=\"255\" alpha=\"255\" />" << std::endl;
    					map_buffer << "<position x=\"" << (*objects_iter).rect.x << "\" y=\"" << (*objects_iter).rect.y << "\" ";
    					map_buffer << "w=\"" << (*objects_iter).rect.w << "\" h=\"" << (*objects_iter).rect.h << "\" z=\"" << (*objects_iter).rect.z << "\" />" << std::endl;
    					if ( (*objects_iter).entity.length() )
    					{
    						map_buffer << "<entity value=\"" << (*objects_iter).entity << ".zes\"/>" << std::endl;
    						map_buffer << "<setting key=\"entity\" value=\"" << (*objects_iter).entity << ".zes.mps\"/>" << std::endl;
    					}

    					map_buffer << "</object>" << std::endl;
    				}
				}
				map_buffer << "</map>";
					
				std::stringstream name_buffer;
				name_buffer << mokoi::path << "/maps/" << c << ".xml";
				file_write( name_buffer.str(), (char*)map_buffer.str().c_str(), map_buffer.str().length() );
				
				c++;
			}
			file_write( mokoi::path + "/sections/Converted.txt", (char*)world_buffer.str().c_str(), world_buffer.str().length() );
		}


		if ( oz::sprites.size() )
		{
			/*
			struct sprite
            {
            	std::string name;
            	std::string mask;
            	std::string parent;
            	double timer;
            	uint32_t flag;
            	position rect;
            };
            */
			std::stringstream sheet_buffer;
			for( std::list<sprite>::iterator sprite_iter = oz::sprites.begin(); sprite_iter != oz::sprites.end();sprite_iter++ )
    		{
				//#Sprite Name, Flag, Mask Value/File, Default Entity, X, Y, Width, Height
				sheet_buffer << rename_sheet((*sprite_iter).name) << "\t" << 0 << "\t" << (*sprite_iter).mask << "\t" << ((*sprite_iter).flag == 1 ? (*sprite_iter).name : "") << "\t";
				sheet_buffer << (*sprite_iter).rect.x << "\t" << (*sprite_iter).rect.y << "\t" << (*sprite_iter).rect.w << "\t" << (*sprite_iter).rect.h << std::endl;
				file_append( mokoi::path + "/sprites/" + rename_sheet( (*sprite_iter).parent ) + ".png.txt", (char*)sheet_buffer.str().c_str(), sheet_buffer.str().length() );
				
				saveImage((*sprite_iter).parent );
				sheet_buffer.str("");
			}
		}
		copyEntities();
		return true;
	}
	
	
	bool saveImage(std::string name)
	{
		uint32_t * pixel;
		int width, height;
		std::stringstream name_stream;
		name_stream << mokoi::path << "/sprites/" << rename_sheet(name) << ".png";
	
		if ( !file_exist( name_stream.str() ) )	
		{
			pixel = oz::getImage( name+ ".bmp", width, height, 2);
			if ( pixel )
			{
				stbi_write_png( name_stream.str().c_str(), width, height, 4, pixel, width*4);
			}
			else
			{
				std::cout << "mokoi::save: " << name <<  ".bmp failed to copy" << std::endl;
			}
		}
		return true;
	}
	
	bool copyEntities()
	{
		file_copy(root+"routines/animation.inc", mokoi::path + "/scripts/routines/animation.inc");
		file_copy(root+"routines/counter.inc", mokoi::path + "/scripts/routines/counter.inc");
		file_copy(root+"routines/enhanced.inc", mokoi::path + "/scripts/routines/enhanced.inc");
		file_copy(root+"routines/entity.inc", mokoi::path + "/scripts/routines/entity.inc");
		file_copy(root+"routines/general.inc", mokoi::path + "/scripts/routines/general.inc");
		file_copy(root+"routines/game_default.inc", mokoi::path + "/scripts/routines/game_default.inc");
		
		if ( oz::hasEntities() )
		{
			std::string name;
			while ( oz::nextEntities(name) )
			{
				std::cout << "mokoi::save: Coping " << name << std::endl;
				file_copy( name, mokoi::path + "/scripts/" + GetNameFromPath(name) + ".mps");
			}
			
		}
		
	}
	
}	
