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
#ifndef LEVEL_H
#define LEVEL_H
#include "exit.h"
#include "block/block.h"
#include "block/staticBlock.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <exception>
#include <vector>

class Level
{
	private:
		//The directory containing the level files:
		static std::string LEVEL_DIR;

		//The start position of the two players:
		sf::Vector2f _start[2];

		//The two exits that must be reached by both characters to progress:
		static const unsigned int EXITS;
		Exit* _exits[2];

		//The blocks used in the level:
		std::vector <Block*> _blocks;

		//A class to create exceptions if reading in the files fails:
		class _LevelException : public std::exception
		{
			private:
				const char* _errorMessage;

			public:
				_LevelException(const char* errorMessage)
				{
					_errorMessage = errorMessage;
				}

				virtual const char* what() const throw()
				{
					return _errorMessage;
				}
		};

	public:
		//Creates all the levels and returns them as a vector:
		static std::vector <Level*> init();
		static void clean(std::vector <Level*>);

		//Constructor and destructor:
		Level(const char*);
		~Level();

		//Check if both players are in seperate exits:
		bool isComplete(Player[]);

		//Return the list of blocks:
		std::vector <Block*> getBlocks();

		//Return the start positions:
		sf::Vector2f getStartPosition(int) const;

		//Return drawables:
		sf::RectangleShape& getExit(int) const;
		sf::RectangleShape& getBlockDrawable(int) const;
};

#endif
