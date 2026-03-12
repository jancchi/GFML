#include "GFML/TextArea.hpp"
#include <sstream>

namespace gf {

    static constexpr float PADDING   = 8.0f;
    static constexpr float MIN_HEIGHT = 40.0f;

    TextArea::TextArea(const std::string& label, const sf::Vector2f& size, const sf::Font& font) {
        m_background.setSize(size);
        m_background.setFillColor(sf::Color::White);
        m_background.setOutlineThickness(1);
        m_background.setOutlineColor(sf::Color::Black);

        m_text.setFont(font);
        m_text.setCharacterSize(20);
        m_text.setFillColor(sf::Color::Black);
        // Top-left origin — appropriate for a textarea
        m_text.setOrigin(0.f, 0.f);
        m_text.setPosition(PADDING, PADDING);

        m_content = label;
        refreshText();
    }

    // ── Core helper ──────────────────────────────────────────────────────────
    void TextArea::refreshText() {
        const float maxWidth = m_background.getSize().x - 2.f * PADDING;

        // Build wrapped string character-by-character
        std::string wrapped;
        std::string currentLine;

        for (char c : m_content) {
            if (c == '\n') {                        // explicit newline
                wrapped += currentLine + '\n';
                currentLine.clear();
                continue;
            }

            std::string probe = currentLine + c;
            m_text.setString(probe);

            if (m_text.getLocalBounds().width > maxWidth && !currentLine.empty()) {
                // Word-wrap: try to break at last space
                auto spacePos = currentLine.rfind(' ');
                if (spacePos != std::string::npos) {
                    wrapped += currentLine.substr(0, spacePos + 1) + '\n';
                    currentLine = currentLine.substr(spacePos + 1) + c;
                } else {
                    // No space found — hard break
                    wrapped += currentLine + '\n';
                    currentLine = std::string(1, c);
                }
            } else {
                currentLine = probe;
            }
        }
        wrapped += currentLine;

        m_text.setString(wrapped);

        // Height = text bounds + padding on both sides
        sf::FloatRect b = m_text.getLocalBounds();
        float contentHeight = b.top + b.height;          // top offset matters in SFML!
        float newHeight = std::max(MIN_HEIGHT, contentHeight + 2.f * PADDING);
        m_background.setSize({m_background.getSize().x, newHeight});

        // Keep text pinned to top-left of the box
        m_text.setPosition(PADDING, PADDING);

        refreshGlow();
    }

    static const sf::Color GLOW_COLOR   { 66, 135, 245, 255 };  // blue tint
    static const sf::Color OUTLINE_BLUR { 66, 135, 245, 180 };
    // ── Glow helper ──────────────────────────────────────────────────────────────
    void TextArea::refreshGlow() {
        sf::Vector2f size = m_background.getSize();

        // Each layer expands outward and fades: innermost = most opaque
        static constexpr float   SPREADS[] = { 2.f, 4.f, 7.f };
        static constexpr uint8_t ALPHAS[]  = { 90,  50,  20  };

        for (int i = 0; i < 3; ++i) {
            float exp = SPREADS[i];
            m_glowLayers[i].setSize({ size.x + exp * 2.f, size.y + exp * 2.f });
            m_glowLayers[i].setPosition(-exp, -exp);
            m_glowLayers[i].setFillColor({ GLOW_COLOR.r, GLOW_COLOR.g,
                                           GLOW_COLOR.b, ALPHAS[i] });
        }
    }

    // ── Events ───────────────────────────────────────────────────────────────
    void TextArea::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed
        && event.mouseButton.button == sf::Mouse::Left)
        {
            // Transform mouse pos into local space to support setPosition/setScale
            sf::Vector2f mouse { (float)event.mouseButton.x,
                                 (float)event.mouseButton.y };
            sf::Vector2f local = getInverseTransform().transformPoint(mouse);

            sf::FloatRect bounds { {0.f, 0.f}, m_background.getSize() };
            m_isFocused = bounds.contains(local);

            // Swap outline to show focus state
            m_background.setOutlineColor(m_isFocused
                ? sf::Color(66, 135, 245)   // blue
                : sf::Color::Black);
            m_background.setOutlineThickness(m_isFocused ? 2.f : 1.f);
            return;
        }

        if (!m_isFocused) return;  // ignore keyboard if not focused

        if (event.type != sf::Event::TextEntered) return;

        uint32_t unicode = event.text.unicode;

        if (unicode == 8) {                              // Backspace
            if (!m_content.empty()) m_content.pop_back();
        } else if (unicode == 13) {                      // Enter → real newline
            m_content += '\n';
        } else if (unicode >= 32 && unicode < 128) {     // Printable ASCII
            m_content += static_cast<char>(unicode);
        }

        refreshText();
    }

    // ── Draw ─────────────────────────────────────────────────────────────────
    void TextArea::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(m_background, states);
        target.draw(m_text, states);
    }

    // ── Accessors ─────────────────────────────────────────────────────────────
    std::string TextArea::getText() const { return m_content; }

    void TextArea::appendText(const std::string& str) {
        m_content += str;
        refreshText();
    }

} // namespace gf