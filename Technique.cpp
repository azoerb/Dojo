#include "Technique.h"

Technique::Technique(int id) {
	name = "Default";
	cost = 100;
	level = BEGINNER;
	unlocked = false;
	id = id;
}

Technique::Technique(char* nam, int cos, int lev, bool unl, int id) {
	name = nam;
	cost = cos;
	level = lev;
	unlocked = unl;
	id = id;
}

// destructor
Technique::~Technique() {
}

int Technique::getId() {
	return id;
}

int Technique::getLevel() {
	return level;
}

// unlock the technique, returns the cost of the technique
int Technique::unlock() {
	unlocked = true;
	return cost;
}

bool Technique::isUnlocked() {
	return unlocked;
}

// how much does it cost?
int Technique::getCost() {
	return cost;
}

// what is the name of the technique?
char * Technique::getName() {
	return name;
}