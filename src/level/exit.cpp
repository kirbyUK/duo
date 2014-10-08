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
#include "exit.h"

const sf::Color Exit::_colours[2] = 
{
	sf::Color(200, 20, 20, 150), //RED
	sf::Color(20, 200, 20, 150)  //GREEN
};

Exit::Exit(float width, float height, float x, float y)
{
	_shape.setSize(sf::Vector2f(width, height));
	_shape.setPosition(x, y);
	_shape.setFillColor(_colours[RED]);
}

bool Exit::isPlayerInside(Player p[])
{
	if((_shape.getGlobalBounds().intersects(p[0].getSprite().getGlobalBounds()))
		|| (_shape.getGlobalBounds().intersects(p[1].getSprite().getGlobalBounds())))
	{
		_shape.setFillColor(_colours[GREEN]);
		return true;
	}
	{
		_shape.setFillColor(_colours[RED]);
		return false;
	}
}

sf::RectangleShape* Exit::getShape()
{
	return &_shape;
}
