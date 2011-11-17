#pragma once

#include "ColumnObject.h"
#include "Goal.h"


class Target : public ColumnObject {
private:
    int speed; // Distance in pixels per second
    
public:
    Target(sf::Image* img, int speed, int column, int offset);
    
    void update(float elapsedTime);
    
    void draw(sf::RenderWindow* window);

	bool hit(Goal* goal);
};
