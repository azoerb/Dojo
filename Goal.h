#pragma once

#include "ColumnObject.h"

#define vec2 sf::Vector2f


class Goal : public ColumnObject {
private:
    int hitFrames;
    float newSize;
    
public:
    Goal(sf::Image* img, int goalPosition, int column);
    
    void draw(sf::RenderWindow* window);
    
    void goalHit();
    
    vec2 getPosition();
};