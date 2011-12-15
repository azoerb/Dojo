#include "Controller.h"
#include <math.h>

Controller::Controller() {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dojo Hero");
    window->UseVerticalSync(true);
    
    player = new Player(PLAYER_HEALTH);
    enemy = new Enemy(BASIC_ENEMY_HEALTH);

	criticalAttack = false;
	criticalFrame = 0;
    
    gameState = GAME_MENU;
    menuState = STATE_MAIN_MENU;

    // display loading image
    loadingImg.LoadFromFile("loading.png");
    loading.SetImage(loadingImg);
    
    drawLoadScreen();

    loadResources();
    initializeObjects();
}

Controller::~Controller() {
    if (window) { delete window; }
	if (player) { delete player; }
	if (enemy) { delete enemy; }
    
    for (int i = 0; i < basicActions.size(); i++) {
        if (basicActions.at(i)) { delete basicActions[i]; }
    }
    
    for (int i = 0; i < specialActions.size(); i++) {
        if (specialActions.at(i)) { delete specialActions[i]; }
    }
    
    for (int i = 0; i < comboActions.size(); i++) {
        if (comboActions.at(i)) { delete comboActions[i]; }
    }
    
}

void Controller::mainLoop() {
    while (window->IsOpened()) {
        processEvents();
        update();
        draw();
    }
}

void Controller::update() {
    if (gameState == GAME_PLAY) {
        // use idle animation after action animation is complete
        if(numActionFrames > 0) {
            numActionFrames--;

            // update health
            if(numActionFrames == 0) {
                switch(currentAnimationType) {
                case ANIMATION_HIT:
					if (criticalAttack) {
						enemy->setHealth(enemy->getHealth() - CRITICAL_ACTION_DAMAGE);
						criticalAttack = false;
					} else {
						enemy->setHealth(enemy->getHealth() - BASIC_ACTION_DAMAGE);
					}
                    break;
                case ANIMATION_COUNTER:
                    player->setHealth(player->getHealth() - BASIC_ACTION_DAMAGE);
                    break;
                }

                 // check if player died
                if(player->getHealth() <= 0) {
                    gameState = GAME_MENU;
                    window->Display();
                }

                // check if enemy died
                if(enemy->getHealth() <= 0) {
                    //
                    resetState(level+1);
                    window->Display();
                }
            }
        }

        elapsedTime = window->GetFrameTime();

        // fix for between levels
        if(elapsedTime > .1) {
            elapsedTime = .1;
        }

        // set to true when we encounter the first target in the respective column
        // to avoid checking far away targets
        // this fixes the problem of "missing" all targets in the column
        bool found[4] = {false, false, false, false};

        for (int i = 0; i < targetSets.size(); i++) {
            targetSets[i].update(elapsedTime);
            
            std::vector<Target>* targets = targetSets[i].getTargets();
            for (int j = 0; j < targets->size(); j++) {
                bool hit = false;
                
                // Remove Target if key is hit
                int col = targets->at(j).getColumn();
                if (keyPresses[col] && !found[col]) {
                    float result = targets->at(j).hitCheck(&goals[col]);
                    if (result == -1) {
                        if (targets->at(j).getPosition().y < goals[col].getPosition().y) {
                            found[col] = true;
							targetSets[i].changeAccuracy(MISS_PENALTY);
                            goals[col].goalMiss();
                        }
                    } else {
                        found[col] = true;
                        targetSets[i].changeAccuracy(result);
                        targetSets[i].removeTarget(j);
                        hit = true;

						if (result >= HIT_BOUND) {
                            goals[col].goalHit(true);
                        } else {
                            goals[col].goalHit(false);
                        }

                        if (j >= targets->size()) {
                            break;
                        }
                    }
                }
                
                // Remove any off-screen Targets
                if (!hit && targets->at(j).getPosition().y > WINDOW_HEIGHT + targets->at(j).getSize()) {
                    targetSets[i].changeAccuracy(MISS_PENALTY);
                    targetSets[i].removeTarget(j);
                }
            }

            // Once all of the targetSet's targets are gone
            if (targets->size() == 0) {
                // Determine accuracy and play animation
                float accuracy = targetSets[i].getAccuracy();

                currentAnimation = rand() % basicActions.size();
                
                // TODO: Still want these to be modified by the current level.
                if (accuracy < BLOCK_BOUND) {
                    currentAnimationType = ANIMATION_COUNTER;
                } else if (accuracy < HIT_BOUND) {
                    currentAnimationType = ANIMATION_BLOCK;
                } else {
                    currentAnimationType = ANIMATION_HIT;

					if (accuracy > CRITICAL_BOUND) {
						criticalFrame = NUM_CRITICAL_FRAMES;
                        criticalAttack = true;
					}
                }
                    
                basicActions[currentAnimation]->selectAnimation(currentAnimationType);
                numActionFrames = basicActions[currentAnimation]->getNumAnimationFrames();

                // update health here when player/enemy are about to die
                switch(currentAnimationType) {
                case ANIMATION_HIT:
					if (criticalAttack) {
						if (enemy->getHealth() <= CRITICAL_ACTION_DAMAGE) {
							enemy->setHealth(0);
							criticalAttack = false;
						}
					} else {
						if (enemy->getHealth() <= BASIC_ACTION_DAMAGE) {
							enemy->setHealth(0);
						}
					}
                    break;
                case ANIMATION_COUNTER:
                    if (player->getHealth() <= BASIC_ACTION_DAMAGE)
                        player->setHealth(0);
                    break;
                }

                // Delete the actionSet
                targetSets.erase(targetSets.begin() + i);
                    
                // reset targets, goals
                addRandomSet();
            }

            // Check if key is pressed and there is no target to hit
            for(int i = 0; i < NUM_COLUMNS; i++) {
                if(keyPresses[i] && !found[i] && targetSets.size() > 0) {
					targetSets[0].changeAccuracy(MISS_PENALTY);
                    goals[i].goalMiss();
                }
            }
        }
    }
}

