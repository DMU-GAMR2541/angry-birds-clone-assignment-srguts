#include "YellowBird.h"

YellowBird::YellowBird(b2Vec2 position, b2World& world)
    : Bird("../../../../assets/Ang_Birds/YellowBird.png", position, world, 0.5f, 0.8f, 0.35f) {
    // sp_rendered.setScale(0.1f, 0.1f);
}

void YellowBird::update() {
    Bird::update();  // call update
}

void YellowBird::draw(sf::RenderWindow& window) {
    Bird::draw(window);  // call draw
}
