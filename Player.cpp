#include "Player.h"

Player::Player(float health) : Character(health) {
    //
}

std::vector<Action> Player::getPossibleActions() {
    return possibleActions;
}

void Player::drawHealthBar(sf::Sprite* star, sf::RenderTarget* target) {
    for(int i = 0; 10*i < this->health && i < 10; i++) {
        star->SetPosition(75 + 30*i, 30);
        target->Draw(*star);
    }

	for(int i = 10; 10*i < this->health && i < 20; i++) {
        star->SetPosition(75 + 30*(i-10), 60);
        target->Draw(*star);
    }
}