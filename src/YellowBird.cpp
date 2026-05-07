#include "YellowBird.h"

YellowBird::YellowBird(b2Vec2 position, b2World& world)
    : Bird("../assets/Ang_Birds/YellowBird.png", position, world, 0.5f, 0.8f) {
    // any YellowBird specific setup here
    sp_rendered.setScale(0.033f, 0.033f);
}

void YellowBird::update() {
    Bird::update();  // call the parent update
}

void YellowBird::draw(sf::RenderWindow& window) {
    Bird::draw(window);  // call the parent draw
}