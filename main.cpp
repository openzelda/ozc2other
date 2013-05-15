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
#include <windows.h>
#include <Shlobj.h>
#include <io.h>
#include "elix_path.h"
#include "elix_string.h"

std::string root;

/* Functions */

int32_t file_copy(std::string src, std::string dest)
{
	int c;
	FILE * source_file, * dest_file;

	if ( file_exist(dest) )
	{
		std::cout << "file_exist(" << dest << ")" << std::endl;
		return 0;
	}

	source_file = fopen( src.c_str(), "rb" );

	if ( source_file )
	{
		dest_file = fopen( dest.c_str(),"wb" );
		if ( !dest_file )
		{
			std::cout << "file_exist(" << dest << ") failed" << std::endl;
			fclose(source_file);
			return 0;
		}
		while ((c = fgetc(source_file)) != EOF)
		{
			fputc(c, dest_file);
		}

		fclose( source_file );
		fclose( dest_file );
		return 1;
	}
	else
	{
		std::cout << "fopen(" << src << ") failed" << std::endl;
	}
	return 0;
}


int32_t file_exist(std::string src)
{
	std::ifstream f;
	f.open(src.c_str(), std::ios::binary | std::ios::in);
	if (!f.good() || f.eof() || !f.is_open()) { return 0; }
	return 1;
}

bool str_has_suffix(const char *str, const char *suffix)
{
	size_t str_length;
	size_t suffix_length;

	str_length = strlen(str);
	suffix_length = strlen(suffix);

	return suffix_length <= str_length ? strncmp(str + str_length - suffix_length, suffix, suffix_length) == 0 : false;
}

void path_create(std::string folder)
{
	mkdir( folder.c_str() );
}

size_t file_write( std::string filename, char * contents, size_t length )
{
	size_t n_written = 0;
	FILE * file = fopen( filename.c_str(), "wb" );
	if ( file )
	{
		n_written = fwrite( contents, 1, length, file);
	}
	fclose( file );
	return n_written;
}

size_t file_append( std::string filename, char * contents, size_t length )
{
	size_t n_written = 0;
	FILE * file = fopen( filename.c_str(), "ab" );
	if ( file )
	{
		n_written = fwrite( contents, 1, length, file);
	}
	fclose( file );
	return n_written;
}
std::string GetNameFromPath( std::string path )
{
	return elix::path::GetName(path);
}


#ifdef __GNUWIN32__
std::string file_open()
{
	std::string path;
	char szFileName[MAX_PATH] = "";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Classic Open Zelda Quest\0*.qss\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "qss";

	if ( GetOpenFileName(&ofn) )
	{
		path = szFileName;
	}
	return path;
}

std::string path_save()
{
	std::string path;
	char szFileName[MAX_PATH] = "";

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = "Pick a Directory to Save to";
	bi.ulFlags = BIF_USENEWUI | BIF_EDITBOX;
	LPITEMIDLIST pidl = SHBrowseForFolder( &bi );
	if ( pidl != 0 )
	{
		if ( SHGetPathFromIDList(pidl,szFileName) )
			path = szFileName;
	}
	return path;
}

void Message( std::string title, std::string message )
{
	MessageBox(NULL, message.c_str(), title.c_str(), MB_OK|MB_ICONINFORMATION|MB_TOPMOST );
}

#else
std::string file_open()
{
	return "";
}


std::string path_save()
{
	return "";
}

void Message( std::string title, std::string message )
{
	std::cout << message << std::endl;
}

#endif


