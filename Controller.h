#pragma once

#include <SFML/Graphics.hpp>
#include "Animation.h"

#define vec2 sf::Vector2f

class ActionSet;
class Goal;

class Controller {
private:
    sf::RenderWindow* window;
    
    sf::Image targetImg;
    sf::Image goalImg;
    sf::Image backgroundImg;
    
    sf::Sprite background;
    sf::Animation animation;
    
    std::vector<ActionSet> actionSets;
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
    void initializeAnimation(sf::Animation* animation, std::string path, 
                             std::string baseFile, std::string fileType, int numImgs);
	void addRandomSet();
    void randomizeGoals();
};
