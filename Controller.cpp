#include "Controller.h"
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

Controller::Controller() {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dojo Hero");
    window->UseVerticalSync(true);
    
    player = new Player(PLAYER_HEALTH);
    enemy = new Enemy(BASIC_ENEMY_HEALTH);

    resetState(1);

    // display loading image
    loadingImg.LoadFromFile("loading.png");
    loading.SetImage(loadingImg);
    window->Clear();
    window->Draw(loading);
    window->Display();


    loadResources();
    initializeObjects();

    window->Clear(sf::Color(255,255,255));
    displayMessage("Press Any Key to Start.", 150, 200, window);
    window->Display();
}

Controller::~Controller() {
    if (window) { delete window; }
	if (player) { delete player; }
	if (enemy) { delete enemy; }
}

void Controller::mainLoop() {
    while (window->IsOpened()) {

        // reset all key presses
        for(int i = 0; i < NUM_COLUMNS; i++) {
            keyPresses[i] = false;
        }

        processEvents();


        if(gameStatus == GAME_PLAY) {
            if(update()) {
                draw();
            }
        } else {
            Sleep(10);
        }
    }
}

bool Controller::update() {

	// use idle animation after action animation is complete
	if(numActionFrames > 0) {
		numActionFrames--;

        // update health
        if(numActionFrames == 0) {
            switch(currentAnimationType) {
            case ANIMATION_HIT:
                enemy->setHealth(enemy->getHealth() - BASIC_ACTION_DAMAGE);
                break;
            case ANIMATION_COUNTER:
                player->setHealth(player->getHealth() - BASIC_ACTION_DAMAGE);
                break;
            }

             // check if player died
            if(player->getHealth() <= 0) {
                printf("You lost\n");
                displayMessage("You have lost! Press any key to try again.", 100, 10, window, 30, sf::Color(255,0,0));
                resetState(1);
                addRandomSet();
                window->Display();
                return false;
            }

            // check if enemy died
            if(enemy->getHealth() <= 0) {
                printf("You won\n");
                displayMessage("You have won! Press any key to continue.", 100, 10, window, 30, sf::Color(255,0,0));
                resetState(level+1);
                addRandomSet();
                window->Display();
                return false;
            }
        }
	}

    elapsedTime = window->GetFrameTime();

    // fix for between levels
    if(elapsedTime > .1) {
        elapsedTime = .1;
    }

	for (int i = 0; i < targetSets.size(); i++) {
        targetSets[i].update(elapsedTime);
        
        std::vector<Target>* targets = targetSets[i].getTargets();
        for (int j = 0; j < targets->size(); j++) {
            bool hit = false;
            
			// Remove Target if key is hit
			for (int k = 0; k < NUM_COLUMNS; k++) {
                if (keyPresses[k] && targets->at(j).getColumn() == k) {
                    float result = targets->at(j).hitCheck(&goals[k]);
                    
                    if (result == -1) {
                        targetSets[i].changeAccuracy(MISS_PENALTY);
                    } else {
                        targetSets[i].changeAccuracy(result);
                        targetSets[i].removeTarget(j);
                        hit = true;
                        if (j >= targets->size()) {
                            break;
                        }
                    }
                }
            }
            
			// Remove any off-screen Targets
            if (!hit && targets->at(j).getPosition().y > WINDOW_HEIGHT + targets->at(j).getSize()) {
                targetSets[i].removeTarget(j);
            }

			// Once all of the targetSet's targets are gone
            if (targets->size() == 0) {
                // Determine accuracy and play animation
                float accuracy = targetSets[i].getAccuracy();
               
                currentAnimation = rand() % basicActions.size();

                // TODO: Probably want to have these be dynamicaly
                // based on difficulty level instead of static.
                if (accuracy > 80) {
                    printf("Good\n");
                    currentAnimationType = ANIMATION_HIT;
                } else if (accuracy > 40) {
                    printf("OK\n");
                    currentAnimationType = ANIMATION_BLOCK;
                } else {
                    printf("Bad\n");
                    currentAnimationType = ANIMATION_COUNTER;
                }
                
				basicActions[currentAnimation]->selectAnimation(currentAnimationType);
                numActionFrames = basicActions[currentAnimation]->getNumAnimationFrames();

                // update health here when player/enemy are about to die
                switch(currentAnimationType) {
                case ANIMATION_HIT:
                    if(enemy->getHealth() <= BASIC_ACTION_DAMAGE)
                        enemy->setHealth(enemy->getHealth() - BASIC_ACTION_DAMAGE);
                    break;
                case ANIMATION_COUNTER:
                    if(player->getHealth() <= BASIC_ACTION_DAMAGE)
                        player->setHealth(player->getHealth() - BASIC_ACTION_DAMAGE);
                    break;
                }

                // Delete the actionSet
                targetSets.erase(targetSets.begin() + i);
				
                // reset targets, goals
				addRandomSet();
			}
        }
    }
    return true;
}

