#include "Action.h"

void Action::init(std::string basePath, std::string fileType, int numImgs) {
    animation.init(basePath, fileType, numImgs);
}

void Action::draw(sf::RenderTarget* target) {
    animation.draw(target);
}

void Action::update() {
    animation.update();
}
