#include "Player.h"

Player::Player(float health) : Character(health) {
    //
}

std::vector<Action> Player::getPossibleActions() {
    return possibleActions;
}