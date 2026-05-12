#include "RedBird.h"

RedBird::RedBird(b2Vec2 position, b2World& world)
    : Bird("../../../../assets/Ang_Birds/RedBirdNoBG.png", position, world, 1.0f, 0.5f, 0.35f) {
    // any RedBird setup here
    // sp_rendered.setScale(0.35f, 0.35f);
}

void RedBird::update() {
    Bird::update();  // call the parent update
}

void RedBird::draw(sf::RenderWindow& window) {
    Bird::draw(window);  // call the parent draw
}