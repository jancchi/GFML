#include "GFML/TextArea.hpp"

namespace gf {

	TextArea::TextArea(const std::string& label, const sf::Vector2f& size, const sf::Font& font) {
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

	void TextArea::handleEvent(const sf::Event& event) {
		if (event.type == sf::Event::TextEntered) {

			if (event.text.unicode == 8) {
				if (!m_content.empty()) m_content.pop_back();
			}

			else if (event.text.unicode < 128) {
				m_content += static_cast<char>(event.text.unicode);
			}
			m_text.setString(m_content);
		}
	}

	void TextArea::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(m_background, states);
		target.draw(m_text, states);
	}

	std::string TextArea::getText() const { return m_content; }

} // namespace gf
