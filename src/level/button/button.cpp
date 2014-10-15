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
#include "toggleButton.h"
#include "pressureButton.h"

extern sf::Image _images[2];

bool Button::init()
{
	if(! ToggleButton::init())
		return false;

	if(! PressureButton::init())
		return false;

	return true;
}

void Button::isPressed(Player p)
{
	//Check if either of the players are on top of the button:
	sf::FloatRect player = p.getSprite().getGlobalBounds();
	sf::FloatRect button = _sprite.getGlobalBounds();
	sf::FloatRect detection(
		button.left + (player.width * 0.25),
		button.top - (player.height * 0.15),
		button.width - (player.width * 0.5),
		button.height
	);
	_handlePressed(player.intersects(detection));
}

sf::Sprite* Button::getSprite()
{
	return &_sprite;
}
