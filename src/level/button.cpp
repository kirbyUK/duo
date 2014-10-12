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
#include "button.h"
#include <iostream>
#include "/home/alex/Code/cpp/sfUtil.h"

#ifndef ASSETS
	#define ASSETS "./assets"
#endif

#ifdef _WIN32
	const std::string Button::BUTTON_PATHS[2] = 
	{
		(((std::string)ASSETS) + ((std::string)"\\sprites\\button.png")),
		(((std::string)ASSETS) + ((std::string)"\\sprites\\button2.png"))
	};
#else
	const std::string Button::BUTTON_PATHS[2] = 
	{
		(((std::string)ASSETS) + ((std::string)"/sprites/button.png")),
		(((std::string)ASSETS) + ((std::string)"/sprites/button2.png"))
	};
#endif

sf::Image Button::_images[2];
//The colour to remove in the images and replace with transparency:
const sf::Color Button::COLOUR_MASK(0, 255, 0);

bool Button::init()
{
	for(unsigned int i = 0; i < 2; i++)
	{
		if(! _images[i].loadFromFile(BUTTON_PATHS[i]))
		{
			std::cerr << "Unable to load '" << BUTTON_PATHS[i] << "'.\n";
			return false;
		}
		else
			//If it loads fine, remove the green background:
			_images[i].createMaskFromColor(COLOUR_MASK);
	}
	return true;
}

Button::Button(sf::Vector2f buttonPos, sf::Vector2f blockPos, Block* block)
{
	_texture.loadFromImage(_images[0]);
	_sprite.setTexture(_texture);
	_sprite.setPosition(buttonPos);
	_block = block;
	_blockPos[0] = _block->_shape.getPosition();
	_blockPos[1] = blockPos;
}

bool Button::isPressed(Player p[])
{
	//Check if either of the players are on top of the button:
	bool isPlayerOnTop = false;
	for(unsigned int i = 0; i < CHARACTERS; i++)
	{
		sf::FloatRect player = p[i].getSprite().getGlobalBounds();
		sf::FloatRect button = _sprite.getGlobalBounds();
		sf::FloatRect detection(
			button.left + (player.width * 0.25),
			button.top - (player.height * 0.15),
			button.width - (player.width * 0.5),
			button.height
		);
		if(player.intersects(detection))
			isPlayerOnTop = true;
	}

	//Move the block and change the sprite if the button is or isn't pressed:
	if(isPlayerOnTop)
	{
		_block->_shape.setPosition(_blockPos[1]);
		_texture.update(_images[1]);
	}
	else
	{
		_block->_shape.setPosition(_blockPos[0]);
		_texture.update(_images[0]);
	}
	return isPlayerOnTop;
}

sf::Sprite* Button::getSprite()
{
	return &_sprite;
}
