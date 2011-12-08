#include "Player.h"

Player::Player(float health) : Character(health) {
    //
}

std::vector<Action> Player::getPossibleActions() {
    return possibleActions;
}

void Player::drawHealthBar(sf::Sprite* star, sf::RenderTarget* target) {
    int i;
    for(i = 0; 10*i < this->health; i++) {
        star->SetPosition(75 + 30*i, 60);
        target->Draw(*star);
    }
}