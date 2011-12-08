#pragma once

#include "Animation.h"
#include "Action.h"
#include "ComboAction.h"
#include "TargetSet.h"
#include "Goal.h"
#include "Player.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>

enum GameStatus {
    GAME_WAIT_FOR_INPUT,
    GAME_PLAY,
    GAME_PAUSE,
};

class Controller {
private:
    sf::RenderWindow* window;
    
    sf::Image targetImg;
    sf::Image goalImg;
    sf::Image backgroundImg;
    sf::Image starImg;
    sf::Image loadingImg;
    
    sf::Sprite background;
    sf::Sprite star;
    sf::Sprite loading;
    
    sf::Animation idleAnimation;
    std::vector<Action*> basicActions;
    std::vector<ComboAction*> comboActions;
    std::vector<TargetSet> targetSets;
    std::vector<Goal> goals;
    
    int level;
    int currentAnimation;
    int currentAnimationType;

	Player* player;
	Enemy* enemy;

	int numActionFrames;
    int gameStatus;

    bool keyPresses[4];
    
    float elapsedTime;

    void addRandomSet();
    void randomizeGoals();
    void displayMessage(std::string message, int x, int y, sf::RenderTarget* target, int size = 50, sf::Color color = sf::Color(0,0,0));
    void resetState(int level);

    
public:
    Controller();
    ~Controller();
    
    void mainLoop();
    bool update();
    void draw();
    void processEvents();
    void loadResources();
    void initializeObjects();
};
