#pragma once 
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>

class GameObject {
protected:
	sf::Sprite sp_rendered;
	sf::Texture sf_tex;

	const float SCALE = 30.0f;
	const float PI = 3.1415927f;

public:
	virtual ~GameObject() = default;

	virtual void draw(sf::RenderWindow& window) {
		window.draw(sp_rendered); // virtual function to draw the object
	}

	virtual void update() = 0; // virtual function to update the object's state
};