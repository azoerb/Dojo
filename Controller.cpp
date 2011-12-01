#include "Controller.h"
#include "ActionSet.h"
#include "Goal.h"
#include <math.h>
#include <sstream>

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
    bool hit;
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
            hit = false;
            
			// Remove Target if key is hit
			for (int k = 0; k < NUM_COLUMNS; k++) {
                if (keyPresses[k] && targets->at(j).getColumn() == k) {
                    float result = targets->at(j).hit(&goals[k]);
                    
                    if (result == -1) {
                        actionSets[i].changeAccuracy(MISS_PENALTY);
                    } else {
                        actionSets[i].changeAccuracy(result);
                        actionSets[i].removeTarget(j, true);
                        hit = true;
                    }
                }
            }
            
			// Remove any off-screen Targets
            if (!hit && targets->at(j).getPosition().y > WINDOW_HEIGHT + targets->at(j).getSize()) {
                actionSets[i].removeTarget(j);
            }

			// Once all of the actionSet's targets are gone
            if (targets->size() == 0) {
                // Determine accuracy and play animation
                float accuracy = actionSets[i].getAccuracy();
               
                // TODO: Probably want to have these be dynamicaly
                // based on difficulty level instead of static.
                if (accuracy > 80) {
                    printf("Good\n");
                } else if (accuracy > 40) {
                    printf("OK\n");
                } else {
                    printf("Bad\n");
                }
                
                // Delete the actionSet
                actionSets.erase(actionSets.begin() + i);
				
                // reset targets, goals
				addRandomSet();
                //randomizeGoals();
			}
        }
    }
    
}

void Controller::draw() {
    window->Clear();
    
    window->Draw(background);
    window->Draw(animation);
    animation.Update();
    
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
        } else if (Event.Type == sf::Event::KeyPressed) {
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
    if (!targetImg.LoadFromFile("target.png") ||
        !goalImg.LoadFromFile("goal.png") ||
        !backgroundImg.LoadFromFile("background.png")) {
        
        // This should quit or throw an exception
    }
}

void Controller::initializeObjects() {
	srand ( time(NULL) );

    background.SetImage(backgroundImg);
    
    initializeAnimation(&animation, "Actions/", "Kick_Hit", ".jpg", 35);
    
    addRandomSet();
    
    goals.push_back(Goal(&goalImg, 500, 0));
    goals.push_back(Goal(&goalImg, 500, 1));
    goals.push_back(Goal(&goalImg, 500, 2));
    goals.push_back(Goal(&goalImg, 500, 3));
}

void Controller::initializeAnimation(sf::Animation* animation, std::string path, 
                                     std::string baseFile, std::string fileType, int numImgs) {
    // So we don't have to hard code in all of the filenames
    for (int i = 1; i < numImgs; i++) {
        std::stringstream ss;
        ss << i;
        
        sf::Image* image = new sf::Image();
        image->LoadFromFile(path + baseFile + ss.str() + fileType);
        animation->AddFrame(image, 1);
    }
    animation->SetBlendMode(sf::Blend::Multiply);
}

void Controller::addRandomSet() {
	int numTargets = MIN_NUM_TARGETS + rand() % (MAX_NUM_TARGETS - MIN_NUM_TARGETS);
    int speed = MIN_TARGET_SPEED + rand() % (MAX_TARGET_SPEED - MIN_TARGET_SPEED);

	actionSets.push_back(ActionSet());
	for(int i = 0; i < numTargets; i++) {
		actionSets[actionSets.size() - 1].addTarget(Target(&targetImg, speed, rand() % NUM_COLUMNS, COLUMN_WIDTH * i));
	}
}

void Controller::randomizeGoals() {
    goals.clear();

    int pos = 250 + rand() % 250;

    for(int i = 0; i < NUM_COLUMNS; i++) {
        goals.push_back(Goal(&goalImg, pos, i));
    }
}
