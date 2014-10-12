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
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <json/json.h>

#ifndef ASSETS
	#define ASSETS "./assets"
#endif

#ifdef _WIN32
	const std::string Level::LEVEL_DIR = ((std::string)ASSETS + "\\levels\\");
	const std::string Level::BACKGROUND_PATH =
		((std::string)ASSETS + "\\sprites\\background.png");
#else
	const std::string Level::LEVEL_DIR = ((std::string)ASSETS + "/levels/");
	const std::string Level::BACKGROUND_PATH =
		((std::string)ASSETS + "/sprites/background.png");
#endif

//The background variables:
sf::Image Level::_backgroundImage;
sf::Texture Level::_backgroundTexture;
sf::Sprite Level::_background;

const unsigned int Level::EXITS = 2;

bool Level::init()
{
	if(! _backgroundImage.loadFromFile(BACKGROUND_PATH))
	{
		std::cerr << "Unable to load '" << BACKGROUND_PATH << "'.\n";
		return false;
	}
	_backgroundTexture.loadFromImage(_backgroundImage);
	_backgroundTexture.setRepeated(true);
	_background.setTexture(_backgroundTexture);
	_background.setTextureRect(sf::IntRect(0, 0, 600, 400));
	return true;
}

std::vector <Level*> Level::initLevels()
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

	std::vector <std::string> filepaths;
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
		filepaths.push_back(filepath);

		//Read the next item in the directory:
		dir_contents = readdir(dir);
	}
	//Sort the names of the files so we get them in the right order:
	std::sort(filepaths.begin(), filepaths.end());

	//Make the levels:
	std::vector <Level*> v;
	for(unsigned int i = 0; i < filepaths.size(); i++)
	{
		//Attempt to make a level from this file:
		Level* level = NULL;
		try
		{
			level = new Level(filepaths[i].c_str());
		}
		catch(std::exception& e)
		{
			throw e;
		}

		//If successful, add it to the vector of levels:
		v.push_back(level);
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

		if(type == "start")
		{
			Json::Value position = element.get("pos", Json::Value());
			for(unsigned int i = 0; i < position.size(); i++)
			{
				_start[i] = sf::Vector2f
				(
					position[i].get("x", 0).asFloat(),
					position[i].get("y", 0).asFloat()
				);
			}
		}
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

			//Check if this block has an attached button, and make it if so:
			Json::Value button = element.get("button", Json::Value());
			if(! button.empty())
			{
				Button* btn = new Button
				(
					sf::Vector2f
					(
						button.get("x", 0).asFloat(),
						button.get("y", 0).asFloat()
					),
					sf::Vector2f
					(
						button.get("blockx", 0).asFloat(),
						button.get("blocky", 0).asFloat()
					),
					b
				);
				_buttons.push_back(btn);
			}
		}
		else if(type == "arrow")
		{
			std::string facing = element.get("facing", "none").asString();
			Facing f;
			if(facing == "north")
				f = NORTH;
			else if(facing == "east")
				f = EAST;
			else if(facing == "south")
				f = SOUTH;
			else if(facing == "west")
				f = WEST;
			else
			{
				std::stringstream error;
				error	<< "Failed to parse file '" << file 
						<< "': Arrow with no facing found\n";
				_LevelException ex(error.str().c_str());
				throw ex;
			}
			Arrow* a = new Arrow
			(
				f,
				element.get("x", 0).asFloat(),
				element.get("y", 0).asFloat()
			);
			_arrows.push_back(a);
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
	for(unsigned int i = 0; i < _arrows.size(); i++)
		delete _arrows[i];

	for(unsigned int i = 0; i < _blocks.size(); i++)
		delete _blocks[i];

	for(unsigned int i = 0; i < _buttons.size(); i++)
		delete _buttons[i];

	for(unsigned int i = 0; i < EXITS; i++)
		delete _exits[i];
}

void Level::checkButtons(Player p[])
{
	for(unsigned int i = 0; i < _buttons.size(); i++)
		_buttons[i]->isPressed(p);
}

bool Level::isComplete(Player p[])
{
	return ((_exits[0]->isPlayerInside(p)) & (_exits[1]->isPlayerInside(p)));
}

std::vector <Block*> Level::getBlocks()
{
	return _blocks;
}

std::vector <Button*> Level::getButtons()
{
	return _buttons;
}

sf::Vector2f Level::getStartPosition(int i) const
{
	return _start[i];
}

sf::Sprite& Level::getBackground()
{
	return _background;
}

std::vector <sf::Drawable*> Level::getDrawables()
{
	//Typecast all the drawables up to an sf::Drawable so they can all be
	//processed in one go: 
	std::vector <sf::Drawable*> drawables;
	for(unsigned int i = 0; i < _arrows.size(); i++)
	{
		sf::Drawable* d = dynamic_cast <sf::Drawable*>(_arrows[i]->getSprite());
		drawables.push_back(d);
	}
	for(unsigned int i = 0; i < _blocks.size(); i++)
	{
		sf::Drawable* d = dynamic_cast <sf::Drawable*>(_blocks[i]->getShape());
		drawables.push_back(d);
	}
	for(unsigned int i = 0; i < _buttons.size(); i++)
	{
		sf::Drawable* d = dynamic_cast <sf::Drawable*>(_buttons[i]->getSprite());
		drawables.push_back(d);
	}
	for(unsigned int i = 0; i < EXITS; i++)
	{
		sf::Drawable* d = dynamic_cast <sf::Drawable*>(_exits[i]->getShape());
		drawables.push_back(d);
	}
	return drawables;
}
