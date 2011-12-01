#pragma once

#include <SFML/Graphics.hpp>
#include "Animation.h"


class Action {
private:
    sf::Animation animation;
    float damage;
    float duration;
    
public:
    void init(std::string basePath, std::string fileType, int numImgs);
    void draw(sf::RenderTarget* target);
    void update();
};
