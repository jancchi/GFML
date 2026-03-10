# GFML (Good and Fast Multimedia Library) ⚡

SFML is great, but coding a GUI from scratch for the 100th time isn't. **GFML** is a high-level C++ wrapper designed to bridge the gap between a low-level multimedia library and a functional game engine.

## ✨ Features
*   **Ready-to-use UI:** Interactive `Button`, `TextArea`, and `Slider` components.
*   **Smart Layouts:** Easy positioning and alignment that doesn't require a math degree.
*   **Streamlined Events:** Simplified event polling with easy callback attachments.
*   **Boilerplate Killer:** Get a window with functional UI up in minutes, not hours.

## 🚀 Quick Start
```cpp
#include <GFML/GFML.hpp>

int main() {
    gfml::Window window(800, 600, "My GFML App");

    gfml::Button myButton("Click Me", {100, 50});
    myButton.setPosition(350, 275);
    
    myButton.onClick([]() {
        std::cout << "Button clicked! That was fast." << std::endl;
    });

    while (window.isOpen()) {
        window.handleEvents();
        window.clear();
        window.draw(myButton);
        window.display();
    }
    return 0;
}
```
## 🛠 Installation
Ensure you have SFML 2.6+ installed.
Clone this repo: git clone https://github.com
Include the /include folder in your project and link the source.
## 🤝 Contributing
Found a bug or want to add a Checkbox? Pull requests are welcome. Let's keep it Good and Fast.
