// Parent object of Player and Enemy

#pragma once

class Character {
protected:
    float health;
public:
    Character(float health) {
        this->health = health;
    }
};