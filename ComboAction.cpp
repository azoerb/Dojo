#include "ComboAction.h"

ComboAction::ComboAction() {
    unlocked = false;
}

void ComboAction::addAnimation(std::string basePath, int numImgs) {
    animation.init(basePath, numImgs);
}

void ComboAction::draw(sf::RenderTarget* target) {
    animation.draw(target);
}

void ComboAction::update() {
    animation.update();
}

int ComboAction::getNumAnimationFrames() {
    return animation.getNumFrames();
}