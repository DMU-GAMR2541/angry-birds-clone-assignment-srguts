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

	// physics setup
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

	// Plank Setup
	b2BodyDef b2_plankBodyDef;
	b2_plankBodyDef.type = b2_dynamicBody;
	b2_plankBodyDef.position.Set(700.0f / SCALE, 550.0f / SCALE);
	b2Body* b2_plankBody = world.CreateBody(&b2_plankBodyDef);
	b2PolygonShape b2_plankBox;
	b2_plankBox.SetAsBox(100.0f / SCALE, 10.0f / SCALE);
	b2_groundBody->CreateFixture(&b2_plankBox, 0.0f);

	// Slingshot Setup
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

    // main game loop
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

			// Check if the bird has been fired
            b2Vec2 velocity = (*it)->getBody()->GetLinearVelocity();
            bool isFired = (*it)->getBody()->GetPosition().x > 250.0f / SCALE; // check if bird left the slingshot

			// if a bird has been fired and is almost stationary, remove it from the list
            if (isFired && velocity.Length() < 0.1f) {
                it = birds.erase(it); // remove the bird and get the next iterator
            }
            else {
                ++it; // move to the next bird
			}

            //if (isFired && velocity.Length() < 0.1f) {
            //    // it = birds.erase(it); 
            //    ++it;
            //}
            //else {
            //    ++it;
            //}
        }

		//// check for collisions and apply damage 
  //      for (auto& pig : pigs) {
  //          std::shared_ptr<DynamicObject> obj = pig;
		//	// Check if the pig is colliding with any bird
  //          Enemy* enemyPart = dynamic_cast<Enemy*>(obj.get());
  //          if (enemyPart && pig->getBody()->GetLinearVelocity().Length() > 2.0f) {
  //              enemyPart->takeDamage(1);
  //          }

        // Update Pigs
        for (auto& pig : pigs) {
            pig->update();
        }

        // rendering
        window.clear(sf::Color(135, 206, 235));

        // Draw ground
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        window.draw(sf_groundVisual);
		

		// Draw slingshot 
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