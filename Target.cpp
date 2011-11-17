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

bool Target::hit(Goal* goal) {
	if(abs(goal->getPosition().y - this->getPosition().y) < 25) {
		return true;
	}
	return false;
}