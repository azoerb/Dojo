#pragma once

#include "ColumnObject.h"

#define vec2 sf::Vector2f


class Target : public ColumnObject {
private:
    int speed; // Distance in pixels per second
    
public:
    Target(sf::Image* img, int speed, int column, int offset);
    
    void update(float elapsedTime);
    
    void draw(sf::RenderWindow* window);
    
    vec2 getPosition();
};