void Controller::draw() {
    window->Clear();

    window->Draw(background);
    
	if(numActionFrames > 0) {
		basicActions[currentAnimation]->draw(window);
	} else {
		idleAnimation.draw(window);
	}

	for(int i = 0; i < targetSets.size(); i++) {
		targetSets[i].draw(window);
	}

	for(int i = 0; i < goals.size(); i++) {
		goals[i].draw(window);
	}

    player->drawHealthBar(&star, window);
    enemy->drawHealthBar(&star, window);

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
                case sf::Key::T:
                    currentAnimationType++;
                    currentAnimationType %= NUM_ANIMATION_TYPES;
                    basicActions[currentAnimation]->selectAnimation(currentAnimationType);
                    break;
                case sf::Key::Y:
                    currentAnimation++;
                    currentAnimation %= basicActions.size();
                    break;
                case sf::Key::Escape:
                    if(gameStatus == GAME_PAUSE) {
                        gameStatus = GAME_PLAY;
                    } else {
                        gameStatus = GAME_PAUSE;
                    }
                    break;
            }

            if(gameStatus == GAME_WAIT_FOR_INPUT) {
                gameStatus = GAME_PLAY;
                targetSets.clear();
                addRandomSet();
            }
        }
    }
}

void Controller::loadResources() {
    if (!targetImg.LoadFromFile("target.png") ||
        !goalImg.LoadFromFile("goal.png") ||
        !backgroundImg.LoadFromFile("background.png") ||
        !starImg.LoadFromFile("star1-25.png")) {
        
        // This should quit or throw an exception
        printf("Error loading resources - controller\n");
    }
}

