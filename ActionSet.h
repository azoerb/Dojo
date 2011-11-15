#pragma once

#include "Target.h"

class ActionSet {
private:
    std::vector<Target> targets;
    
public:    
    std::vector<Target>* getTargets();
    
    void update(float elapsedTime);
    
    void draw(sf::RenderWindow* window);
    
    void addTarget(Target target);
};