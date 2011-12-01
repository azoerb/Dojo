#include "TargetSet.h"

TargetSet::TargetSet() {
	numTargets = 0;
	numTargetsHit = 0;
    accuracy = 0.0;
}

void TargetSet::update(float elapsedTime) {
    for (int i = 0; i < targetList.size(); i++) {
        targetList[i].update(elapsedTime);
    }
}

void TargetSet::draw(sf::RenderTarget* target) {
    for (int i = 0; i < targetList.size(); i++) {
        targetList[i].draw(target);
    }
}

std::vector<Target>* TargetSet::getTargets() {
    return &targetList;
}

void TargetSet::addTarget(Target target) {
    targetList.push_back(target);
	numTargets++;
}

void TargetSet::removeTarget(int index, bool hit) {
    targetList.erase(targetList.begin() + index);

	if(hit) {
		numTargetsHit++;
	}
}

float TargetSet::getAccuracy() {
    return accuracy / numTargets;
}

void TargetSet::changeAccuracy(float delta) {
    accuracy += delta;
}
