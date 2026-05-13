#pragma once
#include "GameObject.h"

class StaticObject : public GameObject {
public:
    // Simple constructor: needs a texture, position, the physics world, and its size
    StaticObject(const std::string& texturePath, b2Vec2 position, b2World& world, sf::Vector2f size) {
        if (sf_tex.loadFromFile(texturePath)) {
            sp_rendered.setTexture(sf_tex);
            sp_rendered.setOrigin(sf_tex.getSize().x / 2.0f, sf_tex.getSize().y / 2.0f);

            // Match the sprite scale to the requested size
            sp_rendered.setScale(size.x / sf_tex.getSize().x, size.y / sf_tex.getSize().y);
        }

        // Physics: Static bodies don't move (default type is b2_staticBody)
        b2BodyDef bodyDef;
        bodyDef.position = position;
        b2_body = world.CreateBody(&bodyDef);

        b2PolygonShape box;
        box.SetAsBox((size.x / 2.0f) / SCALE, (size.y / 2.0f) / SCALE);
        b2_body->CreateFixture(&box, 0.0f); // 0.0 density = static
    }

    // Static objects don't move, so update just ensures the sprite is in place
    void update() override {
        sp_rendered.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
        sp_rendered.setRotation(b2_body->GetAngle() * (180.0f / 3.14159f));
    }

protected:
    b2Body* b2_body;
};