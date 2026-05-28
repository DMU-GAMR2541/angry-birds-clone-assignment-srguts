#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>

class UI {
public:
    UI();
    ~UI() = default;

    // update loading progress 
    void renderLoadingScreen(sf::RenderWindow& window);

private:
	// tracking for progress bar
    float m_loadingProgress;

	// UI elements
    sf::Font m_font;
    sf::Text m_titleText;
    sf::Text m_progressText;
    sf::RectangleShape m_progressBar;
    sf::RectangleShape m_progressBarBackground;

    // asset sprites and textures
    sf::Texture m_bgTexture1;
    sf::Texture m_bgTexture2;
    sf::Sprite m_bgSprite1;
    sf::Sprite m_bgSprite2;
};

#endif // UI_H