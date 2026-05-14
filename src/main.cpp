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
#include "StaticObject.h" 

int main() {
    // window setup
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "annoyed_flocks");
    window.setFramerateLimit(60);

    const float SCALE = 30.0f;

    // cursor visuals 
    sf::Sprite sp_cursor;
    sf::Texture sf_cursorTex;
    if (!sf_cursorTex.loadFromFile("assets/Ang_Birds/cursor.png")) {
        std::cout << "failed to load texture" << std::endl;
    }
    sp_cursor.setTexture(sf_cursorTex);
    sp_cursor.setOrigin(sf_cursorTex.getSize().x / 2.0f, sf_cursorTex.getSize().y / 2.0f);
    sp_cursor.setScale(0.03f, 0.03f);

    // physics setup
    b2Vec2 b2_gravity(0.0f, 9.8f);
    b2World world(b2_gravity);

    // solid green ground extending to screen bottom
    sf::RectangleShape sf_groundVisual(sf::Vector2f(1920.0f, 480.0f)); 
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34)); // forest green
    sf_groundVisual.setOrigin(960.0f, 0.0f); // origin at top center of rectangle

    // b2d definitions
    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(960.0f / SCALE, 600.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);
    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(960.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    // bird platform in the sky
    auto birdPlatform = std::make_shared<StaticObject>("assets/Ang_Birds/wall.png",
        b2Vec2(200.0f / SCALE, 200.0f / SCALE), world, sf::Vector2f(600.0f, 20.0f));

    // right-side wall
    auto rightWall = std::make_shared<StaticObject>("assets/Ang_Birds/wall.png",
        b2Vec2(1000.0f / SCALE, 500.0f / SCALE), world, sf::Vector2f(20.0f, 200.0f));

    // slingshot setup
    Slingshot catapult(sf::Vector2f(200.0f, 500.0f));

    // birds spawned on the high platform and out the way
    std::list<std::shared_ptr<Bird>> birds;
    birds.push_back(std::make_shared<RedBird>(b2Vec2(150.0f / SCALE, 170.0f / SCALE), world));
    birds.push_back(std::make_shared<BlackBird>(b2Vec2(100.0f / SCALE, 170.0f / SCALE), world));
    birds.push_back(std::make_shared<YellowBird>(b2Vec2(50.0f / SCALE, 170.0f / SCALE), world));

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

            // hotkey to swap birds (TAB)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                if (birds.size() > 1) {
                    auto current = birds.front();
                    birds.pop_front();
                    birds.push_back(current);
                }
            }

            if (!birds.empty()) {
                catapult.update(window, event, birds.front());
            }
        }

        world.Step(1.0f / 60.0f, 8, 3);

        // update birds and freeze the ones not being used
        int birdIndex = 0;
        for (auto it = birds.begin(); it != birds.end();) {
            (*it)->update();
            b2Body* body = (*it)->getBody();
            b2Vec2 velocity = body->GetLinearVelocity();
            bool isFired = body->GetPosition().x > 250.0f / SCALE;

            // keep waiting birds frozen in the sky so they do not roll around
            if (!isFired && birdIndex > 0) {
                body->SetLinearVelocity(b2Vec2(0, 0));
                body->SetAngularVelocity(0);
            }

            if (isFired && velocity.Length() < 1.0f) {
                it = birds.erase(it);
            }
            else {
                ++it;
                birdIndex++;
            }
        }

        // update pigs and death logic 
        for (auto it = pigs.begin(); it != pigs.end();) {
            Enemy* enemyPart = dynamic_cast<Enemy*>((*it).get());
            if (enemyPart) {
                float speed = (*it)->getBody()->GetLinearVelocity().Length();
                if (speed > 5.0f) {
                    enemyPart->takeDamage(1);
                    std::cout << "pig hit! hp: " << enemyPart->getHealth() << std::endl;
                }

                if (enemyPart->getHealth() <= 0) {
                    it = pigs.erase(it);
                    continue;
                }
            }
            (*it)->update();
            ++it;
        }

        // update visuals
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sp_cursor.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        rightWall->update();
        birdPlatform->update();

        // rendering
        window.clear(sf::Color(135, 206, 235));

        // draw level
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        window.draw(sf_groundVisual);
        rightWall->draw(window);
        birdPlatform->draw(window);
        catapult.draw(window);

        // draw dynamic objects
        for (auto& bird : birds) bird->draw(window);
        for (auto& pig : pigs) pig->draw(window);

        // draw cursor
        window.draw(sp_cursor);
        window.setMouseCursorVisible(false);
        window.display();
    }

    return 0;
}
