// Parent object of Player and Enemy

#pragma once

#include <SFML/Graphics.hpp>

class Character {
protected:
    float health;

public:
    Character(float health) {
        this->health = health;
    }

	int getHealth() {
		return health;
	}

	void setHealth(int health) {
		this->health = health;
	}
};