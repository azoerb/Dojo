#include "Action.h"

Action::Action() {
    currentType = ANIMATION_HIT;
}

Action::~Action() {
    printf("~Action()\n");
}

void Action::addAnimation(AnimationType type, std::string basePath, int numImgs) {
    animations[type].init(basePath, numImgs);
}

void Action::selectAnimation(AnimationType current) {
    // Reset the current animation back to it's start
    animations[currentType].reset();
    currentType = current;
}

void Action::selectAnimation(int current) {
    animations[currentType].reset();
    currentType = (AnimationType) current;
}

void Action::draw(sf::RenderTarget* target) {
    animations[currentType].draw(target);
}

void Action::update() {
    animations[currentType].update();
}

int Action::getNumAnimationFrames() {
    return animations[currentType].getNumFrames();
}