#include <stdlib.h>;
#include <stdio.h>;
#include "Target.h"


Target::Target(sf::Image* img, int speed, int column, int offset) : ColumnObject(img, column) {
    this->speed = speed;
    
    sprite.SetPosition(vec2((column + 1) * COLUMN_WIDTH, -(size + offset)));
}

void Target::update(float elapsedTime) {
    vec2 pos = getPosition();
    sprite.SetPosition(pos.x, pos.y + elapsedTime * speed);
}

void Target::draw(sf::RenderWindow* window) {
    window->Draw(sprite);
}

float Target::hit(Goal* goal) {
    float dist = abs(goal->getPosition().y - this->getPosition().y);
	if(dist <= MAX_HIT_DISTANCE) {
        return 100.0 - dist / MAX_HIT_DISTANCE * 100.0;
	}
	return -1;
}