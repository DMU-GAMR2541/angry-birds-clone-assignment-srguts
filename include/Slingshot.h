#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Bird.h"
#include <memory>

class Slingshot {
private:
    sf::Vector2f m_anchor;
    bool m_isDragging;
    sf::Vector2f m_dragPos;

    const float m_maxDrag = 100.0f; // limit to drag distance
    const float SCALE = 30.0f;

public:
    Slingshot(sf::Vector2f position);

    void update(sf::RenderWindow& window, sf::Event& event, std::shared_ptr<Bird> activeBird);

    void draw(sf::RenderWindow& window);
};
