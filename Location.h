#ifndef GUARD_loc_h
#define GUARD_loc_h

#include <vector>
#include 'GameMap.h'

public:

	// create a new location
	Location();

	// create a new location with children
	Location(std::vector<Location> children);

	// toggle a location
	void toggle();

private:

	// the ID of the location
	int locationID;

	// we don't want to toggle locations after they have already been toggled
	bool toggled = false;

	// child leaves
	std::vector<Location> children;

#endif