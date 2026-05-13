#include "Slingshot.h"
#include <cmath>

Slingshot::Slingshot(sf::Vector2f position)
    : m_anchor(position), m_isDragging(false) {
}

void Slingshot::update(sf::RenderWindow& window, sf::Event& event, std::shared_ptr<Bird> activeBird) {
    if (!activeBird) return;

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel);

    // check for mouse down
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        float dist = std::sqrt(std::pow(mousePos.x - m_anchor.x, 2) + std::pow(mousePos.y - m_anchor.y, 2));
        if (dist < 60.0f) { // radius to grab slingshot
            m_isDragging = true;
        }
    }

    // check for mouse up 
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (m_isDragging) {
            m_isDragging = false;

            sf::Vector2f fireVec = m_anchor - m_dragPos;
            float power = 0.5f; // adjust power to make launch stronger or faster

            // apply the physics impulse
            activeBird->getBody()->ApplyLinearImpulse(
                b2Vec2(fireVec.x * power, fireVec.y * power),
                activeBird->getBody()->GetWorldCenter(),
                true
            );
        }
    }

    // pulling state
    if (m_isDragging) {
        m_dragPos = mousePos;

        // limit for pull distance
        float dist = std::sqrt(std::pow(m_dragPos.x - m_anchor.x, 2) + std::pow(m_dragPos.y - m_anchor.y, 2));
        if (dist > m_maxDrag) {
            float angle = std::atan2(m_dragPos.y - m_anchor.y, m_dragPos.x - m_anchor.x);
            m_dragPos.x = m_anchor.x + m_maxDrag * std::cos(angle);
            m_dragPos.y = m_anchor.y + m_maxDrag * std::sin(angle);
        }

        // teleport bird to the pull position while dragging
        activeBird->getBody()->SetTransform(b2Vec2(m_dragPos.x / SCALE, m_dragPos.y / SCALE), 0);
        activeBird->getBody()->SetLinearVelocity(b2Vec2(0, 0));
        activeBird->getBody()->SetAngularVelocity(0);
    }
}

void Slingshot::draw(sf::RenderWindow& window) {
    
    // draw a base pillar 
    sf::RectangleShape pillar(sf::Vector2f(10, 100));
    pillar.setOrigin(5, 0);
    pillar.setPosition(m_anchor);
    pillar.setFillColor(sf::Color(101, 67, 33)); // dark brown
    window.draw(pillar);

    if (m_isDragging) {
        sf::Vertex line[] = {
            sf::Vertex(m_anchor, sf::Color::Black),
            sf::Vertex(m_dragPos, sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }
}
