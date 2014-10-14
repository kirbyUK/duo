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
#include "toggleButton.h"
#include <iostream>

#ifndef ASSETS
	#define ASSETS "./assets"
#endif

#ifdef _WIN32
	const std::string ToggleButton::BUTTON_PATHS[2] = 
	{
		(((std::string)ASSETS) + ((std::string)"\\sprites\\t_button.png")),
		(((std::string)ASSETS) + ((std::string)"\\sprites\\t_button2.png"))
	};
#else
	const std::string ToggleButton::BUTTON_PATHS[2] = 
	{
		(((std::string)ASSETS) + ((std::string)"/sprites/t_button.png")),
		(((std::string)ASSETS) + ((std::string)"/sprites/t_button2.png"))
	};
#endif

sf::Image ToggleButton::_images[2];

//The colour to remove in the images and replace with transparency:
const sf::Color ToggleButton::COLOUR_MASK(0, 255, 0);

bool ToggleButton::init()
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

ToggleButton::ToggleButton(sf::Vector2f buttonPos, sf::Vector2f blockPos,
	Block* block)
{
	_texture.loadFromImage(_images[0]);
	_sprite.setTexture(_texture);
	_sprite.setPosition(buttonPos);
	_block = block;
	_blockPos[0] = _block->_shape.getPosition();
	_blockPos[1] = blockPos;
	_isActive = false;
}

void ToggleButton::_handlePressed(bool isPlayerOnTop)
{
	static bool lastTime = false;
	if((isPlayerOnTop) && (isPlayerOnTop != lastTime))
	{
		_isActive = !_isActive;
	}
	lastTime = isPlayerOnTop;

	if(isPlayerOnTop)
		_texture.update(_images[1]);
	else
		_texture.update(_images[0]);

	if(_isActive)
		_block->_shape.setPosition(_blockPos[1]);
	else
		_block->_shape.setPosition(_blockPos[0]);
}
