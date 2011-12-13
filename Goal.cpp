#include "Goal.h"


Goal::Goal(sf::Image* img, int goalPosition, int column) : ColumnObject(img, column) {
    newSize = size;
    hitFrames = 0;
	okFrames = 0;
	rotateFrames = 0;

    sprite.SetPosition((column + 1) * COLUMN_WIDTH, goalPosition);
}

void Goal::draw(sf::RenderTarget* target) {
    // Resize the Goal depending on which frame of the animation it's in.
    if (hitFrames > HIT_FRAMES / 2) {
        newSize += HIT_RATIO / HIT_FRAMES * size;
        hitFrames--;
    } else if (hitFrames > 0) {
        newSize -= HIT_RATIO / HIT_FRAMES * size;
        hitFrames--;
    } else if (okFrames > OK_FRAMES/ 2) {
		newSize -= OK_RATIO / OK_FRAMES * size;
		okFrames--;
	} else if (okFrames > 0) {
		newSize += OK_RATIO / OK_FRAMES * size;
		okFrames--;
	}

	int rotate = 0;
	if (rotateFrames > 0) {
		rotate = 90 / ROTATE_FRAMES;
		rotateFrames --;
	}

	if (rotate != 0) {
		sprite.Rotate(rotate);
	}

    sprite.Resize(newSize, newSize);
    target->Draw(sprite);
}

void Goal::goalHit(bool good) {
    // This sets the frame limit for the Goal bounce animation.
    if (hitFrames == 0 && okFrames == 0) {
		if (good)
			hitFrames = HIT_FRAMES;
		else
			okFrames = OK_FRAMES;
    }
}

void Goal::goalMiss() {
	if (rotateFrames == 0) {
		rotateFrames = ROTATE_FRAMES;
	}
}
