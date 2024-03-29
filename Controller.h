#pragma once

#include "Animation.h"
#include "Action.h"
#include "ComboAction.h"
#include "TargetSet.h"
#include "Goal.h"
#include "Player.h"
#include "Enemy.h"
#include "SoundManager.h"
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
    STATE_INSTRUCTIONS,
    STATE_INTRO
};

class Controller {
private:
    sf::RenderWindow* window;
    
    sf::Image dojoImgInit;
	sf::Image dojoImg0;
	sf::Image dojoImg1;
	sf::Image dojoImg2;
	sf::Image dojoImg3;
	sf::Image dojoImg4;
	sf::Image dojoImg5;
	sf::Image dojoImg6;
	sf::Image dojoImg7;
	sf::Image dojoImg8;
    
	sf::Image gameOverImg;
	sf::Sprite gameOver;
    
	sf::Image lifeImg;
	sf::Image deathImg;
    sf::Image heartImg;
    
    sf::Image targetImg;
    sf::Image targetAltImg;
    sf::Image goalImg;
    sf::Image goalAltImg;
    sf::Image backgroundImg;
    sf::Image starImg;
    sf::Image loadingImg;
    sf::Image menuStoryModeImg;
    sf::Image menuInstructionsImg;
    sf::Image menuBeginnerImg;
    sf::Image menuIntermediateImg;
    sf::Image menuAdvancedImg;
    sf::Image menuAdvanced2Img;
    sf::Image menuEndlessImg;
    sf::Image coinImg;
    
    sf::Sprite dojo;
    sf::Sprite life;
	sf::Sprite death;
    sf::Sprite heart;
	sf::Sprite coin;
    sf::Sprite background;
    sf::Sprite star;
    sf::Sprite loading;
    sf::Sprite menuStoryMode;
    sf::Sprite menuInstructions;
    sf::Sprite menuSelector;
    sf::Sprite menuBeginner;
    sf::Sprite menuIntermediate;
    sf::Sprite menuAdvanced;
    sf::Sprite menuAdvanced2;
    sf::Sprite menuEndless;
    
    sf::Animation idleAnimation;
    sf::Animation climbAnimation;
    sf::Animation enterDojoAnimation;
    
    std::vector<Action*> basicActions;
    std::vector<ComboAction*> comboActions;
    std::vector<TargetSet> targetSets;
    std::vector<Goal> goals;

    std::vector<std::string> comboDescriptions;

    sf::String text;
    
    int level;
    float difficultyLevel;
    int dojoLevel;
	int lives;
    int points;
    int hitCounter;
    //sf::Animation* currentAnimation;
    int currentAnimation;
    int currentAnimationType;

	bool criticalAttack;
    bool comboAttack;
	int criticalFrame;
    
    bool goToMenu;
    bool doCombo;

	Player* player;
	Enemy* enemy;

    #ifdef USE_SOUND
    SoundManager* soundManager;
    #endif

	int numActionFrames;
    int gameState;
    int menuState;
    int menuSelectorPosition;
    bool shouldDraw;
    
    bool endlessMode;

    bool keyPresses[NUM_COLUMNS];

    int powerupCounter[NUM_POWERUPS];
    bool powerupSet;
    int powerupTargetCount;

    int currentNumColumns;
    
    float elapsedTime;

    void addRandomSet();
    void randomizeGoals();
    void displayText(std::string message, int x, int y, sf::RenderTarget* target, int size = 50, sf::Color color = sf::Color(0,0,0), bool bold = false);
    void resetState(int level);
    void resetMenuSelector();
    void loadRandomCombos(int num = 5);
    void loadCombos(int difficulty, bool unlocked = false);
    int getRandomComboAttack();
    
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
