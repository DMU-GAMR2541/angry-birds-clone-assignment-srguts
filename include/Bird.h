#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>
#include "DynamicObject.h"

// bird class 
class Bird : public DynamicObject {
public:
	Bird(const std::string& texturePath, b2Vec2 position, b2World& world, float density, float restitution, float scale);
	void update();
	void draw(sf::RenderWindow& window);

private:
	b2CircleShape b2_circleShape;
	
	// constants 
	const float SCALE = 30.0f;
	const float PI = 3.1415927f;
};