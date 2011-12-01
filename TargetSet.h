#pragma once

#include "Target.h"

class TargetSet {
private:
    std::vector<Target> targetList;
    float accuracy;
	int numTargets;
	int numTargetsHit;
    
public:
    TargetSet();
    
    std::vector<Target>* getTargets();
    
    void update(float elapsedTime);
    
    void draw(sf::RenderTarget* target);
    
    void addTarget(Target target);
    
    void removeTarget(int index, bool hit = false);
    
    float getAccuracy();
    
    void changeAccuracy(float delta);
};
