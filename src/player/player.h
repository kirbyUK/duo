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
#ifndef PLAYER_H
#define PLAYER_H
#include <type_traits>
#include <SFML/Graphics.hpp>
#include <string>
//#include "../sound/sfx.h"

enum Direction { LEFT = -1, RIGHT = 1 };
const unsigned int CHARACTERS = 2;

class Player
{
	private:
		//Static members relating to the loading of the image files for use
		//as textures for the sprite:
		static const short int SPRITES = 1;
		static const std::string SPRITE_PATHS[SPRITES];
		static const sf::Color COLOUR_MASK;
		static sf::Image _sprites[SPRITES];

		//The texture and the sprite:
		sf::Texture _texture;
		sf::Sprite _sprite;

		//The maximum and minium height in pixels the character can go up 
		//before they start to fall back to the ground:
		static const float MAX_JUMP_HEIGHT, MIN_JUMP_HEIGHT;
		float _maxJumpHeight;
		float _jumpDistanceCovered;
		float _yVelocity;
		bool _isJumping;
		bool _canJump;

		Direction _facing;

		//These vectors contain the total distance moved by the player, split
		//across the input from the player, input from a DynamicBlock, and an
		//offset that is subtracted if there is a collision:
		static const float X_VELOCITY, Y_VELOCITY;
		struct Distance { sf::Vector2f player, block, offset, total; } _distance;
		void _resetVectors();

/*		//Sound effects:
		SoundEffect _sfx;*/

		//A function for converting a floating point number to a Direction:
		static Direction _getDirection(float);

		//A function to round decimal numbers to a different precision:
		static float _round(float);

	public:
		//Load the images:
		static bool init();

		//Constructor:
		Player(sf::Vector2f);

		//Resets the player back to how it was if it had just been made:
		void reset(sf::Vector2f);

		//Signal the player to jump:
		void jump();

		//Set the maximum jump height to a fraction of the actual maximum:
		void setMaxJumpHeight(float, float);

		//Signal the player to move in the given direction:
		void move(float); 			//Handles y-direction movement.
		void move(Direction, float);//Handles x-direction movement.

		//Process events at the end of the frame:
		void handleCollision(sf::RenderWindow*);
		template<typename T>
		void handleCollision(T*);
		void handleMovement();

		//Getters:
		sf::Sprite& getSprite();
};

template <typename T>
void Player::handleCollision(T* s)
{
	static_assert(std::is_base_of<sf::Sprite, T>::value ||
				  std::is_base_of<sf::RectangleShape, T>::value,
				  "Error: handleCollisions must be called"
				  "       with Sprite or RectangleShape");
	//Create a new Rect representing the player after the proposed movement:
	float x = (
		(_sprite.getGlobalBounds().left) + 
		(_distance.player.x + _distance.block.x)
	);
	float y = (
		(_sprite.getGlobalBounds().top) + 
		(_distance.player.y + _distance.block.y)
	);

	sf::FloatRect r(x, y,
		_sprite.getGlobalBounds().width,
		_sprite.getGlobalBounds().height
	);

	//Check if this new Rect collides with the given sprite:
	sf::FloatRect intersection;
	if(r.intersects(s->getGlobalBounds(), intersection))
	{
		//If there is a collision, we need to create another rect
		//representing the player after movement with one of the directions
		//cancelled out, as in many cases this will sort out the other
		//direction on it's own. Which direction to handle first is important,
		//and this is determined by taking the intersection rect. If it's
		//taller than it is wide, we do the x direction first, and vice-versa.
		if(intersection.height > intersection.width)
			_distance.offset.x += intersection.width;
		else
			_distance.offset.y += intersection.height;

		//Now we do the same again:
		x = (
			(_sprite.getGlobalBounds().left) +
			(_distance.player.x + _distance.block.x) -
			(_getDirection(_distance.player.x + _distance.block.x) * 
			_distance.offset.x)
		);
		y = (
			(_sprite.getGlobalBounds().top) +
			(_distance.player.y + _distance.block.y) -
			(_getDirection(_distance.player.y + _distance.block.y) * 
			_distance.offset.y)
		);

		r.left = x;
		r.top =  y;

		if(r.intersects(s->getGlobalBounds(), intersection))
		{
			if((_round(intersection.height) != 0) && (_round(intersection.width) != 0))
			{
				if(_distance.offset.x != 0)
					_distance.offset.y += intersection.height;
				else
					_distance.offset.x += intersection.width;
			}
		}
	}
}

#endif
