#include "Slingshot.h"
#include <cmath>

Slingshot::Slingshot(sf::Vector2f position)
    : m_anchor(position), m_isDragging(false) {
}

void Slingshot::update(sf::RenderWindow& window, sf::Event& event, std::shared_ptr<Bird> activeBird) {
    if (!activeBird) return;

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel);

    // 1. Check for Mouse Down
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        float dist = std::sqrt(std::pow(mousePos.x - m_anchor.x, 2) + std::pow(mousePos.y - m_anchor.y, 2));
        if (dist < 60.0f) { // Radius to "grab" the slingshot
            m_isDragging = true;
        }
    }

    // 2. Check for Mouse Up (Launch!)
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (m_isDragging) {
            m_isDragging = false;

            sf::Vector2f fireVec = m_anchor - m_dragPos;
            float power = 0.5f; // Adjust this to make the launch stronger/weaker

            // Apply the physics impulse
            activeBird->getBody()->ApplyLinearImpulse(
                b2Vec2(fireVec.x * power, fireVec.y * power),
                activeBird->getBody()->GetWorldCenter(),
                true
            );
        }
    }

    // 3. Handle the "Pulling" state
    if (m_isDragging) {
        m_dragPos = mousePos;

        // Limit the pull distance
        float dist = std::sqrt(std::pow(m_dragPos.x - m_anchor.x, 2) + std::pow(m_dragPos.y - m_anchor.y, 2));
        if (dist > m_maxDrag) {
            float angle = std::atan2(m_dragPos.y - m_anchor.y, m_dragPos.x - m_anchor.x);
            m_dragPos.x = m_anchor.x + m_maxDrag * std::cos(angle);
            m_dragPos.y = m_anchor.y + m_maxDrag * std::sin(angle);
        }

        // Teleport bird to the pull position while dragging
        activeBird->getBody()->SetTransform(b2Vec2(m_dragPos.x / SCALE, m_dragPos.y / SCALE), 0);
        activeBird->getBody()->SetLinearVelocity(b2Vec2(0, 0));
        activeBird->getBody()->SetAngularVelocity(0);
    }
}

void Slingshot::draw(sf::RenderWindow& window) {
    // Draw a simple base pillar
    sf::RectangleShape pillar(sf::Vector2f(10, 100));
    pillar.setOrigin(5, 0);
    pillar.setPosition(m_anchor);
    pillar.setFillColor(sf::Color(101, 67, 33)); // Dark Brown
    window.draw(pillar);

    if (m_isDragging) {
        sf::Vertex line[] = {
            sf::Vertex(m_anchor, sf::Color::Black),
            sf::Vertex(m_dragPos, sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }
}
