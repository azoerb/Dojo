#pragma once

#include "Target.h"

class TargetSet {
private:
    std::vector<Target> targetList;
    float accuracy;
	int numTargets;
    int numMisses;
    
public:
    TargetSet();
    
    std::vector<Target>* getTargets();
    
    void update(float elapsedTime);
    
    void draw(sf::RenderTarget* target);
    
    void addTarget(Target target);
    
    void removeTarget(int index);
    
    float getAccuracy();
    
    void changeAccuracy(float delta);

    void addMiss();

    int getNumMisses();
};
