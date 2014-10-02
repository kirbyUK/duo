/* Copyright (c) Alex Kerr 2014
* This file is part of 'platformer'
*
* 'platformer' is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* 'platformer' is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRENTY; without the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Purpose License
* along with 'platformer'. If not, see <http://www.gnu.org/licenses/>.
*/
#include <SFML/Graphics.hpp>
#include "block/block.h"
#include "block/staticBlock.h"
#include "player/player.h"

int main()
{
	//Attempt to load all nessecary files:
	if(! Player::init())
		return -1;

	//Make the two characters, which are both controlled by the player:
	Player p1(200, 100), p2(400, 200);

	//Make the window:
	sf::RenderWindow window(sf::VideoMode(800, 600), "Reflect",
		sf::Style::Close);
	sf::Event event;

	StaticBlock b(800, 25, 0, 575);

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
		{
			p1.move(LEFT, frameTime);
			p2.move(LEFT, frameTime);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			p1.move(RIGHT, frameTime);
			p2.move(RIGHT, frameTime);
		}
		p1.move(frameTime);
		p2.move(frameTime);
		p1.handleCollision(b.getShape());
		p2.handleCollision(b.getShape());
		p1.handleCollision(&window);
		p2.handleCollision(&window);
		p1.handleMovement();
		p2.handleMovement();

		window.clear(sf::Color(120, 50, 50));
		window.draw(p1.getSprite());
		window.draw(p2.getSprite());
		window.draw(b.getShape());
		window.display();
		frameTime = frameTimer.restart().asSeconds();
	}
	return 0;
}
