#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>

// bird class 
class Bird {                                           
public:
	Bird(const std::string& texturePath, b2Vec2 position, b2World& world);
	void update();
	void draw(sf::RenderWindow& window);

private:
	sf::Sprite sp_rendered;
	sf::Texture sf_tex;
	b2Body* b2_body;
	b2CircleShape b2_circleShape;

	// constants 
	const float SCALE = 30.0f;
	const float PI = 3.1415927f;
};