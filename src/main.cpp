#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include <vector>
#include <list>
#include <memory>

#include "RedBird.h"
#include "BlackBird.h"
#include "YellowBird.h"
#include "Pig.h"
#include "Slingshot.h" // Ensure you have this header!

int main() {
    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Annoyed_Flocks");
    window.setFramerateLimit(60); // Match physics step for smoothness

    const float SCALE = 30.0f;
    const float PI = 3.1415927f;

    // --- 2. PHYSICS SETUP ---
    b2Vec2 b2_gravity(0.0f, 9.8f);
    b2World world(b2_gravity);

    // Ground Setup
    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(600.0f / SCALE, 600.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);
    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(600.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    sf::RectangleShape sf_groundVisual(sf::Vector2f(1200.0f, 20.0f));
    sf_groundVisual.setOrigin(600.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34));

    // --- 3. OBJECT INSTANTIATION ---
    // Create the OO Slingshot at a specific position
    Slingshot catapult(sf::Vector2f(200.0f, 500.0f));

    // Bird List
    std::list<std::shared_ptr<Bird>> birds;
    birds.push_back(std::make_shared<RedBird>(b2Vec2(200.0f / SCALE, 500.0f / SCALE), world));
    birds.push_back(std::make_shared<BlackBird>(b2Vec2(160.0f / SCALE, 500.0f / SCALE), world));
    birds.push_back(std::make_shared<YellowBird>(b2Vec2(120.0f / SCALE, 500.0f / SCALE), world));

    // Pig List
    std::list<std::shared_ptr<Pig>> pigs;
    pigs.push_back(std::make_shared<Pig>(b2Vec2(800.0f / SCALE, 550.0f / SCALE), world, 3));
    pigs.push_back(std::make_shared<Pig>(b2Vec2(850.0f / SCALE, 550.0f / SCALE), world, 1));

    // --- 4. MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // The Slingshot handles its own mouse input logic now
            if (!birds.empty()) {
                catapult.update(window, event, birds.front());
            }
        }

        // Update Physics
        world.Step(1.0f / 60.0f, 8, 3);

        // Update All Birds
        for (auto it = birds.begin(); it != birds.end();) {
            (*it)->update();

            // Logic: If a bird has been fired and is barely moving, remove it to bring the next one up
            b2Vec2 velocity = (*it)->getBody()->GetLinearVelocity();
            bool isFired = (*it)->getBody()->GetPosition().x > 250.0f / SCALE; // Simple check if it left the slingshot

            if (isFired && velocity.Length() < 0.1f) {
                // it = birds.erase(it); // Uncomment this to cycle through your birds!
                ++it;
            }
            else {
                ++it;
            }
        }

        // Update Pigs
        for (auto& pig : pigs) {
            pig->update();
        }

        // --- 5. RENDERING ---
        window.clear(sf::Color(135, 206, 235));

        // Draw Ground
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        window.draw(sf_groundVisual);

        // Draw Catapult rubber bands (under the bird)
        catapult.draw(window);

        // Draw Birds
        for (auto& bird : birds) {
            bird->draw(window);
        }

        // Draw Pigs
        for (auto& pig : pigs) {
            pig->draw(window);
        }

        window.display();
    }

    return 0;
}