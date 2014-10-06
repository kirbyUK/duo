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
#include "block/block.h"
#include "block/staticBlock.h"
#include "level/level.h"
#include "player/player.h"
#include "sound/music.h"

const int WINDOW_X = 600, WINDOW_Y = 400;

int main()
{
	//Attempt to load all nessecary files:
	if(! Player::init())
		return -1;

	if(! Music::init())
		return -1;

	//Make the two characters, which are both controlled by the player:
	Player player[CHARACTERS] = { Player(200, 100), Player(400, 200) };

	//Make the window:
	sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Reflect",
		sf::Style::Close);
	sf::Event event;

	StaticBlock b(WINDOW_X, 25, 0, WINDOW_Y - 25);
	Music music;

	std::vector <Level*> levels;
	try
	{
		Level* level = new Level("assets/levels/level1.json");
		levels.push_back(level);
	}
	catch(std::exception& e)
	{
		std::cerr << e.what();
	}
	Level* level = levels[0];

	sf::Clock frameTimer;
	float frameTime = 0.0016;
	while(window.isOpen())
	{
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}

		//Handle player movement:
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			for(unsigned int i = 0; i < CHARACTERS; i++)
				player[i].move(LEFT, frameTime);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			for(unsigned int i = 0; i < CHARACTERS; i++)
				player[i].move(RIGHT, frameTime);

		for(unsigned int i = 0; i < CHARACTERS; i++)
		{
			player[i].move(frameTime);
			player[i].handleCollision(b.getShape());
			player[i].handleCollision(&window);
			player[i].handleMovement();
		}

		level->isComplete(player);

		window.clear(sf::Color(120, 50, 50));
		for(unsigned int i = 0; i < CHARACTERS; i++)
			window.draw(player[i].getSprite());
		window.draw(b.getShape());
		window.draw(level->getExit(0));
		window.draw(level->getExit(1));
		window.display();
		frameTime = frameTimer.restart().asSeconds();
	}
	delete level;
	return 0;
}
