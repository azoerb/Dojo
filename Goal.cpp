#include "Goal.h"


Goal::Goal(sf::Image* img1, sf::Image* img2, int goalPosition, int column) : ColumnObject(img1, column) {
    newSize = size;
    hitFrames = 0;
	okFrames = 0;
	rotateFrames = 0;

	altSprite.SetImage(*img2);
    size = img2->GetHeight();
    altSprite.SetCenter(size / 2, size / 2);

    switch (column) {
        case 0:
            altSprite.SetColor(sf::Color(255, 100, 0, 255));
            break;
        case 1:
            altSprite.SetColor(sf::Color(0, 0, 255, 255));
            break;
        case 2:
            altSprite.SetColor(sf::Color(0, 255, 0, 255));
            break;
        case 3:
            altSprite.SetColor(sf::Color(255, 0, 0, 255));
            break;
        case 4:
            altSprite.SetColor(sf::Color(255, 255, 0, 255));
            break;
        case 5:
            altSprite.SetColor(sf::Color(0, 255, 255, 255));
            break;
        default:
            break;
    }

    sprite.SetPosition((column + 1) * COLUMN_WIDTH, goalPosition);
    altSprite.SetPosition((column + 1) * COLUMN_WIDTH, goalPosition);
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

	if (hitFrames > 0) {
		altSprite.Resize(newSize, newSize);
		target->Draw(altSprite);
	} else {
		sprite.Resize(newSize, newSize);
		target->Draw(sprite);
	}
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
