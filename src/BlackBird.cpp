#include "BlackBird.h"

BlackBird::BlackBird(b2Vec2 position, b2World& world)
    : Bird("../../../../assets/Ang_Birds/BlackBird1.png", position, world, 3.0f, 0.1f, 0.05f) {
    // any BlackBird specific setup here
    // sp_rendered.setScale(0.2f, 0.2f);
}

void BlackBird::update() {
    Bird::update();  // call the parent update
}

void BlackBird::draw(sf::RenderWindow& window) {
    Bird::draw(window);  // call the parent draw
}