#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <mutex>                    // added for std::mutex and std::lock_guard                             
#include <future>                   // added for std::async and std::future

// multithreading reference guide
// std::thread        -> spins up an explicit background worker to run a function at the same time as main
// std::async         -> launches a task asynchronously in the background that can return a value when done
// std::future        -> a placeholder object used to wait for and capture the return value from std::async
// std::mutex         -> a mutual exclusion lock that stops multiple threads from touching the same data at once
// std::lock_guard    -> a safe wrapper that locks a mutex instantly and automatically unlocks it when the code block finishes

class UI {
public:
    UI();
    ~UI() = default;

    // update loading progress 
    void renderLoadingScreen(sf::RenderWindow& window);

private:
    // thread syncing and progress tracking
    float m_loadingProgress;
    std::mutex m_progressMutex;

    // background methods to simulate loading different components
    void simulatePhysicsLoading();       // task for std::thread (0% to 50%)
    float simulateAssetLoading();        // task for std::async (50% to 100%) 

    // both threads run at the same time so that the game feels more responsive and the loading screen is more dynamic

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