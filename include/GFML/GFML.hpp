#pragma once

// Core Library Components
#include "Button.hpp"
#include "TextArea.hpp"

// Version Info
#define GFML_VERSION_MAJOR 1
#define GFML_VERSION_MINOR 0

namespace gf {
	template <typename T>
	bool isClicked(const T& object, const sf::RenderWindow& window, const sf::Event& event) {
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {

			sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

			sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

			return object.getGlobalBounds().contains(worldPos);
		}
		return false;
	}

	template <typename T>
bool isHovered(const T& object, const sf::RenderWindow& window) {
		// Get mouse position and convert to world coordinates
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		// Check if world position is inside the object's bounds
		return object.getGlobalBounds().contains(worldPos);
	}

}