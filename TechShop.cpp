#include "TechShop.h"

TechShop::TechShop() {
	cash = 100;
	tab.LoadFromFile("Shop/tab.png");
	bg1.LoadFromFile("Shop/bg1.png");
	bg2.LoadFromFile("Shop/bg2.png");
	bg3.LoadFromFile("Shop/bg3.png");
}

TechShop::TechShop(std::vector<Technique> listOfTechs) {
	techs = listOfTechs;
	cash = 100;
	tab.LoadFromFile("Shop/tab.png");
	bg1.LoadFromFile("Shop/bg1.png");
	bg2.LoadFromFile("Shop/bg2.png");
	bg3.LoadFromFile("Shop/bg3.png");
}

TechShop::~TechShop() {
	delete &currTechs;
	delete &techs;
}

// add the amount of cash desired 
void TechShop::addCash(int amount) {
	cash += amount;
}

// unlock a tech and subtract its cost from total amount
// throw an error if you have insufficient funds
void TechShop::unlockTech(int id) {
	for(std::vector<Technique>::iterator it = techs.begin(); it != techs.end(); ++it) {
		Technique temp = *it;
		if (temp.getId() == id) {
			if (cash - temp.getCost() >= 0) {
				cash -= temp.unlock();
			}
		}
	}
}

void TechShop::addTech(Technique tech) {

	if (techs.size() == 0) {
		techs.reserve(21);
		techs[0] = tech;
	}

	for(int i = 0; i < techs.size(); i++) {
		// check the cost
		if (tech.getCost() < techs[i].getCost()) {
			// if the cost is less, put it in the current element's
			// spot and then push the elements back
			for (int j = techs.size() - 1; j > i; j--) {
				techs[j] = techs[j-1];
			}
			techs[i] = tech;
		}
	}
}

// returns techs based on the level given
std::vector<Technique> TechShop::getPage(int level) {
	std::vector<Technique> oneList;

	for(std::vector<Technique>::iterator it = techs.begin(); it != techs.end(); ++it) {
		Technique temp = *it;
		if (temp.getLevel() == level) {
			oneList.push_back(temp);
		}
	}

	return oneList;
}

// draws the shop on the screen
void TechShop::drawShop(sf::RenderWindow* window) {
	if (currType == BEGINNER) {
		// draw the tab
		sf::Sprite sp_tab;
		sp_tab.SetImage(tab);
		sp_tab.SetPosition(0.0, 0.0);
		window->Draw(sp_tab);

		// draw the correct background
		sf::Sprite sp_bg;
		sp_tab.SetImage(bg1);
		sp_tab.SetPosition(0.0, 70.0);
		window->Draw(sp_bg);

		// overlay the techniques onto the background


	} else if (currType == INTERMEDIATE) {
		// draw the tab
		sf::Sprite sp_tab;
		sp_tab.SetImage(tab);
		sp_tab.SetPosition(0.0, 0.0);
		window->Draw(sp_tab);

		// draw the correct background
		sf::Sprite sp_bg;
		sp_tab.SetImage(bg2);
		sp_tab.SetPosition(0.0, 70.0);
		window->Draw(sp_bg);

		// overlay the techniques onto the background


	} else if (currType == ADVANCED) {
		// draw the tab
		sf::Sprite sp_tab;
		sp_tab.SetImage(tab);
		sp_tab.SetPosition(0.0, 0.0);
		window->Draw(sp_tab);

		// draw the correct background
		sf::Sprite sp_bg;
		sp_tab.SetImage(bg3);
		sp_tab.SetPosition(0.0, 70.0);
		window->Draw(sp_bg);

		// overlay the techniques onto the background

	}
}

// switches the shop screen
void TechShop::switchScreen(int type) {
	currTechs = TechShop::getPage(type);
}