void Controller::initializeObjects() {
	srand(time(NULL));

    background.SetImage(backgroundImg);
    star.SetImage(starImg);
    
    // Add Kick animations
    Action* kick = new Action();
    kick->addAnimation(ANIMATION_HIT, "Actions/Kick_Hit/Kick_Hit", NUM_KICK_HIT_FRAMES);
    kick->addAnimation(ANIMATION_BLOCK, "Actions/Kick_Block/Kick_Block", NUM_KICK_BLOCK_FRAMES);
    kick->addAnimation(ANIMATION_COUNTER, "Actions/Kick_Counter/Kick_Counter", NUM_KICK_COUNTER_FRAMES);
    basicActions.push_back(kick);
    
    // Add Punch animations
    Action* punch = new Action();
    punch->addAnimation(ANIMATION_HIT, "Actions/Punch_Hit/Punch_Hit", NUM_PUNCH_HIT_FRAMES);
    punch->addAnimation(ANIMATION_BLOCK, "Actions/Punch_Block/Punch_Block", NUM_PUNCH_BLOCK_FRAMES);
    punch->addAnimation(ANIMATION_COUNTER, "Actions/Punch_Counter/Punch_Counter", NUM_PUNCH_COUNTER_FRAMES);
    basicActions.push_back(punch);
    
    // Add Headbutt animations
    Action* headbutt = new Action();
    headbutt->addAnimation(ANIMATION_HIT, "Actions/Headbutt_Hit/Headbutt_Hit", NUM_HEADBUTT_HIT_FRAMES);
    headbutt->addAnimation(ANIMATION_BLOCK, "Actions/Headbutt_Block/Headbutt_Block", NUM_HEADBUTT_BLOCK_FRAMES);
    headbutt->addAnimation(ANIMATION_COUNTER, "Actions/Headbutt_Counter/Headbutt_Counter", NUM_HEADBUTT_COUNTER_FRAMES);
    basicActions.push_back(headbutt);

    // Maybe load combos when they are purchased instead of now to speed up loading?
    /*
    ComboAction* punchPunch = new ComboAction();
    punchPunch->addAnimation("ComboActions/Punch_Punch_Combo/Punch_Punch_Combo", NUM_PUNCH_PUNCH_FRAMES);
    comboActions.push_back(punchPunch);
    
    ComboAction* punchKick = new ComboAction();
    punchKick->addAnimation("ComboActions/Punch_Kick_Combo/Punch_Kick_Combo", NUM_PUNCH_KICK_FRAMES);
    comboActions.push_back(punchKick);
    
    ComboAction* punchHeadbutt = new ComboAction();
    punchHeadbutt->addAnimation("ComboActions/Punch_Headbutt_Combo/Punch_Headbutt_Combo", NUM_PUNCH_HEADBUTT_FRAMES);
    comboActions.push_back(punchHeadbutt);
    
    ComboAction* kickPunch = new ComboAction();
    kickPunch->addAnimation("ComboActions/Kick_Punch_Combo/Kick_Punch_Combo", NUM_KICK_PUNCH_FRAMES);
    comboActions.push_back(kickPunch);

    ComboAction* kickKick = new ComboAction();
    kickKick->addAnimation("ComboActions/Kick_Kick_Combo/Kick_Kick_Combo", NUM_KICK_KICK_FRAMES);
    comboActions.push_back(kickKick);
    
    ComboAction* kickHeadbutt = new ComboAction();
    kickHeadbutt->addAnimation("ComboActions/Kick_Headbutt_Combo/Kick_Headbutt_Combo", NUM_KICK_HEADBUTT_FRAMES);
    comboActions.push_back(kickHeadbutt);
    
    ComboAction* headbuttPunch = new ComboAction();
    headbuttPunch->addAnimation("ComboActions/Headbutt_Punch_Combo/Headbutt_Punch_Combo", NUM_HEADBUTT_PUNCH_FRAMES);
    comboActions.push_back(headbuttPunch);
    
    ComboAction* headbuttKick = new ComboAction();
    headbuttKick->addAnimation("ComboActions/Headbutt_Kick_Combo/Headbutt_Kick_Combo", NUM_HEADBUTT_KICK_FRAMES);
    comboActions.push_back(headbuttKick);
    */
    
    // Add Idle animation
    idleAnimation.init("Actions/Idle/Idle", NUM_IDLE_FRAMES);

    // Death animation?
    
    goals.push_back(Goal(&goalImg, 528, 0));
    goals.push_back(Goal(&goalImg, 528, 1));
    goals.push_back(Goal(&goalImg, 528, 2));
    goals.push_back(Goal(&goalImg, 528, 3));
}

void Controller::addRandomSet() {
	int numTargets = MIN_NUM_TARGETS + rand() % (MAX_NUM_TARGETS - MIN_NUM_TARGETS);
    //int speed = TARGET_SPEED * level * SPEED_RATIO;
    int speed = SPEED_START + (level * SPEED_INCREASE);

	targetSets.push_back(TargetSet());
	for(int i = 0; i < numTargets; i++) {
		targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, speed, rand() % NUM_COLUMNS, COLUMN_WIDTH * i));
	}
}

void Controller::randomizeGoals() {
    goals.clear();

    int pos = 250 + rand() % 250;

    for(int i = 0; i < NUM_COLUMNS; i++) {
        goals.push_back(Goal(&goalImg, pos, i));
    }
}

void Controller::displayMessage(std::string message, int x, int y, sf::RenderTarget* target, int size, sf::Color color) {
    sf::String text;
    text.SetText(message);
    text.SetColor(color);
    text.SetFont(sf::Font::GetDefaultFont());
    text.SetSize(size);
    text.Move(x, y);

    target->Draw(text);
}

void Controller::resetState(int level) {
    currentAnimation = 0;
    this->level = level;
	numActionFrames = 0;

    player->setHealth(PLAYER_HEALTH);
    enemy->setHealth(PLAYER_HEALTH);

    gameStatus = GAME_WAIT_FOR_INPUT;
}