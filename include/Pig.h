#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>
#include "DynamicObject.h"
#include "Enemy.h"

class Pig : public DynamicObject, public Enemy {
public:
    Pig(b2Vec2 position, b2World& world, int health);

    void update() override;
    void draw(sf::RenderWindow& window) override;

    // constants 
    const float SCALE = 30.0f;
    const float PI = 3.1415927f;
};