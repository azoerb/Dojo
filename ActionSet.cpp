#include "ActionSet.h"

ActionSet::ActionSet() {
    accuracy = 0.0;
}

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

std::vector<Target>* ActionSet::getTargets() {
    return &targets;
}

void ActionSet::addTarget(Target target) {
    targets.push_back(target);
}

void ActionSet::removeTarget(int index) {
    targets.erase(targets.begin() + index);
}

float ActionSet::getAccuracy() {
    return accuracy;
}
