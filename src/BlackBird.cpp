#include "BlackBird.h"

BlackBird::BlackBird(b2Vec2 position, b2World& world)
    : Bird("../../../../assets/Ang_Birds/BlackBird.png", position, world, 3.0f, 0.1f) {
    // any BlackBird specific setup here
    sp_rendered.setScale(0.08f, 0.08f);
}

void BlackBird::update() {
    Bird::update();  // call the parent update
}

void BlackBird::draw(sf::RenderWindow& window) {
    Bird::draw(window);  // call the parent draw
}