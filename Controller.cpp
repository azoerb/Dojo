#include "Controller.h"
#include <math.h>
#include <SFML/Audio.hpp>

Controller::Controller() {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dojo Hero");
    window->UseVerticalSync(true);
    
    player = new Player(PLAYER_HEALTH);
    enemy = new Enemy(BASIC_ENEMY_HEALTH);

	criticalAttack = false;
    comboAttack = false;
	criticalFrame = 0;
    hitCounter = 0;
    
    goToMenu = false;
    
    endlessMode = false;
    
    gameState = GAME_MENU;
    menuState = STATE_INSTRUCTIONS;

    // display loading image
    loadingImg.LoadFromFile("loading.png");
    loading.SetImage(loadingImg);
    
    drawLoadScreen();

    loadResources();
    initializeObjects();

    #ifdef USE_SOUND
    soundManager->playMusic(MUSIC_0);
    #endif
}

Controller::~Controller() {
    if (window) { delete window; }
	if (player) { delete player; }
	if (enemy) { delete enemy; }

    #ifdef USE_SOUND
    if (soundManager) { delete soundManager; }
    #endif
    
    for (int i = 0; i < basicActions.size(); i++) {
        if (basicActions.at(i)) { delete basicActions[i]; }
    }
    
    for (int i = 0; i < comboActions.size(); i++) {
        if (comboActions.at(i)) { delete comboActions[i]; }
    }
    
}

