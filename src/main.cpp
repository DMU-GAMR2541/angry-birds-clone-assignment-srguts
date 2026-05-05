#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Bird.h"
#include <list>
#include <iostream>
#include <vector>

int main() {
    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Annoyed_Flocks");
    window.setFramerateLimit(90);

    // Setup the SFML variables. 
    sf::Texture sf_tex;          // the texture that will be used to render the dynamic object
    sf::Sprite sp_pig;          // the sprite for the pig
    sf::Sprite sp_cursor;       // the sprite for the cursor 

    // load pig texture
    sf::Texture sf_pigTex;
    if (!sf_pigTex.loadFromFile("H:/Downloads/angry-birds-clone-assignment-srguts/assets/Ang_Birds/PigNoBG.png")) {
        std::cout << "Failed to load texture" << std::endl;
    }

    // load cursor texture
    sf::Texture sf_cursorTex;
    if (!sf_cursorTex.loadFromFile("H:/Downloads/angry-birds-clone-assignment-srguts/assets/Ang_Birds/cursor.png")) {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Assign texture to pig sprite
    sp_pig.setTexture(sf_pigTex);
    sp_pig.setOrigin(sf_pigTex.getSize().x / 2.0f, sf_pigTex.getSize().y / 2.0f);
    sp_pig.setScale(0.031f, 0.031f);

    // Assign texture to cursor sprite
    sp_cursor.setTexture(sf_cursorTex);
    sp_cursor.setOrigin(sf_cursorTex.getSize().x / 2.0f, sf_cursorTex.getSize().y / 2.0f);
    sp_cursor.setScale(0.0199f, 0.0199f);

    // Box2D works in meters. SFML works in pixels.
    const float SCALE = 30.0f;

    // Can set a definition for PI.
    const float PI = 3.1415927;

    // Setup world.
    b2Vec2 b2_gravity(0.0f, 9.8f); // Earth-like gravity
    b2World world(b2_gravity);

    // Setup ground for the circle to move / bounce on.
    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(400.0f / SCALE, 590.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);

    // Define a fixture shape that relates to the collision for the ground.
    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(400.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    // Set up the ground visualisation.
    sf::RectangleShape sf_groundVisual(sf::Vector2f(800.0f, 20.0f));
    sf_groundVisual.setOrigin(400.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34)); // Forest Green

    // Setting up a wall for the ball to hit.
    b2BodyDef b2_wallDef;
    b2_wallDef.position.Set(750.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_wallBody = world.CreateBody(&b2_wallDef);

    // Define a box shape for the wall.
    b2PolygonShape b2_wallBox;
    b2_wallBox.SetAsBox(10.0f / SCALE, 80.0f / SCALE);
    b2_wallBody->CreateFixture(&b2_wallBox, 0.0f);

    // Set up the wall visuals
    sf::RectangleShape sf_wallVisual(sf::Vector2f(20.0f, 160.0f));
    sf_wallVisual.setOrigin(10.0f, 80.0f);
    sf_wallVisual.setFillColor(sf::Color::Red);

    // Define a shape for the cursor similar to the wall, but it will be kinematic and follow the mouse position.
    b2BodyDef b2_cursorDef;
    b2_cursorDef.type = b2_kinematicBody; // Kinematic bodies are not affected by forces but can be moved manually.
    b2_cursorDef.position.Set(0.0f, 0.0f); // Start at the top-left corner.
    b2Body* b2_cursorBody = world.CreateBody(&b2_cursorDef);

    // Dynamic plank body.
    b2BodyDef b2_plankDef;
    b2_plankDef.type = b2_dynamicBody;
    b2_plankDef.position.Set(550.0f / SCALE, 550.0f / SCALE);
    b2Body* b2_plankBody = world.CreateBody(&b2_plankDef);

    // Define a box shape for the plank.
    b2PolygonShape b2_plankBox;
    b2_plankBox.SetAsBox(10.0f / SCALE, 60.0f / SCALE);

    // Define a fixture 
    b2FixtureDef b2_plankFixture;
    b2_plankFixture.shape = &b2_plankBox;
    b2_plankFixture.density = 1.5f;   // Light wood
    b2_plankFixture.friction = 0.3f;
    b2_plankBody->CreateFixture(&b2_plankFixture);

    // Set up the plank visuals
    sf::RectangleShape sf_plankVisual(sf::Vector2f(20.0f, 120.0f));
    sf_plankVisual.setOrigin(10.0f, 60.0f);
    sf_plankVisual.setFillColor(sf::Color(139, 69, 19)); // Brown

    // Create a pig that sits on the plank.
    b2BodyDef b2_pigDef;
    b2_pigDef.type = b2_dynamicBody;
    b2_pigDef.position.Set(595.0f / SCALE, 550.0f / SCALE);
    b2Body* b2_pigBody = world.CreateBody(&b2_pigDef);

    b2CircleShape b2_pigShape;
    b2_pigShape.m_radius = 27.0f / SCALE;

    b2FixtureDef b2_pigFixture;
    b2_pigFixture.shape = &b2_pigShape;
    b2_pigFixture.density = 0.8f;
    b2_pigFixture.restitution = 0.3f;
    b2_pigBody->CreateFixture(&b2_pigFixture);

    std::vector<std::shared_ptr<Bird>> birds;
    for (int i = 0; i < 3; i++) {
        std::shared_ptr<Bird> bird1 = std::make_shared<Bird>("H:/Downloads/angry-birds-clone-assignment-srguts/assets/Ang_Birds/RedBirdNoBG.png", b2Vec2((100.0f + i * 40.0f) / SCALE, 500.0f / SCALE), world);
        birds.push_back(bird1);
    }

    // --- MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // INPUT HANDLING: Press SPACE to launch (Controlled by first bird in list)
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space && !birds.empty()) {
                    b2Body* body = birds[0]->getBody();
                    body->SetTransform(b2Vec2(100.0f / SCALE, 500.0f / SCALE), 0);
                    body->SetLinearVelocity(b2Vec2(0, 0));
                    body->SetAngularVelocity(0);

                    // Apply impulse. Negative Y is UP in Box2D because gravity is positive.
                    body->ApplyLinearImpulse(b2Vec2(12.0f, -12.0f), body->GetWorldCenter(), true);
                    std::cout << "Firing Bird 0!!!!" << std::endl;
                }
            }
        }

        // Update Physics
        world.Step(1.0f / 60.0f, 8, 3);

        for (std::shared_ptr<Bird>& bird : birds) {
            bird->update();
        }

        // The pig follows its physics body
        sp_pig.setPosition(b2_pigBody->GetPosition().x * SCALE, b2_pigBody->GetPosition().y * SCALE);
        sp_pig.setRotation(b2_pigBody->GetAngle() * (180.0f / PI));

        // Static objects usually don't move, but we set position each frame for consistency.
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        sf_wallVisual.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);

        // Dynamic plank.
        sf_plankVisual.setPosition(b2_plankBody->GetPosition().x * SCALE, b2_plankBody->GetPosition().y * SCALE);
        sf_plankVisual.setRotation(b2_plankBody->GetAngle() * (180.0f / PI));

        // Cursor follows the mouse position.
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sp_cursor.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        b2_cursorBody->SetTransform(b2Vec2(mousePos.x / SCALE, mousePos.y / SCALE), 0);

        // Render all content. Clear first 
        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        for (std::shared_ptr<Bird>& bird : birds) {
            bird->draw(window);
        }

        // Draw all the visuals
        window.draw(sf_groundVisual);
        window.draw(sf_wallVisual);
        window.draw(sf_plankVisual);
        window.draw(sp_pig);
        window.draw(sp_cursor);

        window.setMouseCursorVisible(false);             // Hide the default mouse cursor

        window.display();
    }
    return 0;
}