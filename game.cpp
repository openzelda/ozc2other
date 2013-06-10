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

#if __GNUWIN32__
#include <Windows.h>
#include <Rpc.h>
#else
#include <uuid/uuid.h>

#endif

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


	std::string unwanted_prefixes[] = {
		"_cavedun",
		"_cave",
		"_icecavedun",
		"_icecave",
		"_Desert_",
		"_dwforest",
		"_outside",
		"_doutside_",
		"_doutside",
		"_rock",
		"_swamp_",
		"_castle",
		"_pyramid",
		"_town",
		"_dtown",
		"grate",
		"_sign",
		"o_hammer",
		"_tree",
		"_house",
		"snake",

		"d_dung",
		"d_dun",
		"_level9dun",
		"_level9"
		"_level8dun",
		"_level8"
		"level8_dun",
		"level8_",
		"level7_dun",
		"level7_",
		"_level6dun",
		"_level6",
		"_level5dun",
		"_level5",

		"_level4dun",
		"_level4",
		"_level3dun",
		"_level3",
		"_level2dun",
		"_level2",
		"_level2dun",
		"_level2",
		"_level1dun",
		"_level1",
		"_levelAdun",
		"_levelA",
		"_levelCdun",
		"_levelC",
		"_levelBdun",
		"_levelB",

		"_gdun",
		"_building",
		"icepalo_ice",
		""
	};

	std::string rename_sprite( std::string name )
	{
		uint32_t c = 0;

		//std::cout << "Sprite rename  from '" << name << "'";

		while ( unwanted_prefixes[c].length() )
		{
			if ( elix::string::RemovePrefix( name, unwanted_prefixes[c], 1 ) )
				break;
			c++;
		}
		elix::string::RemovePrefix( name, "_", 2 );

		//std::cout << "to '" << name << "'" << std::endl;
		return name;
	}

	std::string rename_sheet( std::string name )
	{
		//std::cout << "Sheet rename  from '" << name << "'";

		elix::string::RemovePrefix( name, "_", 1 );
		elix::string::Replace( name, "Sheet", "" );

		//std::cout << "to '" << name << "'" << std::endl;
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
		config.append("project.author=");
		config.append( get_username() );
		config.append("\n");



		time_t now = time(NULL);

		config.append("project.creation=");
		append_number(config, now);
		config.append("\n");

#ifdef WIN32
		UUID Uuid;
		UuidCreate( &Uuid );

		config.append("project.id=");
		append_number(config, Uuid.Data1);
		config.append("\n");
#else
		char * uuid_text = new char[36];
		uuid_t Uuid;
		uuid_generate(Uuid);

		uuid_unparse(Uuid, uuid_text);
		config.append("project.id=");
		config.append( uuid_text );
		config.append("\n");

		delete [] uuid_text;
#endif


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
					world_buffer << c << "\t" << x << "\t" << y << "\t" << 1 << "\t" << 1 << std::endl;
				}


				std::stringstream map_buffer;
				map_buffer << "<map xmlns=\"http://mokoi.info/projects/mokoi\">" << std::endl;
				map_buffer << "\t<settings>" << std::endl;
				map_buffer << "\t\t<dimensions width=\"" << w << "\" height=\"" << h << "\" />" << std::endl;
				map_buffer << "\t\t<color red=\"0\" blue=\"0\" green=\"0\" mode=\"0\" />" << std::endl;
				map_buffer << "\t</settings>" << std::endl;
				if ( objects.size() )
				{
					for( std::list<object>::iterator objects_iter = objects.begin(); objects_iter != objects.end();objects_iter++ )
					{
						object o = (*objects_iter);
						map_buffer << "\t<object value=\"" << rename_sheet(o._sheet) << ":" <<  rename_sprite(o._sprite) << "\" type=\"sprite\">" << std::endl;
						map_buffer << "\t\t<color red=\"255\" blue=\"255\" green=\"255\" alpha=\"255\" />" << std::endl;
						map_buffer << "\t\t<position x=\"" << o.rect.x << "\" y=\"" << o.rect.y << "\" ";
						map_buffer << "w=\"" << o.rect.w << "\" h=\"" << o.rect.h << "\" z=\"" << o.rect.z*1000 << "\" l=\"" << o.rect.z << "\" />" << std::endl;
						if ( (*objects_iter).entity.length() )
						{
							/* <entity value="%s" language="%s" global="%s"/> */
							map_buffer << "\t\t<entity value=\"" << o.entity << "\" language=\"mps\" global=\"false\"/>" << std::endl;
						}
						map_buffer << "\t</object>" << std::endl;
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

			std::map<std::string, sheet*> sheet_listing;
			std::map<std::string, sheet*>::iterator current_sheet_iter;

			for( std::list<sprite>::iterator sprite_iter = oz::sprites.begin(); sprite_iter != oz::sprites.end(); sprite_iter++ )
			{
				/*
				 *	<sheet xmlns="http://mokoi.info/format/sheet">
					<sprite name="process_0" hidden="hidden" mask="" entity="">
						<position x="74" y="38" w="16" h="16" />
					</sprite>
					<sprite name="process_1" mask="" entity="">
						<position x="74" y="38" w="16" h="16" />
						<child name="process_0" position="0" repeat="0"/>
						<collision id="0" x="0" y="0" w="16" h="16"/>
					</sprite>
					<animation name="process_0" hidden="hidden" mask="" entity="">
						<frame sprite="process_0" x="0" y="0" ms="100"/>
						<frame sprite="process_1" x="0" y="0" ms="100"/>
						<frame sprite="process_2" x="0" y="0" ms="100"/>
					</animation>
				</sheet>
				*/
				sheet * current_sheet = NULL;

				current_sheet_iter = sheet_listing.find( (*sprite_iter).parent  );
				if ( current_sheet_iter == sheet_listing.end() )
				{
					current_sheet = new sheet();
					current_sheet->name = (*sprite_iter).parent;

					sheet_listing[current_sheet->name] = current_sheet;
				}
				else
				{
					current_sheet = current_sheet_iter->second;
				}

				if ( (*sprite_iter).timer )
				{

				}
				else
				{
					std::string m = (*sprite_iter).mask;
					if ( m == "0" )
					{
						m.clear();
					}
					else
					{
						int mask_w = 0, mask_h= 0, mask_value = 0;
						if ( sscanf( m.c_str(), "%dx%dx%d", &mask_w, &mask_h, &mask_value ) == 3 )
						{

							m.clear();
							append_number(m, 255 - mask_value);
							std::cout << (*sprite_iter).mask << " > " << m << std::endl;
						}
					}




					current_sheet->contents << "\t<sprite name=\"" << rename_sprite((*sprite_iter).name) << "\" mask=\"" << m << "\"" << ((*sprite_iter).flag == 1 ? "entity=\""+(*sprite_iter).name+"\"" : "") << ">" << std::endl;
					current_sheet->contents << "\t\t<position x=\"" << (*sprite_iter).rect.x << "\" y=\"" << (*sprite_iter).rect.y << "\" w=\"" << (*sprite_iter).rect.w << "\" h=\"" << (*sprite_iter).rect.h << "\" />" << std::endl;
					current_sheet->contents << "\t</sprite>" << std::endl;
				}
			}

			for( current_sheet_iter = sheet_listing.begin(); current_sheet_iter != sheet_listing.end(); current_sheet_iter++ )
			{
				sheet * current_sheet = current_sheet_iter->second;

				std::string content;


				content.append( "<sheet xmlns=\"http://mokoi.info/format/sheet\">\n" );
				content.append( current_sheet->contents.str() );
				content.append( "</sheet>" );

				saveImage( current_sheet->name );
				file_write( mokoi::path + "/sprites/" + rename_sheet( current_sheet->name ) + ".png.xml", (char*)content.c_str(), content.length() );

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
				std::string file_path = GetNameFromPath(name);
				elix::string::Replace( file_path, ".zes", ".mps" );
				std::cout << "mokoi::save: Coping " << name << std::endl;
				file_copy( name, mokoi::path + "/scripts/" + file_path );
			}

		}

	}

}
