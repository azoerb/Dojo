#include "Controller.h"
#include <math.h>
#include <SFML/Audio.hpp>

Controller::Controller() {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dojo Hero");
    window->UseVerticalSync(true);
    
    player = new Player(PLAYER_HEALTH);
    enemy = new Enemy(BASIC_ENEMY_HEALTH);

	criticalAttack = false;
	criticalFrame = 0;
    currentNumColumns = 4;
    
    goToMenu = false;
    
    gameState = GAME_MENU;
    menuState = STATE_MAIN_MENU;

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
    
    for (int i = 0; i < specialActions.size(); i++) {
        if (specialActions.at(i)) { delete specialActions[i]; }
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
                if (player->getHealth() <= 0) {
                    lives--;
					//check to see if you have 3 lives left..
					if (lives > 0) {
						//we have two options, restart the level, or just continue - like I am.
						player->setHealth(PLAYER_HEALTH);
						//now display menu?
					} else {	
						lives = 3;
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
					if(1) { 
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
					} else if(0) { //endless mode
						//go to next level
						resetState(level+1);
						window->Display();
					}
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
                        }
                    } else {
                        found[col] = true;
                        targetSets[i].changeAccuracy(result);
                        targetSets[i].removeTarget(j);
                        hit = true;

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
                }
            }

            // Once all of the targetSet's targets are gone
            if (targets->size() == 0) {
                // Determine accuracy and play animation
                float accuracy = targetSets[i].getAccuracy();

                currentAnimation = rand() % basicActions.size();
                                
                if (accuracy < BLOCK_BOUND + ((difficultyLevel - 1) * (100 - BLOCK_BOUND))) {
                    currentAnimationType = ANIMATION_COUNTER;
                } else if (accuracy < HIT_BOUND + ((difficultyLevel - 1) * (100 - HIT_BOUND))) {
                    currentAnimationType = ANIMATION_BLOCK;
                } else {
                    currentAnimationType = ANIMATION_HIT;

					if (accuracy > CRITICAL_BOUND + ((difficultyLevel - 1) * (100 - CRITICAL_BOUND))) {
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
            for(int i = 0; i < currentNumColumns; i++) {
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
            window->Draw(coin);
			std::stringstream type; 
			type << points; 
            
            displayText(type.str(), WINDOW_WIDTH/2, WINDOW_HEIGHT - 50, window, 20, sf::Color(255,255,255));
            
            //draw lives accordingly 
			switch(lives) {
                case 3:
                    life.SetPosition(WINDOW_WIDTH/2 - 35, WINDOW_HEIGHT - 90);
                    window->Draw(life);
                    life.SetPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - 90);
                    window->Draw(life);
                    life.SetPosition(WINDOW_WIDTH/2 + 35, WINDOW_HEIGHT - 90);
                    window->Draw(life);
                    break;
                case 2:
                    life.SetPosition(WINDOW_WIDTH/2 - 35, WINDOW_HEIGHT - 90);
                    window->Draw(life);
                    life.SetPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - 90);
                    window->Draw(life);
                    death.SetPosition(WINDOW_WIDTH/2 + 35, WINDOW_HEIGHT - 90);
                    window->Draw(death);
                    break;
                case 1:
                    life.SetPosition(WINDOW_WIDTH/2 - 35, WINDOW_HEIGHT - 90);
                    window->Draw(life);
                    death.SetPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - 90);
                    window->Draw(death);
                    death.SetPosition(WINDOW_WIDTH/2 + 35, WINDOW_HEIGHT - 90);
                    window->Draw(death);
                    break;
                case 0:
                    death.SetPosition(WINDOW_WIDTH/2 - 35, WINDOW_HEIGHT - 90);
                    window->Draw(death);
                    death.SetPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - 90);
                    window->Draw(death);
                    death.SetPosition(WINDOW_WIDTH/2 + 35, WINDOW_HEIGHT - 90);
                    window->Draw(death);
                    break;
            }
            
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
                    if (gameState == GAME_WAIT_FOR_INPUT) {
                        if (goToMenu) {
                            gameState = GAME_MENU;
                            goToMenu = false;
                        } else {
                            gameState = GAME_PLAY;
                        }
                    } else if (gameState == GAME_MENU) {
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
                            menuState = STATE_MAIN_MENU;
                            
                            dojo.SetImage(dojoImgInit);		
							window->Clear();
							dojo.Resize(WINDOW_WIDTH,WINDOW_HEIGHT);
							window->Draw(dojo);
							window->Display();
                            
                            switch (menuSelectorPosition) {
                                case 0:
                                    // Easy
                                    difficultyLevel = 1.0;
                                    resetState(1);
                                    break;
                                case 1:
                                    // Medium
                                    difficultyLevel = DIFFICULTY_MEDIUM;
                                    resetState(1);
                                    break;
                                case 2:
                                    // Hard
                                    difficultyLevel = DIFFICULTY_HARD;
                                    resetState(1);
                                    break;
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
        !lifeImg.LoadFromFile("Life.png") ||
		!deathImg.LoadFromFile("Death.png") || 
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
        !coinImg.LoadFromFile("coin.png") ||
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

    coin.SetImage(coinImg);
	life.SetImage(lifeImg);
	death.SetImage(deathImg); 
	gameOver.SetImage(gameOverImg);
	dojo.SetImage(dojoImgInit);
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
    
    coin.SetPosition(WINDOW_WIDTH/2 - 40, WINDOW_HEIGHT - 55);
	coin.Scale(.15,.15);

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

    /*/ Maybe load combos when they are purchased instead of now to speed up loading?
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
     
    ComboAction* slideTackle = new ComboAction();
    slideTackle->addAnimation("ComboActions/Slide_Tackle/Slide_Tackle", NUM_SLIDE_TACKLE_FRAMES);
    comboActions.push_back(slideTackle);
    
    ComboAction* cartwheel = new ComboAction();
    cartwheel->addAnimation("ComboActions/Cartwheel/Cartwheel", NUM_CARTWHEEL_FRAMES);
    comboActions.push_back(cartwheel);
    
    ComboAction* punchBehindPunch = new ComboAction();
    punchBehindPunch->addAnimation("ComboActions/Punch_Behind_Punch/Punch_Behind_Punch", NUM_PUNCH_BEHIND_PUNCH_FRAMES);
    comboActions.push_back(punchBehindPunch);
    
    ComboAction* jumpKick = new ComboAction();
    jumpKick->addAnimation("ComboActions/Jump_Kick/Jump_Kick", NUM_JUMP_KICK_FRAMES);
    comboActions.push_back(jumpKick);
*/
    
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

	targetSets.push_back(TargetSet());
    int y = 0;
	for(int i = 0; i < numTargets; i++) {
        int col = rand() % currentNumColumns;
		targetSets[targetSets.size() - 1].addTarget(Target(&targetImg, speed, col, y));

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
}

void Controller::randomizeGoals() {
    goals.clear();

    int pos = 250 + rand() % 250;

    for(int i = 0; i < currentNumColumns; i++) {
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
    enemy->setHealth(BASIC_ENEMY_HEALTH);

    // Reset goals
    goals.clear();
    for (int i = 0; i < currentNumColumns; i++) {
        goals.push_back(Goal(&goalImg, &goalAltImg, 528, i));
    }

    gameState = GAME_WAIT_FOR_INPUT;
    
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