#pragma once

#include <SFML/Graphics.hpp>

#define vec2 sf::Vector2f

class Controller {
private:
    sf::RenderWindow* window;
    
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