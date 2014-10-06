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
#include "../block/block.h"
#include "../block/staticBlock.h"
#include <SFML/Graphics.hpp>
#include <exception>
#include <vector>

class Level
{
	private:
		//The two exits that must be reached by both characters to progress:
		static const unsigned int EXITS;
		Exit* _exits[2];

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
		Level(const char*);
		~Level();

		//Check if both players are in seperate exits:
		bool isComplete(Player[]);

		//Return drawables:
		sf::RectangleShape& getExit(int) const;
};

#endif
