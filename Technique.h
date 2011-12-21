#pragma once

#define BEGINNER 1
#define INTERMEDIATE 2
#define ADVANCED 3

class Technique {

/*
 * Fields related to the basic properties a technique will have
 * name - the name of the technique
 * cost - how much the technique costs in the shop
 * level - beginner = 1, intermediate = 2, advanced = 3 (beginner by default)
 * unlocked - boolean about whether or not technique is unlocked
 */
private:
	char* name;
	int id;
	int cost;
	int level;
	bool unlocked;
public:
	// default constructor
	Technique(int id);
	// constructor
	Technique(char* nam, int cos, int lev, bool unl, int id);
	// destructor
	~Technique();

	/* Methods */

	// get the id
	int getId();

	// get the level
	int getLevel();
	
	// unlock the technique, returns the cost of the technique
	int unlock();

	// is it unlocked?
	bool isUnlocked();

	// how much does it cost?
	int getCost();

	// what is the name of the technique?
	char * getName();

};
