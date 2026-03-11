#include "GFML/Button.hpp"

namespace gf {

	Button::Button(const std::string& label, const sf::Vector2f& size, const sf::Font& font) {
		m_background.setSize(size);
		m_background.setFillColor(sf::Color::White);
		m_background.setOutlineThickness(1);
		m_background.setOutlineColor(sf::Color::Black);

		m_text.setFont(font);
		m_text.setString(label);
		m_text.setCharacterSize(20);
		m_text.setFillColor(sf::Color::Black);

		// Center text initially
		sf::FloatRect textBounds = m_text.getLocalBounds();
		m_text.setOrigin(textBounds.left + textBounds.width / 2.0f,
						textBounds.top + textBounds.height / 2.0f);
		m_text.setPosition(size.x / 2.0f, size.y / 2.0f);
	}

	void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) const {
		// Get mouse position relative to the window
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		// Convert to world coordinates (essential if you use sf::View)
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		// Check if mouse is over the button using the combined transform
		bool mouseOver = getTransform().transformRect(m_background.getGlobalBounds()).contains(worldPos);

		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			if (mouseOver && m_callback) {
				m_callback();
			}
		}
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Apply the Button's own transform (position, rotation, scale) to its children
		states.transform *= getTransform();
		target.draw(m_background, states);
		target.draw(m_text, states);
	}


	void Button::setOnClick(const std::function<void()>& callback) { m_callback = callback; }

} // namespace gf
