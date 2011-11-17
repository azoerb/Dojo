#pragma once

#include "Target.h"

class ActionSet {
private:
    std::vector<Target> targets;
    float accuracy;
	int numTargets;
	int numTargetsHit;
    
public:
    ActionSet();
    
    std::vector<Target>* getTargets();
    
    void update(float elapsedTime);
    
    void draw(sf::RenderWindow* window);
    
    void addTarget(Target target);
    
    void removeTarget(int index, bool hit = false);
    
    float getAccuracy();
};