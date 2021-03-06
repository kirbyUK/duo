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
#include "player.h"
#include <iostream>
#include <fstream>
#include <cmath>

#ifndef ASSETS
	#define ASSETS "assets"
#endif

//The filepaths to the images, with backticks for Windows:
#ifdef _WIN32
	const std::string Player::SPRITE_PATHS[SPRITES] = 
	{
		(((std::string)ASSETS) + ((std::string)"\\sprites\\placeholder.png"))
	};
#else
	const std::string Player::SPRITE_PATHS[SPRITES] = 
	{
		(((std::string)ASSETS) + ((std::string)"/sprites/placeholder.png"))
	};
#endif

//The colour to remove in the images and replace with transparency:
const sf::Color Player::COLOUR_MASK(0, 255, 0);

//The array storing all the images once they've been loaded:
sf::Image Player::_sprites[SPRITES];

//How many pixels the character will move after a second of constant motion in
//that direction:
const float Player::X_VELOCITY = 300.0;
const float Player::Y_VELOCITY = 400.0;

//The maximum and minimum jump height in pixels:
const float Player::MAX_JUMP_HEIGHT = 70.0;
const float Player::MIN_JUMP_HEIGHT = MAX_JUMP_HEIGHT / 2;

//Attempts to load up all the images, must be called before the constructor:
bool Player::init()
{
	for(short int i = 0; i < SPRITES; i++)
	{
		if(! _sprites[i].loadFromFile(SPRITE_PATHS[i]))
		{
			std::cerr << "Unable to load '" << SPRITE_PATHS[i] << "'.\n";
			return false;
		}
		else
			//If it loads fine, remove the green background:
			_sprites[i].createMaskFromColor(COLOUR_MASK);
	}
	return true;
}

Player::Player(sf::Vector2f v)
{	
	//Load the texture from the image:
	_texture.loadFromImage(_sprites[0]);
	_sprite.setTexture(_texture);

	//Set the initial position:
	_sprite.setPosition(v);

	//Initalise everything else:
	_isJumping = false;
	_canJump = true;
	_maxJumpHeight = MAX_JUMP_HEIGHT;
	_jumpDistanceCovered = 0;
	_yVelocity = 0;
	_facing = RIGHT;
	_resetVectors();
}

void Player::reset(sf::Vector2f v)
{
	//Set the initial position:
	_sprite.setPosition(v);

	_isJumping = false;
	_canJump = true;
	_maxJumpHeight = MAX_JUMP_HEIGHT;
	_jumpDistanceCovered = 0;
	if(_facing != RIGHT)
	{
		sf::IntRect rect = _sprite.getTextureRect();
		rect.left += rect.width;
		rect.width = -rect.width;
		_sprite.setTextureRect(rect);
		_facing = RIGHT;
	}
	_resetVectors();
}

void Player::jump()
{
	if(_canJump)
	{
		_isJumping = true;
		_canJump = false;
		_yVelocity = Y_VELOCITY;
//		_sfx.play(JUMP);
	}
}

//Sets the maximum jump height to a fraction of the actual maximum, to allow
//for short hopping:
void Player::setMaxJumpHeight(float seconds, float frameTime)
{
	if(_isJumping)
	{
		seconds *= frameTime;
		seconds *= 420;
		_maxJumpHeight = MAX_JUMP_HEIGHT * seconds;
		if(_maxJumpHeight < MIN_JUMP_HEIGHT)
			_maxJumpHeight = MIN_JUMP_HEIGHT;
		else if(_maxJumpHeight > MAX_JUMP_HEIGHT)
			_maxJumpHeight = MAX_JUMP_HEIGHT;
	}
}

//Handles y-based player movement, as it is not based on the player and must
//therefore be called every frame:
void Player::move(float frameTime)
{
	if(_isJumping)
	{
		_yVelocity -= (frameTime * (Y_VELOCITY * 2.3));
		_distance.player.y = -(_yVelocity * frameTime);
	}
	else
	{
		_yVelocity += (frameTime * (Y_VELOCITY * 2.3));
		if(_yVelocity > Y_VELOCITY)
			_yVelocity = Y_VELOCITY;
		_distance.player.y = (_yVelocity * frameTime);
	}
}