void Controller::draw() {
    switch (gameState) {
        case GAME_MENU:
            switch (menuState) {
                case STATE_MAIN_MENU:
                    window->Clear();
                    window->Draw(menuNewGame);
                    window->Draw(menuInstructions);
                    window->Draw(menuSelector);
                    window->Display();
                    break;
                    
                case STATE_NEW_GAME:
                    window->Clear();
                    window->Draw(menuBeginner);
                    window->Draw(menuIntermediate);
                    window->Draw(menuAdvanced);
                    window->Draw(menuSelector);
                    window->Display();
                    break;
                    
                case STATE_INSTRUCTIONS:
                    window->Clear();
                    displayText("Use QWER keys to Play", 180, 200, window, 40, sf::Color(255, 255, 255));
                    window->Display();
                    break;
            }
            break;
        case GAME_PLAY:
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
            
            std::stringstream ss;
            ss << level;
            
            displayText("You", 20, 60, window, 20, sf::Color(255,255,255));
            displayText("Enemy", WINDOW_WIDTH - 70, 60, window, 20, sf::Color(255,255,255));
            displayText("Level " + ss.str(), WINDOW_WIDTH / 2 - 40, 10, window, 30, sf::Color(255,255,255));
            player->drawHealthBar(&star, window);
            enemy->drawHealthBar(&star, window);
            
			if (criticalFrame > 0) {
				displayText("CRITICAL", WINDOW_WIDTH /2 - 100, 100, window, 50, sf::Color(255, 0, 0, 255 * criticalFrame / NUM_CRITICAL_FRAMES), true);
				criticalFrame--;
			}

            window->Display();
            break;
    }
}

void Controller::drawLoadScreen() {
    window->Clear();
    window->Draw(loading);
    window->Display();
}

