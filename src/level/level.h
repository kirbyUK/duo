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
#include "button/button.h"
#include "exit.h"
#include "block/block.h"
#include "block/staticBlock.h"
#include "../interface/arrow.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <exception>
#include <vector>

class Level
{
	private:
		//The directory containing the level files:
		static const std::string LEVEL_DIR;

		//The background image:
		static const std::string BACKGROUND_PATH;
		static sf::Image _backgroundImage;
		static sf::Texture _backgroundTexture;
		static sf::Sprite _background;

		//The start position of the two players:
		sf::Vector2f _start[2];

		//The two exits that must be reached by both characters to progress:
		static const unsigned int EXITS;
		Exit* _exits[2];

		//The arrows (if any) used in the level:
		std::vector <Arrow*> _arrows;

		//The blocks used in the level:
		std::vector <Block*> _blocks;

		//The buttons (if any) used in the level:
		std::vector <Button*> _buttons;


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
		//Loads the background image:
		static bool init();

		//Creates all the levels and returns them as a vector:
		static std::vector <Level*> initLevels();
		static void clean(std::vector <Level*>);

		//Constructor and destructor:
		Level(const char*);
		~Level();

		//Check if any buttons are pressed:
		void checkButtons(Player[]);

		//Check if both players are in seperate exits:
		bool isComplete(Player[]);

		//Return the list of blocks or buttons:
		std::vector <Button*> getButtons();
		std::vector <Block*> getBlocks();

		//Return the start positions:
		sf::Vector2f getStartPosition(int) const;

		//Returns the background sprite:
		static sf::Sprite& getBackground();

		//Return drawables:
		std::vector <sf::Drawable*> getDrawables();
};

#endif
