#include "UI.h"
#include <iostream>

UI::UI() : m_loadingProgress(0.0f) {

	// load font for title and progress text
    if (!m_font.loadFromFile("../../../../assets/fonts/angry-birds.ttf")) {
        std::cerr << "CRITICAL ERROR: Failed to load font assets/Ang_Birds/arial.ttf!" << std::endl;
    }

	// title text setup
    m_titleText.setFont(m_font);
    m_titleText.setString("ANNOYED FLOCKS");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setStyle(sf::Text::Bold);
    m_titleText.setPosition(500.0f, 100.0f); 

	// progress text that updates with the bar
    m_progressText.setFont(m_font);
    m_progressText.setCharacterSize(24);
    m_progressText.setFillColor(sf::Color::Cyan);
    m_progressText.setPosition(600.0f, 420.0f);

    // load images for background
    if (!m_bgTexture1.loadFromFile("../../../../assets/Ang_Birds/RedBirdNoBG.png") ||
        !m_bgTexture2.loadFromFile("../../../../assets/Ang_Birds/Pig.png")) {
        std::cerr << "textures have not loaded correctly" << std::endl;
    }
    else {
        m_bgSprite1.setTexture(m_bgTexture1);
        m_bgSprite1.setPosition(500.0f, 270.0f);
        m_bgSprite1.setScale(0.5f, 0.5f);

        m_bgSprite2.setTexture(m_bgTexture2);
        m_bgSprite2.setPosition(700.0f, 270.0f);
        m_bgSprite2.setScale(0.5f, 0.5f);
    }

	// static background bar for the progress bar
    m_progressBarBackground.setSize(sf::Vector2f(400.0f, 30.0f));
    m_progressBarBackground.setFillColor(sf::Color(50, 50, 50));
    m_progressBarBackground.setPosition(450.0f, 460.0f);

    // progress bar filling up
    m_progressBar.setSize(sf::Vector2f(0.0f, 30.0f));
    m_progressBar.setFillColor(sf::Color::Green);
    m_progressBar.setPosition(450.0f, 460.0f);
}

void UI::renderLoadingScreen(sf::RenderWindow& window) {
    
    // clock to control the progress bar's update, simulates loading time 
    sf::Clock processClock;

    // main loop for the loading screen
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }

        // simulates loading progress 
        if (m_loadingProgress < 100.0f) {
			if (processClock.getElapsedTime().asSeconds() > 0.03) {     // every 30ms update progress bar by 1%
                m_loadingProgress += 1.0f;
                processClock.restart(); // reset frame delay counter
            }
        }

		// text and bar updates based on the current loading progress
        m_progressBar.setSize(sf::Vector2f(m_loadingProgress * 4.0f, 30.0f)); 
        m_progressText.setString(std::to_string(static_cast<int>(m_loadingProgress)) + "%");

        // at 100% break loop and enter game
        if (m_loadingProgress >= 100.0f) {
            break;
        }

        // Draw everything to the screen buffer
        window.clear(sf::Color(30, 30, 30)); // Dark grey background canvas
        window.draw(m_titleText);               // title text using font
        window.draw(m_bgSprite1);               // static image 1
        window.draw(m_bgSprite2);               // static image 2
		window.draw(m_progressBarBackground);   // static background bar for the progress bar
        window.draw(m_progressBar);             // loading progress bar view
		window.draw(m_progressText);            // progress percentage text

        window.display();
    }
}