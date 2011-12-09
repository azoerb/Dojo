#ifndef SHOP_tech_h
#define SHOP_tech_h

#include <SFML/Graphics.hpp>
#include "Technique.h"
#include <list>
using namespace std;

class TechShop {
private:

	// the list of techniques in the game
	std::vector<Technique> techs;

	// for the draw method, this may be a little inefficient
	// but i'm tired right now and i suck at programming
	std::vector<Technique> currTechs;

	// the current type being shown
	int currType;

	// the amount of money the player has
	// i guess the shop is a bank?
	int cash;

public:

	TechShop();

	TechShop(std::vector<Technique> listOfTechs);

	~TechShop();

	// add the amount of cash desired 
	void addCash(int amount);

	// unlock a tech and subtract its cost from total amount
	// throw an error if you have insufficient funds
	void unlockTech(int id);

	// adds a tech
	void addTech(Technique tech);

	// returns techs based on the level given
	std::vector<Technique> getPage(int level);

	// draws the shop on the screen
	void drawShop();

	// displays current level
	void switchScreen(int level);
};

#endif