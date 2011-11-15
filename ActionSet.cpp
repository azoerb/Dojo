#include "ActionSet.h"

void ActionSet::update(float elapsedTime) {
    for (int i = 0; i < targets.size(); i++) {
        targets[i].update(elapsedTime);
    }
}

void ActionSet::draw(sf::RenderWindow* window) {
    for (int i = 0; i < targets.size(); i++) {
        targets[i].draw(window);
    }
}