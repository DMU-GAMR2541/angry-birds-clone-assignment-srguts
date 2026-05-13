#include "Bird.h"
#include <iostream>
#include <algorithm>

// Added scale to constructor
Bird::Bird(const std::string& texturePath, b2Vec2 position, b2World& world, float density, float restitution, float scale) {

	if (!sf_tex.loadFromFile(texturePath)) {
		std::cout << "Failed to load texture" << std::endl;
	}

	// sprite setup
	sp_rendered.setTexture(sf_tex);

	// scale setup
	sp_rendered.setScale(scale, scale);
	sp_rendered.setOrigin(sf_tex.getSize().x / 2.0f, sf_tex.getSize().y / 2.0f);

	// physics body setup
	b2BodyDef b2_bodyDef;
	b2_bodyDef.type = b2_dynamicBody;
	b2_bodyDef.position = position;
	b2_body = world.CreateBody(&b2_bodyDef);

	// radius adapts to size of sprite
	float visualWidth = sf_tex.getSize().x * scale;
	b2_circleShape.m_radius = (visualWidth / 2.0f) / SCALE;

	// bird fixture setup
	b2FixtureDef b2_ballFixture;
	b2_ballFixture.shape = &b2_circleShape;
	b2_ballFixture.density = density;
	b2_ballFixture.restitution = restitution;
	b2_ballFixture.friction = 0.3f;
	b2_body->CreateFixture(&b2_ballFixture);
}

void Bird::update() {
	// Match SFML sprite to Box2D body
	sp_rendered.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
	sp_rendered.setRotation(b2_body->GetAngle() * (180.0f / PI));
}

void Bird::draw(sf::RenderWindow& window) {
	window.draw(sp_rendered);
}
