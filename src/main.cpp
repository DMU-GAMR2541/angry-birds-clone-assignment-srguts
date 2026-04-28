#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>

int main() {
    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Annoyed_Flocks");
    window.setFramerateLimit(90);

    // Setup the SFML variables. 
    sf::Sprite sp_rendered;  // the sprite to be rendered of the dynamic object
    sf::Texture sf_tex;      // the texture that will be used to render the dynamic object
    
    if (!sf_tex.loadFromFile("H:/Downloads/angry-birds-clone-assignment-srguts/assets/Ang_Birds/RedBirdNoBG.png")) {
        std::cout << "Failed to load texture" << std::endl;
    } 

    // Assign texture to sprite
    sp_rendered.setTexture(sf_tex);

    // Set origin to centre so it rotates correctly around its middle 
    sp_rendered.setOrigin(sf_tex.getSize().x / 2.0f, sf_tex.getSize().y / 2.0f);
	sp_rendered.setScale(0.35f, 0.35f);                                              // Scale down the sprite to fit

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

    b2PolygonShape b2_wallBox;
    b2_wallBox.SetAsBox(10.0f / SCALE, 80.0f / SCALE);
    b2_wallBody->CreateFixture(&b2_wallBox, 0.0f);

    sf::RectangleShape sf_wallVisual(sf::Vector2f(20.0f, 160.0f));
    sf_wallVisual.setOrigin(10.0f, 80.0f);
    sf_wallVisual.setFillColor(sf::Color::Red);

    // Dynamic plank body.
    b2BodyDef b2_plankDef;
    b2_plankDef.type = b2_dynamicBody;
    b2_plankDef.position.Set(550.0f / SCALE, 450.0f / SCALE);
    b2Body* b2_plankBody = world.CreateBody(&b2_plankDef);

    b2PolygonShape b2_plankBox;
    b2_plankBox.SetAsBox(10.0f / SCALE, 60.0f / SCALE);

    b2FixtureDef b2_plankFixture;
    b2_plankFixture.shape = &b2_plankBox;
    b2_plankFixture.density = 1.5f;   // Light wood
    b2_plankFixture.friction = 0.3f;
    b2_plankBody->CreateFixture(&b2_plankFixture);

    sf::RectangleShape sf_plankVisual(sf::Vector2f(20.0f, 120.0f));
    sf_plankVisual.setOrigin(10.0f, 60.0f);
    sf_plankVisual.setFillColor(sf::Color(139, 69, 19)); // Brown

    // Create a ball that is fired when space is pressed.
    b2BodyDef b2_ballDef;
    b2_ballDef.type = b2_dynamicBody;
    b2_ballDef.position.Set(100.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_ballBody = world.CreateBody(&b2_ballDef);

    b2CircleShape b2_circleShape;
    b2_circleShape.m_radius = 15.0f / SCALE;

    b2FixtureDef b2_ballFixture;
    b2_ballFixture.shape = &b2_circleShape;
    b2_ballFixture.density = 1.0f;
    b2_ballFixture.restitution = 0.5f; // Bounciness
    b2_ballBody->CreateFixture(&b2_ballFixture);

    sf::CircleShape sf_ballVisual(15.0f);
    sf_ballVisual.setOrigin(15.0f, 15.0f);
    sf_ballVisual.setFillColor(sf::Color::Yellow);

    // --- MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // INPUT HANDLING: Press SPACE to launch
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    // Reset position of the ball so it can be fired again from its original position.
                    b2_ballBody->SetTransform(b2Vec2(100.0f / SCALE, 500.0f / SCALE), 0);
                    b2_ballBody->SetLinearVelocity(b2Vec2(0, 0));
                    b2_ballBody->SetAngularVelocity(0);

                    // Apply impulse. Negative Y is UP in Box2D because gravity is positive.
                    b2_ballBody->ApplyLinearImpulse(b2Vec2(5.0f, -5.0f), b2_ballBody->GetWorldCenter(), true);

                    std::cout << "Firing!!!!" << std::endl;
                }
            }
        }

        // Update Physics
        world.Step(1.0f / 60.0f, 8, 3);

        // Sync all visuals with physics bodies each frame.
        sf_ballVisual.setPosition(b2_ballBody->GetPosition().x * SCALE, b2_ballBody->GetPosition().y * SCALE);
        sf_ballVisual.setRotation(b2_ballBody->GetAngle() * (180.0f / PI));

        sp_rendered.setPosition(b2_ballBody->GetPosition().x * SCALE, b2_ballBody->GetPosition().y * SCALE);
        sp_rendered.setRotation(b2_ballBody->GetAngle() * (180.0f / PI));

        // Static objects usually don't move, but we set position each frame for consistency.
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        sf_wallVisual.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);

        // Dynamic plank.
        sf_plankVisual.setPosition(b2_plankBody->GetPosition().x * SCALE, b2_plankBody->GetPosition().y * SCALE);
        sf_plankVisual.setRotation(b2_plankBody->GetAngle() * (180.0f / PI));

        // Render all content. Clear first to avoid artefacts.
        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        window.draw(sf_groundVisual);
        window.draw(sf_wallVisual);
        window.draw(sf_plankVisual);
        window.draw(sf_ballVisual);
        window.draw(sp_rendered);

        window.display();
    }

    return 0;
}