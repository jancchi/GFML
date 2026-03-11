#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

namespace gf {
	class Button : public sf::Drawable, public sf::Transformable {

		sf::RectangleShape m_background;
		sf::Text m_text;

		std::function<void()> m_callback;

		bool m_isHovered = false;

	public:
		Button(const std::string& label, const sf::Vector2f& size, const sf::Font& font);

		void handleEvent(const sf::Event& event, const sf::RenderWindow& window) const;
		void setOnClick(const std::function<void()>& callback);

		void setBackgroundColor(const sf::Color color){m_background.setFillColor(color);}
		void setTextColor(const sf::Color color){m_text.setFillColor(color);}
	protected:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}