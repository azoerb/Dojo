#pragma once

#include "Animation.h"
#include "Action.h"
#include "TargetSet.h"
#include "Goal.h"
#include <SFML/Graphics.hpp>

class Controller {
private:
    sf::RenderWindow* window;
    
    sf::Image targetImg;
    sf::Image goalImg;
    sf::Image backgroundImg;
    
    sf::Sprite background;
    Action action;
    
    std::vector<Action> basicActions;
    std::vector<Action> comboActions;
    std::vector<TargetSet> targetSets;
    std::vector<Goal> goals;

    bool keyPresses[4];
    
    float elapsedTime;
    
public:
    Controller();
    ~Controller();
    
    void mainLoop();
    void update();
    void draw();
    void processEvents();
    void loadResources();
    void initializeObjects();
	void addRandomSet();
    void randomizeGoals();
};
