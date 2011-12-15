#include "Enemy.h"

Enemy::Enemy(float health, bool attack) : Character(health) {
    this->attack = attack;
}

bool Enemy::canAttack() {
    return attack;
}

void Enemy::drawHealthBar(sf::Sprite* star, sf::RenderTarget* target) {
    for(int i = 0; 10*i < this->health && i < 10; i++) {
        star->SetPosition(725 - 30*(i+1), 60);
        target->Draw(*star);
    }

	/*for(int i = 10; 10*i < this->health && i < 20; i++) {
        star->SetPosition(725 - 30*(i-9), 60);
        target->Draw(*star);
    }*/
}