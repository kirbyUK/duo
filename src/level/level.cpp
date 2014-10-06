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
#include <jsoncpp/json/json.h>

const unsigned int Level::EXITS = 2;

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
}

bool Level::isComplete(Player p[])
{
	return ((_exits[0]->isPlayerInside(p)) && (_exits[1]->isPlayerInside(p)));
}

sf::RectangleShape& Level::getExit(int i) const
{
	return _exits[i]->getShape();
}
