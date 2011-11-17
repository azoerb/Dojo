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

	bool leftHit = false, rightHit = false;
        
    if (window->GetInput().IsKeyDown(sf::Key::Left)) {
        goals[0].goalHit();
		leftHit = true;
    }
    
    if (window->GetInput().IsKeyDown(sf::Key::Right)) {
        goals[1].goalHit();
		rightHit = true;
    }
    
    for (int i = 0; i < actionSets.size(); i++) {
        actionSets[i].update(elapsedTime);
        
        std::vector<Target>* targets = actionSets[i].getTargets();
        for (int j = 0; j < targets->size(); j++) {

			// Remove Target if key is hit
			if (leftHit && targets->at(j).getColumn() == 0 && targets->at(j).hit(&goals[0])) {
				actionSets[i].removeTarget(j, true);
			}
			// Remove Target if key is hit
			else if (rightHit && targets->at(j).getColumn() == 1 && targets->at(j).hit(&goals[1])) {
				actionSets[i].removeTarget(j, true);
			}
			// Remove any off-screen Targets
            else if (targets->at(j).getPosition().y > WINDOW_HEIGHT + targets->at(j).getSize()) {
                actionSets[i].removeTarget(j);
            }

			// Once all of the actionSet's targets are gone
            if (targets->size() == 0) {
                // Determine accuracy and play animation
                float accuracy = actionSets[i].getAccuracy();
               
                // TODO: Probably want to have these be dynamicaly
                // based on difficulty level instead of static.
                if (accuracy > 80) {
                    
                } else if (accuracy > 40) {
                    
                } else {
                    
                }
                
                // Delete the actionSet
                actionSets.erase(actionSets.begin() + i);
				
				// add another actionSet
				addRandomSet();
			}
        }
    }
    
}

void Controller::draw() {
    window->Clear();

	for(int i = 0; i < actionSets.size(); i++) {
		actionSets[i].draw(window);
	}

	for(int i = 0; i < goals.size(); i++) {
		goals[i].draw(window);
	}
	
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
	srand ( time(NULL) );

    actionSets.push_back(ActionSet());
    actionSets[actionSets.size() - 1].addTarget(Target(&targetImg, 100, 0, 0));
    actionSets[actionSets.size() - 1].addTarget(Target(&targetImg, 100, 1, 40));

    
    goals.push_back(Goal(&goalImg, 500, 0));
    goals.push_back(Goal(&goalImg, 500, 1));
}

void Controller::addRandomSet() {
	int numTargets = rand() % 10 + 1;

	actionSets.push_back(ActionSet());
	for(int i = 0; i < numTargets; i++) {
		actionSets[actionSets.size() - 1].addTarget(Target(&targetImg, 100, rand()%2, 64*i));
	}
}
