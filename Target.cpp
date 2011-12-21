#include <stdlib.h>
#include <stdio.h>
#include "Target.h"


Target::Target(sf::Image* img, sf::Image* img2, int speed, int column, int offset, bool special) : ColumnObject(img, column) {
    this->speed = speed;
    this->special = special;

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
        case 6:
            altSprite.SetColor(sf::Color(255, 255, 255, 255));
            break;
        case 7:
            altSprite.SetColor(sf::Color(128, 0, 128, 255));
            break;
        default:
            break;
    }
    
    if (column < NUM_COLUMNS_LEFT) {
        sprite.SetPosition(vec2((column + 1) * COLUMN_WIDTH, -(size + offset)));
        altSprite.SetPosition(vec2((column + 1) * COLUMN_WIDTH, -(size + offset)));
    } else {
        sprite.SetPosition(vec2(1.f * WINDOW_WIDTH - (NUM_COLUMNS - column) * COLUMN_WIDTH, -(size + offset)));
        altSprite.SetPosition(vec2(1.f * WINDOW_WIDTH - (NUM_COLUMNS - column) * COLUMN_WIDTH, -(size + offset)));
    }
}

void Target::update(float elapsedTime) {
    vec2 pos = getPosition();

    altSprite.SetPosition(pos.x, pos.y + elapsedTime * speed);
    sprite.SetPosition(pos.x, pos.y + elapsedTime * speed);
}

void Target::draw(sf::RenderTarget* target) {

    if (special) {
        target->Draw(altSprite);
    } else {
        target->Draw(sprite);
    }
}

float Target::hitCheck(Goal* goal) {
    float dist = abs(goal->getPosition().y - getPosition().y);
	if(dist <= MAX_HIT_DISTANCE) {
        return 100.0 - dist / MAX_HIT_DISTANCE * 100.0;
	}
	return -1;
}

bool Target::isSpecial() {
    return special;
}