#include "BlackBird.h"

BlackBird::BlackBird(b2Vec2 position, b2World& world)
	: Bird("../../../../assets/Ang_Birds/BlackBird.png", position, world, 0.5f, 0.5f, 0.5f) {   // higher density, lower restitution, bigger scale
    // sp_rendered.setScale(0.2f, 0.2f);
}

void BlackBird::update() {
    Bird::update();  // call update
}

void BlackBird::draw(sf::RenderWindow& window) {
    Bird::draw(window);  // call draw
}