void Player::move(Direction d, float frameTime)
{
	//Adjust the position vector:
	switch(d)
	{
		case LEFT:  _distance.player.x = LEFT *  (X_VELOCITY * frameTime); break;
		case RIGHT: _distance.player.x = RIGHT * (X_VELOCITY * frameTime); break;
	}
}

//Checks if the proposed movement will cause the character to go offscreen:
void Player::handleCollision(sf::RenderWindow* window)
{
	//Create a new Rect representing the player after the proposed movement:
	float x = (
		(_sprite.getGlobalBounds().left) +
		(_distance.player.x) -
		(_getDirection(_distance.player.x) * _distance.offset.x)
	);
	float y = (
		(_sprite.getGlobalBounds().top) +
		(_distance.player.y) -
		(_getDirection(_distance.player.y) * _distance.offset.y)
	);

	sf::FloatRect r(x, y,
		_sprite.getGlobalBounds().width,
		_sprite.getGlobalBounds().height
	);

	//Check if any of the points are outside the screen:
	if(r.left < 0)
		_distance.offset.x += -r.left;
	else if((r.left + r.width) > window->getSize().x)
		_distance.offset.x += ((r.left + r.width) - window->getSize().x);
	else if(r.top < 0)
		_distance.offset.y += -r.top;
}

//Moves the player based on the values in the direction vector. This should be
//called last, after everything involving collisions and all that has been done:
void Player::handleMovement()
{
	_distance.total.x = (
		(_distance.player.x) -
		(_getDirection(_distance.player.x) * _distance.offset.x)
	);
	_distance.total.y = (
		(_distance.player.y) -
		(_getDirection(_distance.player.y) * _distance.offset.y)
	);

	//Move the player:
	_sprite.move(_round(_distance.total.x), _round(_distance.total.y));

	//If the player is jumping, add the distance jumped to the total distance
	//jumped so we know when to end the jump and start the player's descent:
	if(_isJumping)
	{
		_jumpDistanceCovered += -_distance.player.y;
		if((_jumpDistanceCovered >= _maxJumpHeight) ||
			(_round(_distance.offset.y) == _round(_distance.player.y)))
		{
			_isJumping = false;
			_jumpDistanceCovered = 0;
			_yVelocity = 0;
			_maxJumpHeight = MAX_JUMP_HEIGHT;
		}
	}

	//Check if we collided with a platform this frame and allow jumping if so:
	if(((_round(_distance.offset.y) == _round(_distance.player.y))) &&
		(_round(_distance.offset.y > 0)))
	{
		_yVelocity = 0;

		if(! _canJump)
			_canJump = true;
	}

	//Flip the character if required:
	if(_distance.player.x != 0)
	{
		if(_facing != _getDirection(_distance.player.x))
		{
			//Do the flip:
			sf::IntRect rect = _sprite.getTextureRect();
			rect.left += rect.width;
			rect.width = -rect.width;
			_sprite.setTextureRect(rect);

			_facing = _getDirection(_distance.player.x);
		}
	}
	_resetVectors();
}

sf::Sprite& Player::getSprite()
{
	return _sprite;
}

//Simplifies resetting the vectors as it's repeated quite a bit:
void Player::_resetVectors()
{
	//Reset the vectors:
	_distance.player.x = 0;
	_distance.player.y = 0;
	_distance.offset.x = 0;
	_distance.offset.y = 0;
	_distance.total.x = 0;
	_distance.total.y = 0;
}

Direction Player::_getDirection(float f)
{
	if(f > 0) return RIGHT; else return LEFT;
}

float Player::_round(float f)
{
	//<stackoverflow.com/questions/11208971/round-a-float-to-a-given-precision>
	return (float)(std::floor(f * (1.0 / 0.001) + 0.5) / (1.0 / 0.001));
}
