#include "Controller.h"
#include "ActionSet.h"
#include "Goal.h"
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

Controller::Controller() {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dojo Hero");
    window->UseVerticalSync(true);
    
    loadResources();
    initializeObjects();
}

Controller::~Controller() {
    if (window) { delete window; }
}

void Controller::mainLoop() {
    while (window->IsOpened()) {
        update();
        draw();
    }
}

void Controller::update() {
    processEvents();
    
    elapsedTime = window->GetFrameTime();
        
    if (window->GetInput().IsKeyDown(sf::Key::Left)) {
        goals[0].goalHit();
    }
    
    if (window->GetInput().IsKeyDown(sf::Key::Right)) {
        goals[1].goalHit();
    }
    
    for (int i = 0; i < actionSets.size(); i++) {
        actionSets[i].update(elapsedTime);
    }
}

void Controller::draw() {
    window->Clear();
    
    // Draw stuff here
    actionSets[0].draw(window);
    goals[0].draw(window);
    goals[1].draw(window);
    
    window->Display();
}

void Controller::processEvents() {
    // Process events
    sf::Event Event;
    while (window->GetEvent(Event)) {
        // Close window : exit
        if (Event.Type == sf::Event::Closed)
            window->Close();
    }
}

void Controller::loadResources() {
    if (!targetImg.LoadFromFile("target.png")) {
        printf("Error loading images");
    }
    if (!goalImg.LoadFromFile("goal.png")) {
        printf("Error loading images");
    }
}

void Controller::initializeObjects() {
    actionSets.push_back(ActionSet());
    actionSets[actionSets.size() - 1].addTarget(Target(&targetImg, 100, 0, 0));
    actionSets[actionSets.size() - 1].addTarget(Target(&targetImg, 100, 1, 40));

    
    goals.push_back(Goal(&goalImg, 500, 0));
    goals.push_back(Goal(&goalImg, 500, 1));
}
