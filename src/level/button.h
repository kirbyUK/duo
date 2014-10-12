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
#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include <string>
#include "block/block.h"
#include "../player/player.h"

class Block;

class Button
{
	private:
		//Static members relating to the loading of the image files for use
		//as textures for the sprite:
		static const std::string BUTTON_PATHS[2];
		static const sf::Color COLOUR_MASK;
		static sf::Image _images[2];
		sf::Texture _texture;
		sf::Sprite _sprite;

		//The block the button moves:
		Block* _block;

		//The possible positions of the block:
		sf::Vector2f _blockPos[2];

	public:
		//Load the images:
		static bool init();

		//Constructor:
		Button(sf::Vector2f, sf::Vector2f, Block*);

		//Check if the button is pressed:
		bool isPressed(Player[]);

		//Returns the sprite:
		sf::Sprite* getSprite(); 
};

#endif
