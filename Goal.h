#pragma once

#include "ColumnObject.h"


class Goal : public ColumnObject {
private:
    int hitFrames;
	int okFrames;
	int rotateFrames;
    float newSize;
    
public:
    Goal(sf::Image* img, int goalPosition, int column);
    
    void draw(sf::RenderTarget* target);
    
    void goalHit(bool good);

    void goalMiss();
};
