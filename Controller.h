#pragma once

#include <SFML/Graphics.hpp>

#define vec2 sf::Vector2f

class ActionSet;
class Goal;

class Controller {
private:
    sf::RenderWindow* window;
    
    sf::Image targetImg;
    sf::Image goalImg;
    
    ActionSet* set1;
    Goal* goal1;
    Goal* goal2;
    
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
};