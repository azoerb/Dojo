#pragma once

#include <SFML/Graphics.hpp>
#include "Animation.h"

enum AnimationType {
    ANIMATION_HIT = 0,
    ANIMATION_BLOCK,
    ANIMATION_COUNTER,
};

class Action {
private:
    sf::Animation animations[3];
    
    AnimationType currentAnimation;
    
    float damage;
    
public:
    Action();
    void addAnimation(AnimationType type, std::string basePath, int numImgs);
    void selectAnimation(AnimationType current);
    void draw(sf::RenderTarget* target);
    void update();
};
