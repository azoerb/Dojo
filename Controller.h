#pragma once

#include "Animation.h"
#include "Action.h"
#include "ComboAction.h"
#include "TargetSet.h"
#include "Goal.h"
#include "Player.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>

enum GameState {
    GAME_LOADING,
    GAME_WAIT_FOR_INPUT,
    GAME_MENU,
    GAME_PLAY,
    GAME_PAUSE,
};

enum MenuState {
    STATE_MAIN_MENU = 0,
    STATE_NEW_GAME,
    STATE_INSTRUCTIONS
};

class Controller {
private:
    sf::RenderWindow* window;
    
    sf::Image targetImg;
    sf::Image goalImg;
    sf::Image goalAltImg;
    sf::Image backgroundImg;
    sf::Image starImg;
    sf::Image loadingImg;
    sf::Image menuNewGameImg;
    sf::Image menuInstructionsImg;
    sf::Image menuBeginnerImg;
    sf::Image menuIntermediateImg;
    sf::Image menuAdvancedImg;
    
    sf::Sprite background;
    sf::Sprite star;
    sf::Sprite loading;
    sf::Sprite menuNewGame;
    sf::Sprite menuInstructions;
    sf::Sprite menuSelector;
    sf::Sprite menuBeginner;
    sf::Sprite menuIntermediate;
    sf::Sprite menuAdvanced;
    
    sf::Animation idleAnimation;
    std::vector<Action*> basicActions;
    std::vector<Action*> specialActions;
    std::vector<ComboAction*> comboActions;
    std::vector<TargetSet> targetSets;
    std::vector<Goal> goals;
    
    int level;
    //sf::Animation* currentAnimation;
    int currentAnimation;
    int currentAnimationType;

	Player* player;
	Enemy* enemy;

	int numActionFrames;
    int gameState;
    int menuState;
    int menuSelectorPosition;
    bool shouldDraw;

    bool keyPresses[NUM_COLUMNS];
    
    float elapsedTime;

    void addRandomSet();
    void randomizeGoals();
    void displayMessage(std::string message, int x, int y, sf::RenderTarget* target, int size = 50, sf::Color color = sf::Color(0,0,0));
    void resetState(int level);
    void resetMenuSelector();
    
public:
    Controller();
    ~Controller();
    
    void mainLoop();
    void update();
    void draw();
    void drawLoadScreen();
    void processEvents();
    void loadResources();
    void initializeObjects();
};
