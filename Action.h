#pragma once

#include <SFML/Graphics.hpp>
#include "Animation.h"

enum AnimationType {
    ANIMATION_HIT = 0,
    ANIMATION_BLOCK,
    ANIMATION_COUNTER,
    NUM_ANIMATION_TYPES
};

class Action {
private:
    sf::Animation animations[3];
    
    AnimationType currentType;
    
    float damage;
    
public:
    Action();
    virtual ~Action();
    void addAnimation(AnimationType type, std::string basePath, int numImgs);
    void selectAnimation(AnimationType current);
    void selectAnimation(int current);
    void draw(sf::RenderTarget* target);
    void update();
};
