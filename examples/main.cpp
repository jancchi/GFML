//
// Created by admin on 3/10/26.
//

#include <iostream>
#include "GFML/GFML.hpp"

int main() {

	sf::Font font;

	font.loadFromFile("/usr/share/fonts/pixelon.regular.ttf");

	sf::RenderWindow window(sf::VideoMode(800, 600), "GFML Application");

	gf::Button button("Click me!", {4,4}, font);

	gf::TextArea userInput("Enter text: ", {80,20}, font);

	userInput.setPosition(400, 400);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			userInput.handleEvent(event);
			if (event.type == sf::Event::Closed) {
				window.close();
			}

		}

		window.clear(sf::Color::Blue);
		window.draw(userInput);
		window.draw(button);
		window.display();
	}



	return 0;
}