#pragma once

#include "ColumnObject.h"


class Goal : public ColumnObject {
private:
    int hitFrames;
	int okFrames;
	int rotateFrames;
    float newSize;
	sf::Sprite altSprite;
    
public:
    Goal(sf::Image* img1, sf::Image* img2, int goalPosition, int column);
    
    void draw(sf::RenderTarget* target);
    
    void goalHit(bool good);

    void goalMiss();
};
