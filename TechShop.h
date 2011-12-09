#ifndef SHOP_tech_h
#define SHOP_tech_h

#include <SFML/Graphics.hpp>
#include "Technique.h"
#include "Controller.h"
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

	// images that the shop will wind up using that we only want to load once
	sf::Image tab;
	sf::Image bg1;
	sf::Image bg2;
	sf::Image bg3;

public:

	TechShop();

	TechShop(std::vector<Technique> listOfTechs);

	~TechShop();

	void setSprite() {

	}

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
	void drawShop(sf::RenderWindow* window);

	// displays current level
	void switchScreen(int level);
};

#endif