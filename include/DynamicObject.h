#pragma once 
#include <string>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "GameObject.h"

class DynamicObject : public GameObject {
private:
	std::string name;
	float x, y; // Position of the object
	float velocityX, velocityY; // Velocity of the object

protected:
	sf::Sprite sp_rendered;
	sf::Texture sf_tex;
	b2Body* b2_body;

public:
	virtual ~DynamicObject() = default;
	virtual void update() = 0; // virtual function to update the object's state
	virtual void draw(sf::RenderWindow& window) = 0; // virtual function to draw the object

	b2Body* getBody() const { return b2_body; }

};