void Controller::processEvents() {
    // reset all key presses
    for(int i = 0; i < NUM_COLUMNS; i++) {
        keyPresses[i] = false;
    }
    
    
    sf::Event Event;
    while (window->GetEvent(Event)) {
        if (Event.Type == sf::Event::Closed) {
            window->Close();
        } else if (Event.Type == sf::Event::KeyPressed) {
            switch(Event.Key.Code) {
                case sf::Key::Q:
                    keyPresses[0] = true;
                    break;
                    
                case sf::Key::W:
                    if (NUM_COLUMNS >= 2) {
                        keyPresses[1] = true;
                    }
                    break;
                    
                case sf::Key::E:
                    if (NUM_COLUMNS >= 3) {
                        keyPresses[2] = true;
                    }
                    break;
                    
                case sf::Key::R:
                    if (NUM_COLUMNS >= 4) {
                        keyPresses[3] = true;
                    }
                    break;
                    
                case sf::Key::T:
                    if (NUM_COLUMNS >= 5) {
                        keyPresses[4] = true;
                    }
                    break;
                    
                case sf::Key::Y:
                    if (NUM_COLUMNS >= 6) {
                        keyPresses[5] = true;
                    }
                    break;
                    
                case sf::Key::Escape:
                    if(gameState == GAME_PAUSE) {
                        gameState = GAME_PLAY;
                    } else if (gameState == GAME_PLAY) {
                        gameState = GAME_PAUSE;
                    } else if (gameState == GAME_MENU && (menuState == STATE_NEW_GAME || menuState == STATE_INSTRUCTIONS)) {
                        resetMenuSelector();
                        menuState = STATE_MAIN_MENU;
                    }
                    break;
                    
                case sf::Key::Space:
                    if (gameState == GAME_MENU) {
                        if (menuState == STATE_MAIN_MENU) {
                            if (menuSelectorPosition == 0) {
                                // New Game Menu
                                resetMenuSelector();
                                menuState = STATE_NEW_GAME;
                            } else {
                                // Instructions Menu
                                resetMenuSelector();
                                menuState = STATE_INSTRUCTIONS;
                            }
                        } else if (menuState == STATE_NEW_GAME) {
                            // Start the game
                            resetMenuSelector();
                            menuState = STATE_MAIN_MENU;
                            switch (menuSelectorPosition) {
                                case 0:
                                    // Easy
                                    resetState(1);
                                    break;
                                case 1:
                                    // Medium
                                    resetState(1);
                                    break;
                                case 2:
                                    // Hard
                                    resetState(1);
                                    break;
                            }
                        } else {
                            // Back to Main Menu
                            resetMenuSelector();
                            menuState = STATE_MAIN_MENU;
                        }
                    }
                    break;
                    
                case sf::Key::Up:
                    if (gameState == GAME_MENU) {
                        if (menuSelectorPosition > 0) {
                            menuSelectorPosition--;
                            menuSelector.SetPosition(menuSelector.GetPosition().x, menuSelector.GetPosition().y - 60);
                        }
                    }
                    break;
                    
                case sf::Key::Down:
                    if (gameState == GAME_MENU) {
                        if (menuState == STATE_MAIN_MENU) {
                            if (menuSelectorPosition < NUM_MENU_ITEMS - 1) {
                                menuSelectorPosition++;
                                menuSelector.SetPosition(menuSelector.GetPosition().x, menuSelector.GetPosition().y + 60);
                            }
                        } else if (menuState == STATE_NEW_GAME) {
                            if (menuSelectorPosition < NUM_DIFFICULTY_LEVELS - 1) {
                                menuSelectorPosition++;
                                menuSelector.SetPosition(menuSelector.GetPosition().x, menuSelector.GetPosition().y + 60);
                            }
                        }
                    }
                    break;
            }
        }
    }
}

