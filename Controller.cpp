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
        
    // Example keyboard input
    if (window->GetInput().IsKeyDown(sf::Key::Left)) {
        goal1->goalHit();
    }
    
    if (window->GetInput().IsKeyDown(sf::Key::Right)) {
        goal2->goalHit();
    }
    
    set1->update(elapsedTime);
}

void Controller::draw() {
    window->Clear();
    
    // Draw stuff here
    set1->draw(window);
    goal1->draw(window);
    goal2->draw(window);
    
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
    set1 = new ActionSet();
    
    set1->targets.push_back(Target(&targetImg, 100, 0, 0));
    set1->targets.push_back(Target(&targetImg, 100, 1, 40));
    
    goal1 = new Goal(&goalImg, 500, 0);
    goal2 = new Goal(&goalImg, 500, 1);
}
