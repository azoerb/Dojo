#pragma once

#include "ColumnObject.h"
#include "Goal.h"


class Target : public ColumnObject {
private:
    int speed; // Distance in pixels per second
    bool special; // special target (for powerups)
	sf::Sprite altSprite;
    
public:
    Target(sf::Image* img, sf::Image* img2, int speed, int column, int offset, bool special = false);
    
    void update(float elapsedTime);
    
    void draw(sf::RenderTarget* target);

	float hitCheck(Goal* goal);

    bool isSpecial();

};
