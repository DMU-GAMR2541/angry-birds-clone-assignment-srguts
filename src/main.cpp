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
#include "Slingshot.h" 

int main() {
    // window setup
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    const float SCALE = 30.0f;
    const float PI = 3.1415927f;

    // cursor visuals 
    sf::Sprite sp_cursor;
    sf::Texture sf_cursorTex;
    if (!sf_cursorTex.loadFromFile("../../../../assets/Ang_Birds/cursor.png")) {
        std::cout << "Failed to load texture" << std::endl;
    }
    sp_cursor.setTexture(sf_cursorTex);
    sp_cursor.setOrigin(sf_cursorTex.getSize().x / 2.0f, sf_cursorTex.getSize().y / 2.0f);
    sp_cursor.setScale(0.03f, 0.03f);

    // physics setup
    b2Vec2 b2_gravity(0.0f, 9.8f);
    b2World world(b2_gravity);

    // ground
    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(600.0f / SCALE, 600.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);
    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(600.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    sf::RectangleShape sf_groundVisual(sf::Vector2f(1200.0f, 20.0f));
    sf_groundVisual.setOrigin(600.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34));

    // right-side plank
    b2BodyDef wallDef;
    wallDef.position.Set(1000.0f / SCALE, 500.0f / SCALE); 
    b2Body* wallBody = world.CreateBody(&wallDef);
    b2PolygonShape wallBox;
    wallBox.SetAsBox(10.0f / SCALE, 100.0f / SCALE);
    wallBody->CreateFixture(&wallBox, 0.0f); 

    sf::RectangleShape wallVisual(sf::Vector2f(20.0f, 200.0f));
    wallVisual.setOrigin(10.0f, 100.0f);
    wallVisual.setFillColor(sf::Color::Red);

    // object setup
    Slingshot catapult(sf::Vector2f(200.0f, 500.0f));

    // birds
    std::list<std::shared_ptr<Bird>> birds;
    birds.push_back(std::make_shared<RedBird>(b2Vec2(100.0f / SCALE, 580.0f / SCALE), world));
    birds.push_back(std::make_shared<BlackBird>(b2Vec2(60.0f / SCALE, 580.0f / SCALE), world));
    birds.push_back(std::make_shared<YellowBird>(b2Vec2(20.0f / SCALE, 580.0f / SCALE), world));

    // pigs
    std::list<std::shared_ptr<Pig>> pigs;
    pigs.push_back(std::make_shared<Pig>(b2Vec2(800.0f / SCALE, 550.0f / SCALE), world, 3));
    pigs.push_back(std::make_shared<Pig>(b2Vec2(850.0f / SCALE, 550.0f / SCALE), world, 1));

    // main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!birds.empty()) {
                catapult.update(window, event, birds.front());
            }
        }

        world.Step(1.0f / 60.0f, 8, 3);

        // Update Birds
        for (auto it = birds.begin(); it != birds.end();) {
            (*it)->update();
            b2Vec2 velocity = (*it)->getBody()->GetLinearVelocity();
            bool isFired = (*it)->getBody()->GetPosition().x > 250.0f / SCALE;
            
            if (isFired && velocity.Length() < 1.0f) {
                it = birds.erase(it);
            } else {
                ++it;
            }
        }

        // update pigs
        for (auto it = pigs.begin(); it != pigs.end();) {
            Enemy* enemyPart = dynamic_cast<Enemy*>((*it).get());
            if (enemyPart && enemyPart->getHealth() <= 0) {
                it = pigs.erase(it);
            } else {
                (*it)->update();
                ++it;
            }
        }

        // update visuals
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sp_cursor.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        wallVisual.setPosition(wallBody->GetPosition().x * SCALE, wallBody->GetPosition().y * SCALE);

        // rendering
        window.clear(sf::Color(135, 206, 235));

        // draw static elements
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        window.draw(sf_groundVisual);
        window.draw(wallVisual);
        catapult.draw(window);

        // draw dynamic elements
        for (auto& bird : birds) bird->draw(window);
        for (auto& pig : pigs) pig->draw(window);

        window.draw(sp_cursor);
        window.setMouseCursorVisible(false);
        window.display();
    } 

    return 0; 
}