void Controller::mainLoop() {
    dojoLevel = 0;
	lives = 3;
	points = 0;
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
					if (criticalAttack || comboAttack) {
						enemy->setHealth(enemy->getHealth() - CRITICAL_ACTION_DAMAGE);

                        // Attack+ powerup
                        if (powerupCounter[0] > 0) {
                            enemy->setHealth(enemy->getHealth() - BASIC_ACTION_DAMAGE);
                        }
					} else {
						enemy->setHealth(enemy->getHealth() - BASIC_ACTION_DAMAGE);

                        // Attack+ powerup
                        if (powerupCounter[0] > 0) {
                            enemy->setHealth(enemy->getHealth() - BASIC_ACTION_DAMAGE);
                        }
					}
                    break;
                case ANIMATION_COUNTER:
                    player->setHealth(player->getHealth() - BASIC_ACTION_DAMAGE);
                    break;
                }

                 // check if player died
                if (player->getHealth() <= 0) {
                    lives--;
					//check to see if you have 3 lives left..
					if (lives > 0) {
						//we have two options, restart the level, or just continue - like I am.
						player->setHealth(PLAYER_HEALTH);
						//now display menu?
					} else {	
						lives = 3;
                        level = 1;
						window->Clear();
						gameOver.Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
						window->Draw(gameOver);
						window->Display();

						gameState = GAME_WAIT_FOR_INPUT;
                        goToMenu = true;
					}
                }

                // check if enemy died
                if(enemy->getHealth() <= 0) {
                    //animation to next level
					
					//transition to shop
					//insert shop stuff here..
                    
					
					//check to see if you are in story mode
					if(!endlessMode) { 
						switch (dojoLevel) {
							case 0:
								dojo.SetImage(dojoImg0);
								break;
							case 1:
								dojo.SetImage(dojoImg1);
								break;
							case 2:
								dojo.SetImage(dojoImg2);
								break;
							case 3:
								dojo.SetImage(dojoImg3);
								break;
							case 4:
								dojo.SetImage(dojoImg4);
								break;
							case 5:
								dojo.SetImage(dojoImg5);
								break;
							case 6:
								dojo.SetImage(dojoImg6);
								break;
							case 7:
								dojo.SetImage(dojoImg7);
								break;
							case 8:
								dojo.SetImage(dojoImg8);
								break;
							default:
                                printf("Error - default dojo switch\n");
								window->Close();
								break;
						}
                        
						window->Clear();
						dojo.Resize(WINDOW_WIDTH,WINDOW_HEIGHT);
						window->Draw(dojo);

                        if ((level-2)/2 < comboActions.size() && (level-2)/2 >= 0) {
                            comboActions[(level-2)/2]->unlock();
                            displayText("You have unlocked the " + comboDescriptions[(level-2)/2] + " combo!", 0, 0, window, 24, sf::Color(255, 255, 255), true);
                        }

						window->Display();
                        
						if(dojoLevel == 8) {
							//going back to the main menu, the player won the game
							gameState = GAME_WAIT_FOR_INPUT;
                            goToMenu = true;
							dojoLevel = 0;
						} else {
							//just go to the next level
							dojoLevel++;
							resetState(level+1);
						}
					} else { //endless mode
						//go to next level
						resetState(level+1);
						window->Display();
                        gameState = GAME_PLAY;
					}
                }
            }
        }

        elapsedTime = window->GetFrameTime();

        // fix for between levels
        if(elapsedTime > .1) {
            elapsedTime = .1;
        }

        // update powerup counters
        for (int i = 0; i < NUM_POWERUPS; i++) {
            if (powerupCounter[i] > 0) {
                powerupCounter[i]--;
            }
        }

        // set to true when we encounter the first target in the respective column
        // to avoid checking far away targets
        // this fixes the problem of "missing" all targets in the column
        bool found[NUM_COLUMNS];
        for (int i = 0; i < NUM_COLUMNS; i++) {
            found[i] = false;
        }

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
                            hitCounter = 0;
                        }
                    } else {
                        found[col] = true;

                        if (powerupSet && targets->at(j).isSpecial()) {
                            powerupTargetCount --;
                        }

                        targetSets[i].changeAccuracy(result);
                        targetSets[i].removeTarget(j);
                        hit = true;
                        hitCounter++;

						if (result >= HIT_BOUND + ((difficultyLevel - 1) * (100 - HIT_BOUND))) {
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
                    hitCounter = 0;
                }
            }

            // Once all of the targetSet's targets are gone
            if (targets->size() == 0) {

                // check for powerup
                if (powerupSet && powerupTargetCount == 0) {
                    powerupTargetCount--;

                    int powerup = rand() % NUM_POWERUPS;
                    powerupCounter[powerup] = POWERUP_DURATION;
                }

                // Determine accuracy and play animation
                float accuracy = targetSets[i].getAccuracy();
                criticalAttack = false;
                comboAttack = false;

                // accuracy boost powerup
                if (powerupCounter[1] > 0) {
                    accuracy += 25;
                }

                currentAnimation = rand() % basicActions.size();
                                
                if (accuracy < BLOCK_BOUND + ((difficultyLevel - 1) * (100 - BLOCK_BOUND))) {
                    currentAnimationType = ANIMATION_COUNTER;

                } else if (accuracy < HIT_BOUND + ((difficultyLevel - 1) * (100 - HIT_BOUND))) {
                    // use combo attack
                    if (hitCounter > COMBO_THRESHOLD_2) {
						criticalFrame = NUM_CRITICAL_FRAMES;

                        if (getRandomComboAttack() != -1) {
                            currentAnimation = getRandomComboAttack();
                            comboAttack = true;
                        }
                    }
                    currentAnimationType = ANIMATION_BLOCK;
                } else {
                    currentAnimationType = ANIMATION_HIT;

                    // use combo attack
                    if (hitCounter > COMBO_THRESHOLD_1) {
						criticalFrame = NUM_CRITICAL_FRAMES;

                        if (getRandomComboAttack() != -1) {
                            currentAnimation = getRandomComboAttack();
                            comboAttack = true;
                        }
                    }
                    // critical attack
					else if (accuracy > CRITICAL_BOUND + ((difficultyLevel - 1) * (100 - CRITICAL_BOUND))) {
						criticalFrame = NUM_CRITICAL_FRAMES;
                        criticalAttack = true;
					}
                }

                if (comboAttack) {
                    numActionFrames = comboActions[currentAnimation]->getNumAnimationFrames();
                } else {
                    basicActions[currentAnimation]->selectAnimation(currentAnimationType);
                    numActionFrames = basicActions[currentAnimation]->getNumAnimationFrames();
                }

                // update health here when player/enemy are about to die
                bool addNewSet = true;
                switch(currentAnimationType) {
                case ANIMATION_HIT:
					if (criticalAttack || comboAttack) {
						if (enemy->getHealth() <= CRITICAL_ACTION_DAMAGE || (powerupCounter[0] > 0 && enemy->getHealth() <= CRITICAL_ACTION_DAMAGE + BASIC_ACTION_DAMAGE)) {
							enemy->setHealth(0);
                            addNewSet = false;
                        }
					} else {
                        if (enemy->getHealth() <= BASIC_ACTION_DAMAGE || (powerupCounter[0] > 0 && enemy->getHealth() <= 2*BASIC_ACTION_DAMAGE)) {
							enemy->setHealth(0);
                            addNewSet = false;
						}
					}
                    break;
                case ANIMATION_COUNTER:
                    if (player->getHealth() <= BASIC_ACTION_DAMAGE) {
                        player->setHealth(0);
                    }
                    break;
                }

                // Delete the actionSet
                targetSets.erase(targetSets.begin() + i);
                    
                // reset targets, goals
                if (addNewSet) {
                    addRandomSet();
                }
            }

            // Check if key is pressed and there is no target to hit
            for(int i = 0; i < currentNumColumns; i++) {
                if(keyPresses[i] && !found[i] && targetSets.size() > 0) {
					targetSets[0].changeAccuracy(MISS_PENALTY);
                    goals[i].goalMiss();
                    hitCounter = 0;
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
                    window->Draw(menuStoryMode);
                    window->Draw(menuEndless);
                    window->Draw(menuInstructions);
                    window->Draw(menuSelector);
                    window->Display();
                    break;
                    
                case STATE_NEW_GAME:
                    window->Clear();
                    window->Draw(menuBeginner);
                    window->Draw(menuIntermediate);
                    window->Draw(menuAdvanced);
                    window->Draw(menuAdvanced2);
                    window->Draw(menuSelector);
                    window->Display();
                    break;
                    
                case STATE_INSTRUCTIONS:
                    window->Clear();
                    displayText("Use QWER keys to Play", 180, 200, window, 40, sf::Color(255, 255, 255));
                    displayText("Try to hit each target in the center", 80, 250, window, 40, sf::Color(255, 255, 255));
                    displayText("Escape to pause", 230, 300, window, 40, sf::Color(255, 255, 255));
                    displayText("Space and arrow keys to navigate menus", 30, 350, window, 40, sf::Color(255, 255, 255));
                    window->Display();
                    break;
            }
            break;
        case GAME_PLAY:
            window->Clear();
            window->Draw(background);
            /*if (!endlessMode) {
                //window->Draw(coin);
            
                std::stringstream type; 
                type << points; 
                
                displayText(type.str(), WINDOW_WIDTH/2 + 60, WINDOW_HEIGHT - 40, window, 20, sf::Color(255,255,255));
            }*/
            //draw lives accordingly
            for(int i = 0; i < lives; i++) {
                heart.SetPosition(WINDOW_WIDTH/2 - 50 + 35*i, WINDOW_HEIGHT - 40);
                window->Draw(heart);
            }

            // draw hit counter
            std::stringstream count;
            count << hitCounter;

            if (hitCounter >= COMBO_THRESHOLD_2) {
                displayText("Combo: " + count.str(), WINDOW_WIDTH/2 - 90, WINDOW_HEIGHT - 90, window, 32, sf::Color(255, 0, 0), true);
            } else if (hitCounter >= COMBO_THRESHOLD_1) {
                displayText("Combo: " + count.str(), WINDOW_WIDTH/2 - 90, WINDOW_HEIGHT - 90, window, 32, sf::Color(255, 255, 0), true);
            } else {
                displayText("Combo: " + count.str(), WINDOW_WIDTH/2 - 90, WINDOW_HEIGHT - 90, window, 32, sf::Color(255, 255, 255), true);
            }

            // draw current powerups
            // 2X attack
            if (powerupCounter[0] > 0) {
                displayText("ATTACK+", WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT - 110, window, 16, sf::Color(255, 0, 0), true);
            }
            // accuracy boost
            if (powerupCounter[1] > 0) {
                displayText("ACCURACY+", WINDOW_WIDTH/2, WINDOW_HEIGHT - 110, window, 16, sf::Color(255, 0, 0), true);
            }

            if(numActionFrames > 0) {
                if (comboAttack) {
                    comboActions[currentAnimation]->draw(window);
                } else {
                    basicActions[currentAnimation]->draw(window);
                }
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
                if (comboAttack) {
                    displayText("COMBO", WINDOW_WIDTH /2 - 80, 100, window, 50, sf::Color(255, 0, 0, 255 * criticalFrame / NUM_CRITICAL_FRAMES), true);
                } else {
				    displayText("CRITICAL", WINDOW_WIDTH /2 - 100, 100, window, 50, sf::Color(255, 0, 0, 255 * criticalFrame / NUM_CRITICAL_FRAMES), true);
                }
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
                    #ifdef USE_SOUND
                    if (gameState == GAME_PLAY)
	                    soundManager->playSound(TARGET_SOUND_0);
                    #endif
                    break;
                    
                case sf::Key::W:
                    if (currentNumColumns >= 2) {
                        keyPresses[1] = true;
                        #ifdef USE_SOUND
                        if (gameState == GAME_PLAY)
	                        soundManager->playSound(TARGET_SOUND_1);
                        #endif
                    }
                    break;
                    
                case sf::Key::E:
                    if (currentNumColumns>= 3) {
                        keyPresses[2] = true;
                        #ifdef USE_SOUND
                        if (gameState == GAME_PLAY)
	                        soundManager->playSound(TARGET_SOUND_2);
                        #endif
                    }
                    break;
                    
                case sf::Key::R:
                    if (currentNumColumns >= 4) {
                        keyPresses[3] = true;
                        #ifdef USE_SOUND
                        if (gameState == GAME_PLAY)
	                        soundManager->playSound(TARGET_SOUND_3);
                        #endif
                    }
                    break;
                    
                case sf::Key::U:
                    if (currentNumColumns >= 5) {
                        keyPresses[4] = true;
                        #ifdef USE_SOUND
                        if (gameState == GAME_PLAY)
	                        soundManager->playSound(TARGET_SOUND_4);
                        #endif
                    }
                    break;
                    
                case sf::Key::I:
                    if (currentNumColumns >= 6) {
                        keyPresses[5] = true;
                        #ifdef USE_SOUND
                        if (gameState == GAME_PLAY)
	                        soundManager->playSound(TARGET_SOUND_5);
                        #endif
                    }
                    break;

                case sf::Key::O:
                    if (currentNumColumns >= 7) {
                        keyPresses[6] = true;
                        #ifdef USE_SOUND
                        if (gameState == GAME_PLAY)
	                        soundManager->playSound(TARGET_SOUND_6);
                        #endif
                    }
                    break;

                case sf::Key::P:
                    if (currentNumColumns >= 8) {
                        keyPresses[7] = true;
                        #ifdef USE_SOUND
                        if (gameState == GAME_PLAY)
	                        soundManager->playSound(TARGET_SOUND_7);
                        #endif
                    }
                    break;

                case sf::Key::K:
                    if (gameState == GAME_PLAY)
                        player->setHealth(0);
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
                    
                case sf::Key::Return:
                    gameState = GAME_MENU;
                    lives = 3;
                    points = 0;
                    break;

                case sf::Key::Space:
                    if (gameState == GAME_WAIT_FOR_INPUT) {
                        if (goToMenu) {
                            gameState = GAME_MENU;
                            goToMenu = false;
                        } else {
                            gameState = GAME_PLAY;
                        }
                    } else if (gameState == GAME_MENU) {
                        if (menuState == STATE_MAIN_MENU) {
                            switch (menuSelectorPosition) {
                                case 0: // Story mode
                                    resetMenuSelector();
                                    endlessMode = false;
                                    menuState = STATE_NEW_GAME;
                                    break;
                                case 1: // Endless mode
                                    resetMenuSelector();
                                    endlessMode = true;
                                    menuState = STATE_NEW_GAME;
                                    break;
                                case 2: // Instructions Menu
                                    resetMenuSelector();
                                    menuState = STATE_INSTRUCTIONS;
                                    break;
                            }
                        } else if (menuState == STATE_NEW_GAME) {
                            // Start the game
                            menuState = STATE_MAIN_MENU;
                            loadCombos(menuSelectorPosition, endlessMode);

                            switch (menuSelectorPosition) {
                                case 0:
                                    // Easy
                                    difficultyLevel = 1.0;
                                    currentNumColumns = 4;
                                    resetState(1);
                                    break;
                                case 1:
                                    // Medium
                                    difficultyLevel = DIFFICULTY_MEDIUM;
                                    currentNumColumns = 4;
                                    resetState(1);
                                    break;
                                case 2:
                                    // Hard
                                    difficultyLevel = DIFFICULTY_HARD;
                                    currentNumColumns = 4;
                                    resetState(1);
                                    break;
                                case 3:
                                    // Hard with 8 columns
                                    difficultyLevel = DIFFICULTY_HARD;
                                    currentNumColumns = 8;
                                    resetState(1);
                                    break;
                            }
                            if (endlessMode) {
                                gameState = GAME_PLAY;
                            } else {
                                dojo.SetImage(dojoImgInit);		
                                window->Clear();
                                dojo.Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
                                window->Draw(dojo);
                                window->Display();
                            }
                            
                            resetMenuSelector();
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
        !targetAltImg.LoadFromFile("target-alt.png") ||
        !heartImg.LoadFromFile("heart.png") ||
        !goalImg.LoadFromFile("goal.png") ||
        !goalAltImg.LoadFromFile("goal-alt.png") ||
        !backgroundImg.LoadFromFile("background.png") ||
        !gameOverImg.LoadFromFile("Dojo/Gameover.png") ||
		!dojoImgInit.LoadFromFile("Dojo/Dojo_Init.png") || 
        !dojoImg0.LoadFromFile("Dojo/Dojo_0.png") || 
		!dojoImg1.LoadFromFile("Dojo/Dojo_1.png") || 
		!dojoImg2.LoadFromFile("Dojo/Dojo_2.png") || 
		!dojoImg3.LoadFromFile("Dojo/Dojo_3.png") || 
		!dojoImg4.LoadFromFile("Dojo/Dojo_4.png") || 
		!dojoImg5.LoadFromFile("Dojo/Dojo_5.png") || 
		!dojoImg6.LoadFromFile("Dojo/Dojo_6.png") || 
		!dojoImg7.LoadFromFile("Dojo/Dojo_7.png") || 
		!dojoImg8.LoadFromFile("Dojo/Dojo_8.png") || 
        !starImg.LoadFromFile("star1-25.png") ||
        //!coinImg.LoadFromFile("coin.png") ||
        !menuStoryModeImg.LoadFromFile("menu-story-mode.png") ||
        !menuInstructionsImg.LoadFromFile("menu-instructions.png") ||
        !menuBeginnerImg.LoadFromFile("menu-beginner.png") ||
        !menuIntermediateImg.LoadFromFile("menu-intermediate.png") ||
        !menuAdvancedImg.LoadFromFile("menu-advanced.png") ||
        !menuAdvanced2Img.LoadFromFile("menu-advanced2.png") ||
        !menuEndlessImg.LoadFromFile("menu-endless.png")) {
        
        // This should quit or throw an exception
        printf("Error loading resources - controller\n");
        window->Close();
    }
}

void Controller::initializeObjects() {
	srand(time(NULL));

    //coin.SetImage(coinImg);
    heart.SetImage(heartImg);
	gameOver.SetImage(gameOverImg);
	dojo.SetImage(dojoImgInit);
    background.SetImage(backgroundImg);
    star.SetImage(starImg);
    menuStoryMode.SetImage(menuStoryModeImg);
    menuInstructions.SetImage(menuInstructionsImg);
    menuBeginner.SetImage(menuBeginnerImg);
    menuIntermediate.SetImage(menuIntermediateImg);
    menuAdvanced.SetImage(menuAdvancedImg);
    menuAdvanced2.SetImage(menuAdvanced2Img);
    menuEndless.SetImage(menuEndlessImg);
    menuSelector.SetImage(starImg);
    
    menuStoryMode.SetPosition(50, 30);
    menuEndless.SetPosition(50, 90);
    menuInstructions.SetPosition(50, 150);
    
    menuBeginner.SetPosition(50, 30);
    menuIntermediate.SetPosition(50, 90);
    menuAdvanced.SetPosition(50, 150);
    menuAdvanced2.SetPosition(50, 210);
    
    menuSelector.SetPosition(10, 45);
    
    //coin.SetPosition(WINDOW_WIDTH/2 + 20, WINDOW_HEIGHT - 45);
	//coin.Scale(.15,.15);

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

    // Add Idle animation
    idleAnimation.init("Actions/Idle/Idle", NUM_IDLE_FRAMES);
    
    // Add transition animations
    climbAnimation.init("Transitions/Climb/Climb", NUM_CLIMB_FRAMES);
    enterDojoAnimation.init("Transitions/Enter_Dojo/Enter_Dojo", NUM_ENTER_DOJO_FRAMES);

    // setup SoundManager
    #ifdef USE_SOUND
    soundManager = new SoundManager();
    #endif
}

void Controller::addRandomSet() {
	//int numTargets = MIN_NUM_TARGETS + rand() % (MAX_NUM_TARGETS - MIN_NUM_TARGETS);
    //int speed = TARGET_SPEED * level * SPEED_RATIO;
	int numTargets = MIN_NUM_TARGETS + level + rand() % (2*level);
    if(numTargets > MAX_NUM_TARGETS) {
        numTargets = MAX_NUM_TARGETS;
    }
    int speed = SPEED_START + ((difficultyLevel - 1) * 200) + (level * SPEED_INCREASE);

    bool havePowerup = false;

    if (rand() % 100 < POWERUP_THRESHOLD) {
        int powerup = rand() % NUM_POWERUPS;
        havePowerup = true;
        powerupTargetCount = 0;
    }

	targetSets.push_back(TargetSet());
    int y = 0;
	for(int i = 0; i < numTargets; i++) {
        int col = rand() % currentNumColumns;

        if (havePowerup && rand() % 100 < POWERUP_TARGET_THRESHOLD) {
    		targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, &targetAltImg, speed, col, y, true));
            powerupTargetCount++;
        } else {
		    targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, &targetAltImg, speed, col, y));
        }

        // add doubles, triples, quadruples
        /*
        if(level > 2 && rand() % 10 < 2) {
            targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, speed, (col+1) % currentNumColumns, y));

            if(level > 4 && rand() % 10 < 5) {
                targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, speed, (col+2) % currentNumColumns, y));

                if(level > 6 && rand() % 10 < 5) {
                    targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, speed, (col+3) % currentNumColumns, y));
                }
            }
        }
        */

        y += COLUMN_WIDTH;
	}

    if (powerupTargetCount > 0) {
        powerupSet = true;
    }
}

