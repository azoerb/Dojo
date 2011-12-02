#ifndef GUARD_gameMap_h
#define GUARD_gameMap_h

#include <vector>
#include "Location.h"

class GameMap {
private:

	// we'll want a vector of locations, conceptually that's what our map is
    std::vector<Location> places;
    
public:

	// create a new gamemap
    GameMap();
    
	// returns a pointer to the locations on the 
    std::vector<Location>* getLocations();
    
	// this will toggle an event, locations will create more locations
	// a location can only be toggled once, and based on the structure of the
	// tree, it creates its own children, more about that later
    int toggleEvent(int locationID);

	// need to be able to add locations
	void addLocation(Location loc);

	// 

private:

	// vector of locations to represent game map
	std::vector<Location> locations;
};

#endif