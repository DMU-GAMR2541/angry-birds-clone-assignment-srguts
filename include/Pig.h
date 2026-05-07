#pragma once
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>

class Pig : public DynamicObject {
public:
	Pig(b2Vec2 position, b2World& world);
	void update() override;
	void draw(sf::RenderWindow& window) override;

	// constants 
	const float SCALE = 30.0f;
	const float PI = 3.1415927f;
};
