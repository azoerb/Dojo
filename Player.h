#pragma once

#include "Character.h"
#include "Action.h"

class Player: public Character {
private:
    std::vector<Action> possibleActions;
    
public:
    Player(float health);
    std::vector<Action> getPossibleActions();
    void drawHealthBar(sf::Sprite* star, sf::RenderTarget* target);
};
