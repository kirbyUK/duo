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
#include "leftRight.h"

LeftRight::LeftRight(float velocity, float boundary1, float boundary2)
{
	_distanceMoved.x = 0;
	_distanceMoved.y = 0;

	_velocity = velocity;
	if(boundary1 > boundary2)
	{
		_boundaries[0] = boundary1;
		_boundaries[1] = boundary2;
	}
	else
	{
		_boundaries[0] = boundary2;
		_boundaries[1] = boundary1;
	}
	_direction = 1;
}

void LeftRight::handleEvents(sf::RectangleShape& shape, float frameTime)
{
	//Check if we need to change direction:
	if(shape.getPosition().x >= _boundaries[0])
		_direction = -1;
	else if(shape.getPosition().x <= _boundaries[1])
		_direction = 1;

	//Move the block:
	shape.move((_direction * (_velocity * frameTime)), 0);

	//Save the distance moved:
	_distanceMoved.x = (_direction * (_velocity * frameTime));
}

