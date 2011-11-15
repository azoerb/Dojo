#pragma once

#include "Target.h"

class ActionSet {
private:
    
public:
    std::vector<Target> targets;
    void update(float elapsedTime);
    void draw(sf::RenderWindow* window);
};