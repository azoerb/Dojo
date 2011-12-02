#include "Action.h"

Action::Action() {
    currentAnimation = ANIMATION_HIT;
}

void Action::addAnimation(AnimationType type, std::string basePath, int numImgs) {
    animations[type].init(basePath, numImgs);
}

void Action::selectAnimation(AnimationType current) {
    // Reset the current animation back to it's start
    animations[currentAnimation].reset();
    currentAnimation = current;
}

void Action::draw(sf::RenderTarget* target) {
    animations[currentAnimation].draw(target);
}

void Action::update() {
    animations[currentAnimation].update();
}
