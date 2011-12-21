#pragma once

#include <SFML/Graphics.hpp>
#include "Animation.h"

class ComboAction {
private:
    sf::Animation animation;
    float damage;
    bool unlocked;
    
public:
    ComboAction();
    void addAnimation(std::string basePath, int numImgs);
    void draw(sf::RenderTarget* target);
    void update();
    int getNumAnimationFrames();
    bool checkUnlocked();
    void unlock();
};