void Controller::randomizeGoals() {
    goals.clear();

    int pos = 250 + rand() % 250;

    for(int i = 0; i < currentNumColumns; i++) {
        goals.push_back(Goal(&goalImg, &goalAltImg, pos, i));
    }
}

void Controller::displayText(std::string message, int x, int y, sf::RenderTarget* target, int size, sf::Color color, bool bold) {
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
    enemy->setHealth(BASIC_ENEMY_HEALTH);

    // Reset goals
    goals.clear();
    for (int i = 0; i < currentNumColumns; i++) {
        goals.push_back(Goal(&goalImg, &goalAltImg, 528, i));
    }

    gameState = GAME_WAIT_FOR_INPUT;

    // reset powerups
    for (int i = 0; i < NUM_POWERUPS; i++) {
        powerupCounter[i] = 0;
    }
    powerupTargetCount = 0;
    powerupSet = false;
    
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

/*
void Controller::loadRandomCombos(int num) {
    // delete any existing combos
    for (int i = 0; i < comboActions.size(); i++) {
        if (comboActions.at(i)) { delete comboActions[i]; }
    }
    comboDescriptions.clear();

    // create new combos
    for (int i = 0; i < num; i++) {
        comboActions.push_back(new ComboAction());
        comboDescriptions.push_back("");
    }

    // make sure we don't load the same combo twice
    bool added[NUM_COMBO_ATTACKS];
    for (int i = 0; i < NUM_COMBO_ATTACKS; i++) {
        added[i] = false;
    }

    // randomly add the combos
    for (int i = 0; i < num; i++) {

        int num = rand()%NUM_COMBO_ATTACKS;

        while (added[num]) {
            num = (num+1)%NUM_COMBO_ATTACKS;
        }

        printf("\tnum: %d\n", num);
        added[num] = true;

        switch(num) {
        case 0:
            comboActions[i]->addAnimation("ComboActions/Punch_Punch_Combo/Punch_Punch_Combo", NUM_PUNCH_PUNCH_FRAMES);
            comboDescriptions[i] = "Punch-Punch";
            break;
        case 1:
            comboActions[i]->addAnimation("ComboActions/Punch_Kick_Combo/Punch_Kick_Combo", NUM_PUNCH_KICK_FRAMES);
            comboDescriptions[i] = "Punch-Kick";
            break;
        case 2:
            comboActions[i]->addAnimation("ComboActions/Punch_Headbutt_Combo/Punch_Headbutt_Combo", NUM_PUNCH_HEADBUTT_FRAMES);
            comboDescriptions[i] = "Punch-Headbutt";
            break;
        case 3:
            comboActions[i]->addAnimation("ComboActions/Kick_Punch_Combo/Kick_Punch_Combo", NUM_KICK_PUNCH_FRAMES);
            comboDescriptions[i] = "Kick-Punch";
            break;
        case 4:
            comboActions[i]->addAnimation("ComboActions/Kick_Kick_Combo/Kick_Kick_Combo", NUM_KICK_KICK_FRAMES);
            comboDescriptions[i] = "Kick-Kick";
            break;
        case 5:
            comboActions[i]->addAnimation("ComboActions/Kick_Headbutt_Combo/Kick_Headbutt_Combo", NUM_KICK_HEADBUTT_FRAMES);
            comboDescriptions[i] = "Kick-Headbutt";
            break;
        case 6:
            comboActions[i]->addAnimation("ComboActions/Headbutt_Punch_Combo/Headbutt_Punch_Combo", NUM_HEADBUTT_PUNCH_FRAMES);
            comboDescriptions[i] = "Headbutt-Punch";
            break;
        case 7:
            comboActions[i]->addAnimation("ComboActions/Headbutt_Kick_Combo/Headbutt_Kick_Combo", NUM_HEADBUTT_KICK_FRAMES);
            comboDescriptions[i] = "Headbutt-Kick";
            break;
        case 8:
            comboActions[i]->addAnimation("ComboActions/Slide_Tackle/Slide_Tackle", NUM_SLIDE_TACKLE_FRAMES);
            comboDescriptions[i] = "Slide Tackle";
            break;
        case 9:
            comboActions[i]->addAnimation("ComboActions/Cartwheel/Cartwheel", NUM_CARTWHEEL_FRAMES);
            comboDescriptions[i] = "Cartwheel";
            break;
        case 10:
            comboActions[i]->addAnimation("ComboActions/Punch_Behind_Punch/Punch_Behind_Punch", NUM_PUNCH_BEHIND_PUNCH_FRAMES);
            comboDescriptions[i] = "Punch-Behind-Punch";
            break;
        case 11:
            comboActions[i]->addAnimation("ComboActions/Jump_Kick/Jump_Kick", NUM_JUMP_KICK_FRAMES);
            comboDescriptions[i] = "Jump Kick";
            break;
        }
    }

    // unlock first combo
    comboActions[0]->unlock();
}
*/

void Controller::loadCombos(int difficulty, bool unlocked) {
     // delete any existing combos
    for (int i = 0; i < comboActions.size(); i++) {
        if (comboActions.at(i)) { delete comboActions[i]; }
    }
    comboActions.clear();
    comboDescriptions.clear();

    // create new combos
    for (int i = 0; i < NUM_COMBOS_PER_DIFFICULTY; i++) {
        comboActions.push_back(new ComboAction());
        comboDescriptions.push_back("");

        if (unlocked) {
            comboActions[i]->unlock();
        }
    }

    // load combos based on difficulty
    switch(difficulty) {
    case 0: // easy
        comboActions[0]->addAnimation("ComboActions/Punch_Punch_Combo/Punch_Punch_Combo", NUM_PUNCH_PUNCH_FRAMES);
        comboDescriptions[0] = "Punch-Punch";
        comboActions[1]->addAnimation("ComboActions/Kick_Punch_Combo/Kick_Punch_Combo", NUM_KICK_PUNCH_FRAMES);
        comboDescriptions[1] = "Kick-Punch";
        comboActions[2]->addAnimation("ComboActions/Headbutt_Kick_Combo/Headbutt_Kick_Combo", NUM_HEADBUTT_KICK_FRAMES);
        comboDescriptions[2] = "Headbutt-Kick";
        comboActions[3]->addAnimation("ComboActions/Slide_Tackle/Slide_Tackle", NUM_SLIDE_TACKLE_FRAMES);
        comboDescriptions[3] = "Slide Tackle";
        break;
    case 1: // intermediate
        comboActions[0]->addAnimation("ComboActions/Punch_Kick_Combo/Punch_Kick_Combo", NUM_PUNCH_KICK_FRAMES);
        comboDescriptions[0] = "Punch-Kick";
        comboActions[1]->addAnimation("ComboActions/Kick_Headbutt_Combo/Kick_Headbutt_Combo", NUM_KICK_HEADBUTT_FRAMES);
        comboDescriptions[1] = "Kick-Headbutt";
        comboActions[2]->addAnimation("ComboActions/Headbutt_Punch_Combo/Headbutt_Punch_Combo", NUM_HEADBUTT_PUNCH_FRAMES);
        comboDescriptions[2] = "Headbutt-Punch";
        comboActions[3]->addAnimation("ComboActions/Cartwheel/Cartwheel", NUM_CARTWHEEL_FRAMES);
        comboDescriptions[3] = "Cartwheel";
        break;
    case 2: // advanced
    case 3: // advanced +
        comboActions[0]->addAnimation("ComboActions/Punch_Headbutt_Combo/Punch_Headbutt_Combo", NUM_PUNCH_HEADBUTT_FRAMES); // PROBLEM HERE
        comboDescriptions[0] = "Punch-Headbutt";
        comboActions[1]->addAnimation("ComboActions/Kick_Kick_Combo/Kick_Kick_Combo", NUM_KICK_KICK_FRAMES);
        comboDescriptions[1] = "Kick-Kick";
        comboActions[2]->addAnimation("ComboActions/Punch_Behind_Punch/Punch_Behind_Punch", NUM_PUNCH_BEHIND_PUNCH_FRAMES);
        comboDescriptions[2] = "Punch-Behind-Punch";
        comboActions[3]->addAnimation("ComboActions/Jump_Kick/Jump_Kick", NUM_JUMP_KICK_FRAMES);
        comboDescriptions[3] = "Jump Kick";
        break;
    }
}

int Controller::getRandomComboAttack() {
    int num = 0;

    for (int i = 0; i < comboActions.size(); i++) {
        if (comboActions[i]->checkUnlocked()) {
            num++;
        }
    }

    if (num == 0) {
        return -1;
    }

    return rand() % num;
}