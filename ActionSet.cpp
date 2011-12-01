#include "ActionSet.h"

ActionSet::ActionSet() {
	numTargets = 0;
	numTargetsHit = 0;
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
	numTargets++;
}

void ActionSet::removeTarget(int index, bool hit) {
    targets.erase(targets.begin() + index);

	if(hit) {
		numTargetsHit++;
	}
}

float ActionSet::getAccuracy() {
    return accuracy / numTargets;
}

void ActionSet::changeAccuracy(float delta) {
    accuracy += delta;
}
