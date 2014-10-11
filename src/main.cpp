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
#include <SFML/Graphics.hpp>
#include <iostream>
#include <exception>
#include <vector>
#include "interface/arrow.h"
#include "level/level.h"
#include "player/player.h"
#include "sound/music.h"

const int WINDOW_X = 600, WINDOW_Y = 400;

int main()
{
	//Attempt to load all nessecary files:
	if(! Arrow::init())
		return -1;

	if(! Level::init())
		return -1;

	if(! Player::init())
		return -1;

	if(! Music::init())
		return -1;

	std::vector <Level*> levels;
	try
	{
		levels = Level::initLevels();
	}
	catch(std::exception& e)
	{
		std::cerr << e.what();
		return -1;
	}
	int currentLevel = 0;
	Level* level = levels[currentLevel];

	//Make the two characters, which are both controlled by the player:
	Player player[CHARACTERS] =
	{ 
		Player(level->getStartPosition(0)),
		Player(level->getStartPosition(1))
	};

	//Make the window:
	sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Reflect",
		sf::Style::Close);
	sf::Event event;

	Music music;

	sf::Clock jumpTimer;
	bool isSpaceHit = false;
	sf::Clock frameTimer;
	float frameTime = 0.0016;
	while(window.isOpen())
	{
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();

			//If the spacebar is released:
			if(event.type == sf::Event::KeyReleased)
			{
				if(event.key.code == sf::Keyboard::Space)
				{
					for(unsigned int i = 0; i < CHARACTERS; i++)
					{
						player[i].setMaxJumpHeight(jumpTimer.getElapsedTime().asSeconds(),
							frameTime);
					}
					isSpaceHit = false;
				}
			}
		}

		//Handle player movement:
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			for(unsigned int i = 0; i < CHARACTERS; i++)
				player[i].move(LEFT, frameTime);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			for(unsigned int i = 0; i < CHARACTERS; i++)
				player[i].move(RIGHT, frameTime);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if(! isSpaceHit)
			{
				for(unsigned int i = 0; i < CHARACTERS; i++)
				{
					player[i].jump();
					jumpTimer.restart();
				}
				isSpaceHit = true;
			}
		}

		//Handle collisions and movement:
		for(unsigned int i = 0; i < CHARACTERS; i++)
		{
			player[i].move(frameTime);
			for(unsigned int j = 0; j < level->getBlocks().size(); j++)
				player[i].handleCollision(level->getBlocks().at(j)->getShape());
			player[i].handleCollision(&window);
			player[i].handleMovement();
		}

		if(level->isComplete(player))
		{
			level = levels[++currentLevel];
			for(unsigned int i = 0; i < CHARACTERS; i++)
				player[i].reset(level->getStartPosition(i));
		}

		//Draw the frame:
		window.clear();
		window.draw(Level::getBackground());
		for(unsigned int i = 0; i < CHARACTERS; i++)
			window.draw(player[i].getSprite());
		std::vector <sf::Drawable*> drawables = level->getDrawables();
		for(unsigned int i = 0; i < drawables.size(); i++)
			window.draw(*drawables[i]);
		window.display();

		//Get the time of that frame:
		frameTime = frameTimer.restart().asSeconds();
	}
	Level::clean(levels);
	return 0;
}
