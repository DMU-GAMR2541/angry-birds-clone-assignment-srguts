#include "Bird.h"
#include <iostream>
#include <algorithm>

Bird::Bird(const std::string& texturePath, b2Vec2 position, b2World& world, float density, float restitution) {
	// setup texture sprite and body here
	if (!sf_tex.loadFromFile(texturePath)) {
		std::cout << "Failed to load texture" << std::endl;
	}

	// --- SETUP SPRITE ---
	sp_rendered.setTexture(sf_tex);
	sp_rendered.setOrigin(sf_tex.getSize().x / 2.0f, sf_tex.getSize().y / 2.0f);
	// Scale is set by subclass constructors
	std::cout << sf_tex.getSize().x << " " << sf_tex.getSize().y << std::endl;
	std::cout << sp_rendered.getLocalBounds().getSize().x << " " << sp_rendered.getLocalBounds().getSize().y << std::endl;

	// setup physics body
	b2BodyDef b2_bodyDef;
	b2_bodyDef.type = b2_dynamicBody;
	b2_bodyDef.position = position;
	b2_body = world.CreateBody(&b2_bodyDef);

	// Define a circle shape for the bird (FIXED: radius must be set)
	b2_circleShape.m_radius = 15.0f / SCALE;

	// Define a fixture for the bird 
	b2FixtureDef b2_ballFixture;
	b2_ballFixture.shape = &b2_circleShape;
	b2_ballFixture.density = density;
	b2_ballFixture.restitution = restitution; // Bounciness
	b2_ballFixture.friction = 0.3f; // Added friction for better behaviour
	b2_body->CreateFixture(&b2_ballFixture);
}

void Bird::update() {
	// update the sprite's position and rotation to match the physics body
	sp_rendered.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
	sp_rendered.setRotation(b2_body->GetAngle() * (180.0f / PI));
}

void Bird::draw(sf::RenderWindow& window) {
	// draw the sprite to the window
	window.draw(sp_rendered);
}