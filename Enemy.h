#pragma once

#include "Character.h"

class Enemy: public Character {
private:
    bool attack;
    
public:
    Enemy(float health, bool attack = false);
    bool canAttack();
    void drawHealthBar(sf::Sprite* star, sf::RenderTarget* target);
};
