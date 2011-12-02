#include "Enemy.h"

Enemy::Enemy(float health, bool attack) : Character(health) {
    this->attack = attack;
}

bool Enemy::canAttack() {
    return attack;
}