#pragma once
#include "Bird.h"
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>

class YellowBird : public Bird {
public:
	YellowBird(b2Vec2 position, b2World& world);
	void update() override;
	void draw(sf::RenderWindow& window) override;
};