int main (int argc, char *argv[])
{

	std::cout << "oz2other - Convert Open Zelda Classic Quest to another format" << std::endl;
	std::cout << "Supported Formats" << std::endl;
	std::cout << "\tMokoi Game (include Modern Open Zelda)" << std::endl;
	std::cout << "\t" << std::endl;

	root = elix::path::GetBase(argv[0], true);

	std::string path = file_open();
	if ( path.length() )
	{

		oz::load( elix::path::GetBase(elix::path::GetBase(path, false), true), elix::path::GetName(path) );

		std::string output_path;
		output_path = path_save();
		//output_path = "C:\\Users\\luke\\alchera\\TheJourneyBegins";
		if ( !output_path.empty() )
		{
			elix::string::Replace( output_path, "\\", "/" );
			mokoi::save ( output_path, oz::name );
			Message("ozc2other - Convert Open Zelda Classic Quest to another format","Your Open Zelda Classic Quest has been converted\nPlease note that Screen/Group scripts and images files have to be converted manually." );
		}
	}

	return 0;
}
/*
std::string MGM_GetFullSpriteName(std::string name)
{
	return MGM_Sprite_Parent[name] + name;
}

int MGM_LikesItOnTop(std::string name, int default_layer)
{
	if ( MGM_Sprite_OnTop[name] )
	{
		return 5;
	}
	return default_layer;
}

void MGM_Game_Convert(char * file)
{
	printf("Convert %s\n", file);

	printf("looking for %s...", MGM_OldGame_QSS.c_str());
	if (g_file_test(MGM_OldGame_QSS.c_str(), G_FILE_TEST_EXISTS ) )
	{
		printf("	found.\n");
		MGM_Game_NewCreateFolder(file);
		MGM_Game_Path.append("/");
		MGM_Game_Path.append(file);
		MGM_Game_New(MGM_Game_Path.c_str());
		MGM_OldGame_ScanSprites();
		MGM_OldGame_ScanEntities();
		MGM_OldGame_ScanQss();
	}
	else
	{
		printf("	missing.\n");
		quiting();
	}

}

void MGM_OldGame_ScanSprites()
{
	std::string MGM_OldGame_SpriteDir = "sprite sheets\\";
	GDir * current_directory = g_dir_open (MGM_OldGame_SpriteDir.c_str(), 0, &MGM_Game_Error);
	const gchar * current_file = g_dir_read_name(current_directory);

	if (MGM_Game_Error != NULL)
	{
		printf("error message: %s\n", MGM_Game_Error->message);
	}
	g_clear_error(&MGM_Game_Error);

	while (current_file != NULL)
	{
		if (g_str_has_suffix(current_file, ".spt"))
		{
			MGM_OldGame_ConvertSpt(current_file);
		}
		if (g_str_has_suffix(current_file, ".bmp"))
		{
			std::string MGM_OldGame_BMPFile = "sprite sheets\\";
			MGM_OldGame_BMPFile += current_file;
			std::string MGM_NewGame_SpriteFile = MGM_Game_Path + "/sprites/" + current_file;
			MGM_NewGame_SpriteFile = MGM_NewGame_SpriteFile.substr(0,MGM_NewGame_SpriteFile.length()-3) + "png";

			GdkPixbuf * sheet_image = gdk_pixbuf_new_from_file (MGM_OldGame_BMPFile.c_str(), NULL);
			GdkPixbuf * sheet2_image = gdk_pixbuf_scale_simple(sheet_image, gdk_pixbuf_get_width(sheet_image)*2, gdk_pixbuf_get_height(sheet_image)*2, GDK_INTERP_NEAREST );
			gdk_pixbuf_save(sheet2_image, MGM_NewGame_SpriteFile.c_str(), "png", NULL, NULL);
		}
		current_file = g_dir_read_name(current_directory);
	}
}

void MGM_OldGame_ConvertSpt(std::string file)
{
	std::string MGM_OldGame_SpriteFile = "sprite sheets\\" + file;
	std::string MGM_NewGame_SpriteFile = MGM_Game_Path + "/sprites/" + file;
	MGM_NewGame_SpriteFile = MGM_NewGame_SpriteFile.substr(0,MGM_NewGame_SpriteFile.length()-3) + "png.txt";
	std::string parent = file.substr(0,file.length()-3) + "png:";

	std::ifstream sptfile(MGM_OldGame_SpriteFile.c_str());
	std::ofstream outfile(MGM_NewGame_SpriteFile.c_str());

	std::string buffer;
	int fill_line_count = 0;
	float anim_count = 0.0f;
	int anim_frames, anim_frame_count = 0;
	int mode = 0;

	std::string name;
	std::string mask;

	int position[4];
	char flag = 0;
	std::stringstream anim_buffer;
	//Sprite	X	Y	width	height	special

	std::cout << "Converting Spt: " << parent << std::endl;
	do
	{
		std::getline (sptfile, buffer);
		switch(mode)
		{
			case 0:
				if(buffer.length())
					printf(" (%s)\n", buffer.c_str() );
				else
					printf("\n");
				mode = 1;
			break;
			case 1:

				if ( buffer == "[ENTITY]")
				{
					flag += 1;
				}
				else if ( buffer == "[BACKGROUND]")
				{
					//don't care
				}
				else
				{
					if ( !buffer.length() )
					{
						mode = 3;
					}
					else
					{
						std::cout << "Found " << buffer << "\n";
						name = buffer;
						MGM_Sprite_OnTop[name] = false;
						MGM_Sprite_Parent[name] = parent;
						mode = 2;
					}
				}
			break;
			case 2:
				if (buffer == "ANIMATED")
				{
					fill_line_count = 0;
					mode = 4;
					break;
				}

				if ( fill_line_count == 0)
				{
					position[0] = atoi(buffer.c_str()) * 2;
					fill_line_count++;
				}
				else if ( fill_line_count == 1)
				{
					position[1] = atoi(buffer.c_str()) * 2;
					fill_line_count++;
				}
				else if ( fill_line_count == 2)
				{
					position[2] = atoi(buffer.c_str()) * 2;
					fill_line_count++;
				}
				else if ( fill_line_count == 3)
				{
					position[3] = atoi(buffer.c_str()) * 2;
					fill_line_count++;
				}
				else if ( fill_line_count == 4)
				{
					if (buffer == "[FILL]")
					{
						//don't care
					}
					else
					{
						mask = (buffer.length() ? buffer : "0");
						fill_line_count++;
					}
				}
				else if ( fill_line_count == 5)
				{
					//don't care (on top)
					//Should Cache, and use when converting LND
					if (buffer == "TRUE")
					{
						MGM_Sprite_OnTop[name] = true;
					}

					fill_line_count++;
				}
				else if ( fill_line_count == 6)
				{
					//don't care (alpha)
					fill_line_count++;
				}
				else
				{
					if (buffer == "[EE]")
					{

						outfile << name << "\t" << position[0] << "\t" << position[1] << "\t" << position[2] << "\t" << position[3]  << "\t" << (flag + 0) << "\t" << mask << "\n";
						fill_line_count = 0;
						flag = 0;
						mode = 1;
					}
				}
			break;
			case 3:

			break;
			case 4:
				if ( fill_line_count == 0)
				{
					anim_frame_count = 0;
					anim_frames = atoi(buffer.c_str());

					fill_line_count++;
				}
				else if ( fill_line_count == 1)
				{
					anim_count = atof(buffer.c_str());
					fill_line_count++;
				}
				else if ( fill_line_count == 2)
				{
					position[0] = atoi(buffer.c_str()) * 2;
					fill_line_count++;
				}
				else if ( fill_line_count == 3)
				{
					position[1] = atoi(buffer.c_str()) * 2;
					fill_line_count++;
				}
				else if ( fill_line_count == 4)
				{
					position[2] = atoi(buffer.c_str()) * 2;
					fill_line_count++;
				}
				else if ( fill_line_count == 5)
				{
					position[3] = atoi(buffer.c_str()) * 2;
					anim_frame_count++;
					printf("animation frame\n");
					outfile << name << "_f" << anim_frame_count << "\t" << position[0] << "\t" << position[1] << "\t" << position[2] << "\t" << position[3]  << "\t" << (0) << "\t0\n";

					if (anim_frame_count != anim_frames)
					{
						fill_line_count = 2;
					}
					else
					{
						fill_line_count = 6;
					}

				}
				else if ( fill_line_count == 6)
				{
					mask = (buffer.length() ? buffer : "false");
					fill_line_count++;
				}
				else if ( fill_line_count == 7)
				{
					//don't care (on top)
					//Should Cache, and use when converting LND
					fill_line_count++;
				}
				else if ( fill_line_count == 8)
				{
					//don't care (alpha)
					fill_line_count++;
				}
				else  if ( fill_line_count == 9)
				{
					if (buffer == "[EE]")
					{
						outfile << name << "\tANIMATION\t16\t16\t" << anim_count << "\t";
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
						anim_frames = 0;
						fill_line_count = 0;
						flag = 0;
						mode = 1;
					}
				}
				else
				{
					fill_line_count = 0;
				}
			break;
		}
	} while (! sptfile.eof());

}

void MGM_OldGame_ConvertLnd(std::ofstream * file, int x, int y, int offset_x, int offset_y)
{
	std::stringstream name_stream;
	name_stream << "questdata\\" << MGM_OldGame_Name << "\\screens\\" << x << "-" << y << ".lnd";
	std::string file_name = name_stream.str();

	std::string buffer;

	std::ifstream lndfile(file_name.c_str());
	int sprite_count = 0;
	int fill_count = 0;
	int sprite_line_count = 0;
	int fill_line_count = 0;
	int mode = 0;

	std::stringstream sprite_buffer;
	std::stringstream fill_buffer;
	std::string sprite_name;
	int position[4];

	do
	{
		std::getline (lndfile, buffer);
		switch(mode)
		{
			case 0:
				sprite_count = atoi(buffer.c_str());
				printf("\t Sprites: %d\n", sprite_count);
				if (sprite_count)
					mode = 1;
				else
					mode = 2;
			break;
			case 1:
				if ( sprite_line_count == 0)
				{
					position[0] = atoi(buffer.c_str()) + (offset_x*640);
					sprite_line_count++;
				}
				else if ( sprite_line_count == 1)
				{
					position[1] = atoi(buffer.c_str()) + (offset_y*480);
					sprite_line_count++;
				}
				else if ( sprite_line_count == 2)
				{
					sprite_name = buffer;
					sprite_line_count++;
				}
				else
				{
					fill_buffer << "<object value=\"" << MGM_GetFullSpriteName(sprite_name) << "\" type=\"sprite\">" << std::endl;
					fill_buffer << "<position x=\""<< position[0] <<"\" y=\""<< position[1] <<"\" w=\"0\" h=\"0\" z=\"" << MGM_LikesItOnTop(buffer, 1) << "\" /></object>";
					sprite_count--;
					sprite_line_count = 0;
					if (!sprite_count)
					{
						mode = 2;
					}
				}
			break;
			case 2:
				fill_count = atoi(buffer.c_str());
				printf("\t Fills: %d\n", fill_count);
				if (fill_count)
					mode = 3;
				else
					mode = 4;
			break;
			case 3:
				if ( fill_line_count == 0)
				{
					position[0] = atoi(buffer.c_str()) + (offset_x*640);
					fill_line_count++;
				}
				else if ( fill_line_count == 1)
				{
					position[1] = atoi(buffer.c_str()) + (offset_y*480);
					fill_line_count++;
				}
				else if ( fill_line_count == 2)
				{
					position[2] = (atoi(buffer.c_str()) + (offset_x*640)) - position[0];
					fill_line_count++;
				}
				else if ( fill_line_count == 3)
				{
					position[3] = (atoi(buffer.c_str()) + (offset_y*480)) - position[1];
					fill_line_count++;
				}
				else
				{
					fill_buffer << "<object value=\"" << MGM_GetFullSpriteName(buffer) << "\" type=\"sprite\">";
					fill_buffer << "<position x=\""<< position[0] <<"\" y=\""<< position[1] <<"\" w=\""<< position[2] <<"\" h=\"" << position[3] << "\" z=\"" << MGM_LikesItOnTop(buffer,0) << "\" /></object>";
					sprite_count--;
					fill_line_count = 0;
					if (!sprite_count)
					{
						mode = 4;
					}
				}
			break;
			case 4:
				break;
		}
	} while (! lndfile.eof());
	std::string lnd_temp;

	lnd_temp = fill_buffer.str();
	file->write(lnd_temp.c_str(), lnd_temp.length());
	lnd_temp = sprite_buffer.str();
	file->write(lnd_temp.c_str(), lnd_temp.length());

}
void MGM_OldGame_ConvertGroup(std::string name, int group[4], std::string music)
{
	//questdata\TheJourneyBegins\screens\0-0.lnd
	//TheJourneyBegins\maps

	std::string world_filename = MGM_Game_Path + "/world.txt";
	std::ofstream world;
	world.open(world_filename.c_str(), std::ios::app);
	if( world ) {
		printf("Group: %s \n", name.c_str());
		world << name << "\t" << group[0] << "\t" << group[1] << "\t" << group[2] << "\t" << group[3] << "\n";
	}
	world.close();

	std::string map_filename = MGM_Game_Path + "/maps/" + name + ".xml";
	std::ofstream map;
	map.open(map_filename.c_str());
	map << "<map xmlns=\"http://mokoi.sourceforge.net/projects/mokoi\">";
	map << "<settings>";
	map << "<dimensions width=\"" << group[2] << "\" height=\"" << group[2] << "\" />";
	map << "<color red=\"255\" blue=\"255\" green=\"255\" mode=\"0\" />";
	map << "</settings>";
	for( int x = 0; x < group[2]; x++)
	{
		for( int y = 0; y < group[3]; y++)
		{
			printf("\tConverting %d-%d.lnd \n", group[0]+x, group[1]+y);
			MGM_OldGame_ConvertLnd(&map, group[0]+x, group[1]+y, x, y);
		}
	}
	map << "</map>";
	map.close();

	std::string old_script = "questdata\\" + MGM_OldGame_Name + "\\scripts\\group\\" + name + ".zes";
	std::string new_script = MGM_Game_Path + "/maps/scripts/" + name + ".mps";

	printf("\tMoving Script %s to %s (%s)\n", old_script.c_str(), new_script.c_str(), (copy_file(old_script, new_script) == 0 ? "success" : "falid"));
//	printf("\tMoving Script %s to %s (%s)\n", old_script.c_str(), new_script.c_str(), (g_rename(old_script.c_str(), new_script.c_str()) == 0 ? "success" : "falid"));
	//g_rename(old_script.c_str(), new_script.c_str());

}

void MGM_OldGame_ScanQss()
{
	std::string file_name = "save\\" + MGM_OldGame_Name + ".qss";
	std::string buffer;
	std::ifstream file(file_name.c_str());
	int line_count = 0;
	int screen_count = 0;
	int group_count = 0;
	int map_line_count = 0;
	int group_line_count = 0;
	int game_setting_count = 0;
	int mode = 0;

	int group[4];
	std::string current_group;
	int temp_convert;

	std::string config_filename = MGM_Game_Path + "/game.mokoi";
	std::ofstream config;
	config.open(config_filename.c_str());
	config << "[Mokoi]\n";
	do
	{
		std::getline (file, buffer);
		switch(mode)
		{
			case 0:
				mode = 1;
			break;
			case 1:
				screen_count = atoi(buffer.c_str());
				printf("OZ Screen to convert: %d\n", screen_count);
				mode = 2;
			break;
			case 2:
				if ( map_line_count == 0)
				{
					map_line_count++;
				}
				else if ( map_line_count == 1)
				{
					map_line_count++;
				}
				else
				{
					screen_count--;
					map_line_count = 0;
					if (!screen_count)
					{
						mode = 3;
					}
				}
			break;
			case 3:
				group_count = atoi(buffer.c_str());
				printf("OZ Groups to convert: %d\n", group_count);
				mode = 4;
			break;
			case 4:
				if ( group_line_count == 0)
				{
					current_group = buffer;
					group_line_count++;
				}
				else if ( group_line_count == 1)
				{
					group[0] = atoi(buffer.c_str()) / 128;
					group_line_count++;
				}
				else if ( group_line_count == 2)
				{
					group[1] = atoi(buffer.c_str()) / 96;
					group_line_count++;
				}
				else if ( group_line_count == 3)
				{
					group[2] = atoi(buffer.c_str())/128;
					if (group[2] < group[0])
					{
						temp_convert = group[0];
						group[0] = group[2];
						group[2] = temp_convert - group[0];
					}
					else
					{
						group[2] = (atoi(buffer.c_str())/128) - group[0];
					}
					group_line_count++;
				}
				else if ( group_line_count == 4)
				{
					group[3] = atoi(buffer.c_str())/96;
					if (group[3] < group[1])
					{
						temp_convert = group[1];
						group[1] = group[3];
						group[3] = temp_convert - group[1];
					}
					else
					{
						group[3] = (atoi(buffer.c_str())/96) - group[1];
					}
					group_line_count++;
				}
				else if ( group_line_count == 5)
				{
					group_line_count++;
				}
				else
				{
					MGM_OldGame_ConvertGroup(current_group, group, buffer);
					group_count--;
					group_line_count = 0;
					if (!group_count)
					{
						mode = 5;
					}
				}

			break;
			case 5:

				if (game_setting_count == 0)
					config << "world.startx" << "\t=\t" << buffer << "\n";
				else if (game_setting_count == 1)
					config << "world.starty" << "\t=\t" << buffer << "\n";
				else if (game_setting_count == 2)
					config << "project.title" << "\t=\t" << buffer << "\n";
				else if (game_setting_count == 4)
					config << "audio.able" << "\t=\t" << (buffer == "0" ? "true" : "false")  << "\n";

				game_setting_count++;
			break;
		}
	} while (! file.eof());
	config << "player.number" << "\t=\t1\n";
	config << "display.width" << "\t=\t640\n";
	config << "display.height" << "\t=\t480\n";
	config << "map.width" << "\t=\t640\n";
	config << "map.height" << "\t=\t480\n";
	config.close();

	std::string old_script = "questdata\\" + MGM_OldGame_Name + "\\scripts\\main\\main.zes";
	std::string new_script = MGM_Game_Path + "/scripts/main.mps";
	printf("Moving Script %s to %s (%s)\n", old_script.c_str(), new_script.c_str(), (copy_file(old_script, new_script) == 0 ? "success" : "falid"));
	//printf("\tMoving Script %s to %s (%s)\n", old_script.c_str(), new_script.c_str(), (g_rename(old_script.c_str(), new_script.c_str()) == 0 ? "success" : "falid"));

}


bool MGM_Game_Load(const char * file)
{
	MGM_Game_Path = file;
	#ifdef __GNUWIN32__
		MGM_Game_Path += "\\";
	#else
		MGM_Game_Path += "/";
	#endif
	std::string mokoi_file = MGM_Game_Path + "game.mokoi";
	if (g_file_test(mokoi_file.c_str(), G_FILE_TEST_EXISTS ) )
	{
		printf("MGM_Game_Path = %s\n", MGM_Game_Path.c_str());
		return true;
	}
	else
	{
		printf("game.mokoi not found in %s\n", MGM_Game_Path.c_str());
		return false;
	}
}



bool MGM_Game_New(const char * file)
{
	printf("New Game Directory: %s\n", file);
	MGM_Game_Path = file;

	MGM_Game_NewCreateFolder("soundfx");
	MGM_Game_NewCreateFolder("music");
	MGM_Game_NewCreateFolder("dialog");
	MGM_Game_NewCreateFolder("lang");
	MGM_Game_NewCreateFolder("maps");
	MGM_Game_NewCreateFolder("maps/scripts");
	MGM_Game_NewCreateFolder("scripts");
	MGM_Game_NewCreateFolder("scripts/maps");
	MGM_Game_NewCreateFolder("sprites");
	MGM_Game_NewCreateFolder("preload");


	std::string temp_mokoifile;
	temp_mokoifile = MGM_Game_Path + "/dialog/en.txt";
	g_file_set_contents (temp_mokoifile.c_str(), "", -1, &MGM_Game_Error);
	temp_mokoifile = MGM_Game_Path + "/lang/en.txt";
	g_file_set_contents (temp_mokoifile.c_str(), "", -1, &MGM_Game_Error);
	temp_mokoifile = MGM_Game_Path + "/preload/entities.txt";
	g_file_set_contents (temp_mokoifile.c_str(), "main\nplayer", -1, &MGM_Game_Error);
	return true;
}

std::string MGM_Game_GetPath()
{
	return MGM_Game_Path;
}


bool MGM_Game_Compiled(std::string gamefile)
{

	return true;
}

void MGM_Game_CompileDirectory(std::string path )
{
	//Update Progress bar
	std::string fullpathname = MGM_Game_GetPath() + path;
	GDir * current_directory = g_dir_open (fullpathname.c_str(), 0, &MGM_Game_Error);
	const gchar * current_file = g_dir_read_name(current_directory);

	if (MGM_Game_Error != NULL)
	{
		printf("error message: %s\n", MGM_Game_Error->message);
	}
	g_clear_error(&MGM_Game_Error);
	while (current_file != NULL)
	{
		if (g_str_has_suffix(current_file, ".mps"))
		{
			int filenamelen = g_utf8_strlen(current_file, 64);
			//gchar * output = g_utf8_strdown(current_file, (filenamelen - 3));
			std::string string_inputfile = fullpathname + current_file;
			std::string string_outputfile = "-o" + string_inputfile.substr(0,string_inputfile.length()-4) + ".amx";
			std::string string_logfile = "-e" + string_inputfile.substr(0,string_inputfile.length()-4) + ".log";
			int argc = 4;
			char **argv = new char*[4];

			MGM_Misc_SetProgressBar("Compiling Script...", -1);
			g_print("compiling file: %s %s\n", string_inputfile.c_str(), string_outputfile.c_str());

			argv[1] = new char[string_inputfile.length()+1];
			argv[2] = new char[string_outputfile.length()+1];
			argv[3] = new char[string_logfile.length()+1];
			strcpy(argv[1], string_inputfile.c_str());
			strcpy(argv[2], string_outputfile.c_str());
			strcpy(argv[3], string_logfile.c_str());

		}
		current_file = g_dir_read_name(current_directory);
	}
}


int MGM_Misc_GetFileSize(std::string filename)
{
	struct stat file;

	if(!g_stat(filename.c_str(), &file))
	{
		return file.st_size;
	}
	return -1;
}

void MGM_OldGame_MoveFile(std::string old_path, std::string old_file, std::string new_path, bool entity)
{
	std::string old = old_path + old_file;
	std::string newf = new_path + old_file;
	if (entity)
		newf = newf.substr(0,newf.length()-3) + "mps";
	printf("Moving File %s to %s (%s)\n", old.c_str(), newf.c_str(), (copy_file(old, newf) == 0 ? "success" : "falid"));

}

void MGM_OldGame_ScanEntities()
{
	std::string MGM_OldGame_SoundsDir = "entities\\";
	std::string MGM_NewGame_SoundsFile = MGM_Game_Path + "/entities";
	GDir * current_directory = g_dir_open (MGM_OldGame_SoundsDir.c_str(), 0, &MGM_Game_Error);
	const gchar * current_file = g_dir_read_name(current_directory);

	if (MGM_Game_Error != NULL)
	{
		printf("error message: %s\n", MGM_Game_Error->message);
	}
	g_clear_error(&MGM_Game_Error);

	std::fstream file;
	file.open (MGM_NewGame_SoundsFile.c_str(), std::fstream::out);
	file << "main" << "\n";
	while (current_file != NULL)
	{
		if (g_str_has_suffix(current_file, ".zes"))
		{
			file << current_file << "\n";
			MGM_OldGame_MoveFile(MGM_OldGame_SoundsDir, current_file, MGM_Game_Path + "/scripts/", true);
		}
		current_file = g_dir_read_name(current_directory);
	}
	file.close();
}
void MGM_OldGame_ScanAudio()
{
	std::string MGM_OldGame_SoundsDir = "sounds\\";
	std::string MGM_NewGame_SoundsFile = MGM_Game_Path + "/preload/audio.txt";
	GDir * current_directory = g_dir_open (MGM_OldGame_SoundsDir.c_str(), 0, &MGM_Game_Error);
	const gchar * current_file = g_dir_read_name(current_directory);

	if (MGM_Game_Error != NULL)
	{
		printf("error message: %s\n", MGM_Game_Error->message);
	}
	g_clear_error(&MGM_Game_Error);

	std::ofstream audio_file;
	audio_file.open (MGM_NewGame_SoundsFile.c_str(), std::fstream::out);
	while (current_file != NULL)
	{
		if (g_str_has_suffix(current_file, ".wav"))
		{
			//MGM_OldGame_MoveFile(MGM_OldGame_SoundsDir, current_file, MGM_Game_Path + "/soundfx/", false);
			audio_file << "soundfx\t" << current_file << "\n";
		}
		if (g_str_has_suffix(current_file, ".it") || g_str_has_suffix(current_file, ".mid"))
		{
			//MGM_OldGame_MoveFile(MGM_OldGame_SoundsDir, current_file, MGM_Game_Path + "/music/", false);
			audio_file << "music\t" << current_file << "\n";
		}
		current_file = g_dir_read_name(current_directory);
	}
	audio_file.close();
}

void MGM_Misc_CreateProgressWindow()
{

}

void MGM_Misc_SetProgressWindowTitle(std::string title)
{

}

void MGM_Misc_SetProgressBar(std::string title, int progress)
{

}

void MGM_Misc_DeleteProgressWindow()
{

}
*/


