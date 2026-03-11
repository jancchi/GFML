//
// Created by admin on 3/10/26.
//

#include <iostream>
#include "GFML/GFML.hpp"

int main() {

	sf::Font font;

	font.loadFromFile("/usr/share/fonts/pixelon.regular.ttf");

	sf::RenderWindow window(sf::VideoMode(800, 600), "GFML Application");

	gf::TextArea userInput("Enter text: ", {20,20}, font);

	userInput.setPosition(400, 400);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

		}

		window.clear(sf::Color::Green);
		window.draw(userInput);
		window.display();
	}



	return 0;
}