void Controller::loadResources() {
    if (!targetImg.LoadFromFile("target.png") ||
        !goalImg.LoadFromFile("goal.png") ||
        !goalAltImg.LoadFromFile("goal-alt.png") ||
        !backgroundImg.LoadFromFile("background.png") ||
        !starImg.LoadFromFile("star1-25.png") ||
        !menuNewGameImg.LoadFromFile("menu-new-game.png") ||
        !menuInstructionsImg.LoadFromFile("menu-instructions.png") ||
        !menuBeginnerImg.LoadFromFile("menu-beginner.png") ||
        !menuIntermediateImg.LoadFromFile("menu-intermediate.png") ||
        !menuAdvancedImg.LoadFromFile("menu-advanced.png")) {
        
        // This should quit or throw an exception
        printf("Error loading resources - controller\n");
    }
}

void Controller::initializeObjects() {
	srand(time(NULL));

    background.SetImage(backgroundImg);
    star.SetImage(starImg);
    menuNewGame.SetImage(menuNewGameImg);
    menuInstructions.SetImage(menuInstructionsImg);
    menuBeginner.SetImage(menuBeginnerImg);
    menuIntermediate.SetImage(menuIntermediateImg);
    menuAdvanced.SetImage(menuAdvancedImg);
    menuSelector.SetImage(starImg);
    
    menuNewGame.SetPosition(50, 30);
    menuInstructions.SetPosition(50, 90);
    
    menuBeginner.SetPosition(50, 30);
    menuIntermediate.SetPosition(50, 90);
    menuAdvanced.SetPosition(50, 150);
    
    menuSelector.SetPosition(10, 45);

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

    for (int i = 0; i < NUM_COLUMNS; i++) {
        goals.push_back(Goal(&goalImg, &goalAltImg, 528, i));
    }
}

void Controller::addRandomSet() {
	//int numTargets = MIN_NUM_TARGETS + rand() % (MAX_NUM_TARGETS - MIN_NUM_TARGETS);
    //int speed = TARGET_SPEED * level * SPEED_RATIO;
	int numTargets = MIN_NUM_TARGETS + level + rand() % (2*level);
    if(numTargets > MAX_NUM_TARGETS) {
        numTargets = MAX_NUM_TARGETS;
    }
    int speed = SPEED_START + (level * SPEED_INCREASE);

	targetSets.push_back(TargetSet());
    int y = 0;
	for(int i = 0; i < numTargets; i++) {
        int col = rand() % NUM_COLUMNS;
		targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, speed, col, y));

        // add doubles, triples, quadruples
        /*
        if(level > 2 && rand() % 10 < 2) {
            targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, speed, (col+1) % NUM_COLUMNS, y));

            if(level > 4 && rand() % 10 < 5) {
                targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, speed, (col+2) % NUM_COLUMNS, y));

                if(level > 6 && rand() % 10 < 5) {
                    targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, speed, (col+3) % NUM_COLUMNS, y));
                }
            }
        }
        */

        y += COLUMN_WIDTH;
	}
}

void Controller::randomizeGoals() {
    goals.clear();

    int pos = 250 + rand() % 250;

    for(int i = 0; i < NUM_COLUMNS; i++) {
        goals.push_back(Goal(&goalImg, &goalAltImg, pos, i));
    }
}

void Controller::displayText(std::string message, int x, int y, sf::RenderTarget* target, int size, sf::Color color, bool bold) {
    sf::String text;
    text.SetText(message);
    text.SetColor(color);
    text.SetFont(sf::Font::GetDefaultFont());
    text.SetSize(size);
    text.SetPosition(x, y);

	if (bold) {
		text.SetStyle(sf::String::Bold);
	}

    target->Draw(text);
}

void Controller::resetState(int level) {
    currentAnimation = 0;
    this->level = level;
	numActionFrames = 0;

    player->setHealth(PLAYER_HEALTH);
    enemy->setHealth(PLAYER_HEALTH);

    gameState = GAME_PLAY;
    
    targetSets.clear();
    for (int i = 0; i < NUM_COLUMNS; i++) {
        keyPresses[i] = false;
    }
    addRandomSet();
}

void Controller::resetMenuSelector() {
    menuSelectorPosition = 0;
    menuSelector.SetPosition(10, 45);
}