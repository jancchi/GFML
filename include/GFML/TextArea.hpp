#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace gf {

	class TextArea : public sf::Drawable, public sf::Transformable {
	public:
		TextArea(const std::string& label, const sf::Vector2f& size, const sf::Font& font);

		void handleEvent(const sf::Event& event);
		void appendText(const std::string& str);
		std::string getText() const;

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::RectangleShape m_background;
		sf::Text m_text;
		std::string m_content;

		bool m_isFocused = false;
	};

} // namespace gf
