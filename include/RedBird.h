#pragma once
#include "Bird.h"
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>

class RedBird : public Bird {
public:
	RedBird(b2Vec2 position, b2World& world);
	void update() override;
	void draw(sf::RenderWindow& window) override;
};