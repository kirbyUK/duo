/* Copyright (c) 2014, Alex Kerr
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
* SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
* OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
* CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#include "level.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <jsoncpp/json/json.h>

#ifndef ASSETS
	#define ASSETS "./assets"
#endif

#ifdef _WIN32
	std::string Level::LEVEL_DIR = ((std::string)ASSETS + "\\levels\\");
#else
	std::string Level::LEVEL_DIR= ((std::string)ASSETS + "/levels/");
#endif

const unsigned int Level::EXITS = 2;

std::vector <Level*> Level::init()
{
	//Read the levels directory and create a new object for each file:
	DIR* dir = opendir(LEVEL_DIR.c_str());
	if(dir == NULL)
	{
		std::stringstream error;
		error 	<< "Failed to open '" << LEVEL_DIR << "': "
				<< strerror(errno) << std::endl;
		_LevelException ex(error.str().c_str());
		throw ex;
	}

	std::vector <Level*> v;
	dirent* dir_contents = readdir(dir);
	while(dir_contents != NULL)
	{
		//Get the name of the next item:
		std::string name = dir_contents->d_name;

		//Check we are not reading "." or "..":
		if((name == ".") || (name == ".."))
		{
			//Read the next entry:
			dir_contents = readdir(dir);
			continue;
		}

		//Construct the full path of the item:
		std::string filepath = LEVEL_DIR + name;
		
		//Attempt to make a level from this file:
		Level* l = NULL;
		try
		{
			l = new Level(filepath.c_str());
		}
		catch(std::exception& e)
		{
			throw e;
		}

		//If successful, add it to the vector of levels:
		v.push_back(l);

		//Read the next item in the directory:
		dir_contents = readdir(dir);
	}
	return v;
}

void Level::clean(std::vector <Level*> levels)
{
	for(unsigned int i = 0; i < levels.size(); i++)
		delete levels[i];
}

Level::Level(const char* file)
{
	//Attempt to open file, throw exception on error:
	std::ifstream in(file, std::ios::binary);
	if(! in)
	{
		std::stringstream error;
		error 	<< "Failed to open file '" << file << "': "
				<< strerror(errno) << std::endl;
		_LevelException ex(error.str().c_str());
		throw ex;
	}
	//Attempt to parse JSON, throw exception on error:
	Json::Value root;
	Json::Reader reader;
	if(! reader.parse(in, root))
	{
		std::stringstream error;
		error	<< "Failed to parse file '" << file << "': "
				<< reader.getFormattedErrorMessages();
		_LevelException ex(error.str().c_str());
		throw ex;
	}

	//If the parse is successful, go through all the items and create them
	//within the level object:
	short int exits = 0; //The number of exit gates made.
	for(unsigned int i = 0; i < root.size(); i++)
	{
		//Read the type:
		Json::Value element = root[i];
		std::string type = element.get("type", "none").asString();

		//If it is an exit, create a new exit:
		if(type == "exit")
		{
			_exits[exits++] = new Exit
			(
				element.get("width", 0).asFloat(),
				element.get("height", 0).asFloat(),
				element.get("x", 0).asFloat(),
				element.get("y", 0).asFloat()
			);
		}
		else if(type == "static")
		{
			StaticBlock* b = new StaticBlock
			(
				element.get("width", 0).asFloat(),
				element.get("height", 0).asFloat(),
				element.get("x", 0).asFloat(),
				element.get("y", 0).asFloat()
			);
			_blocks.push_back(b);
		}
		//If the element we try to read has no type, throw and error:
		else if(type == "none")
		{
			std::stringstream error;
			error	<< "Failed to parse file '" << file 
					<< "': Element with no type found\n";
			_LevelException ex(error.str().c_str());
			throw ex;
		}
	}
}

Level::~Level()
{
	for(unsigned int i = 0; i < EXITS; i++)
		delete _exits[i];

	for(unsigned int i = 0; i < _blocks.size(); i++)
		delete _blocks[i];
}

bool Level::isComplete(Player p[])
{
	return ((_exits[0]->isPlayerInside(p)) & (_exits[1]->isPlayerInside(p)));
}

std::vector <Block*> Level::getBlocks()
{
	return _blocks;
}

sf::RectangleShape& Level::getExit(int i) const
{
	return _exits[i]->getShape();
}

sf::RectangleShape& Level::getBlockDrawable(int i) const
{
	return _blocks[i]->getShape();
}
