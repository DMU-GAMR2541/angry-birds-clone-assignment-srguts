#include "Pig.h"
#include <iostream>

Pig::Pig(b2Vec2 position, b2World& world) {
    // load texture
    if (!sf_tex.loadFromFile("../../../../assets/Ang_Birds/Pig.png")) {
        std::cout << "Failed to load texture" << std::endl;
    }
    else {
        std::cout << "Pig texture loaded: "
            << sf_tex.getSize().x
            << " "
            << sf_tex.getSize().y
            << std::endl;
    }

    // setup sprite
    sp_rendered.setTexture(sf_tex);
    sp_rendered.setOrigin(sf_tex.getSize().x / 2.0f, sf_tex.getSize().y / 2.0f);
    sp_rendered.setScale(0.7f, 0.7f);

    // setup physics body
    b2BodyDef b2_bodyDef;
    b2_bodyDef.type = b2_dynamicBody;
    b2_bodyDef.position = position;
    b2_body = world.CreateBody(&b2_bodyDef);

    // setup fixture
    b2CircleShape b2_pigShape;
    b2_pigShape.m_radius = 20.0f / 30.0f;

    b2FixtureDef b2_pigFixture;
    b2_pigFixture.shape = &b2_pigShape;
    b2_pigFixture.density = 2.0f;
    b2_pigFixture.restitution = 0.2f;
    b2_pigFixture.friction = 0.5f;

    b2_body->CreateFixture(&b2_pigFixture);
}

void Pig::update() {
    sp_rendered.setPosition(
        b2_body->GetPosition().x * SCALE,
        b2_body->GetPosition().y * SCALE
    );

    sp_rendered.setRotation(
        b2_body->GetAngle() * (180.0f / PI)
    );
}

void Pig::draw(sf::RenderWindow& window) {
    window.draw(sp_rendered);
}
