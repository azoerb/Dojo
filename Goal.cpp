#include "Goal.h"


Goal::Goal(sf::Image* img, int goalPosition, int column) : ColumnObject(img, column) {
    newSize = size;
    hitFrames = 0;

    sprite.SetPosition((column + 1) * COLUMN_WIDTH, goalPosition);
}

void Goal::draw(sf::RenderWindow* window) {
    // Resize the Goal depending on which frame of the animation it's in.
    if (hitFrames > HIT_FRAMES / 2) {
        newSize += HIT_RATIO / HIT_FRAMES * size;
        hitFrames--;
    } else if (hitFrames > 0) {
        newSize -= HIT_RATIO / HIT_FRAMES * size;
        hitFrames--;
    }
    sprite.Resize(newSize, newSize);
    window->Draw(sprite);
}

void Goal::goalHit() {
    // This sets the frame limit for the Goal bounce animation.
    if (hitFrames == 0) {
        hitFrames = HIT_FRAMES;
    }
}
