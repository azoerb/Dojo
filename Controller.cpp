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
    // reset all key presses
    for(int i = 0; i < NUM_COLUMNS; i++) {
        keyPresses[i] = false;
    }

    processEvents();
    
    elapsedTime = window->GetFrameTime();

	for (int i = 0; i < actionSets.size(); i++) {
        actionSets[i].update(elapsedTime);
        
        std::vector<Target>* targets = actionSets[i].getTargets();
        for (int j = 0; j < targets->size(); j++) {

			// Remove Target if key is hit
			if (keyPresses[0] && targets->at(j).getColumn() == 0 && targets->at(j).hit(&goals[0])) {
				actionSets[i].removeTarget(j, true);
			}
			else if (keyPresses[1] && targets->at(j).getColumn() == 1 && targets->at(j).hit(&goals[1])) {
				actionSets[i].removeTarget(j, true);
			}
            else if (keyPresses[2] && targets->at(j).getColumn() == 2 && targets->at(j).hit(&goals[2])) {
				actionSets[i].removeTarget(j, true);
			}
            else if (keyPresses[3] && targets->at(j).getColumn() == 3 && targets->at(j).hit(&goals[3])) {
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
				
                // reset targets, goals
				addRandomSet();
                randomizeGoals();
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
        if (Event.Type == sf::Event::Closed) {
            window->Close();
        } else if (Event.Type == sf::Event::KeyReleased) {
            switch(Event.Key.Code) {
            case sf::Key::Q:
                goals[0].goalHit();
                keyPresses[0] = true;
                break;
            case sf::Key::W:
                goals[1].goalHit();
                keyPresses[1] = true;
                break;
            case sf::Key::E:
                goals[2].goalHit();
                keyPresses[2] = true;
                break;
            case sf::Key::R:
                goals[3].goalHit();
                keyPresses[3] = true;
                break;
            }
        }
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
    goals.push_back(Goal(&goalImg, 500, 2));
    goals.push_back(Goal(&goalImg, 500, 3));
}

void Controller::addRandomSet() {
	int numTargets = 2 + rand() % 14;
    int speed = 100 + rand() % 100;

	actionSets.push_back(ActionSet());
	for(int i = 0; i < numTargets; i++) {
		actionSets[actionSets.size() - 1].addTarget(Target(&targetImg, speed, rand() % NUM_COLUMNS, 64*i));
	}
}

void Controller::randomizeGoals() {
    goals.clear();

    int pos = 250 + rand() % 250;

    for(int i = 0; i < NUM_COLUMNS; i++) {
        goals.push_back(Goal(&goalImg, pos, i));
    }
}
