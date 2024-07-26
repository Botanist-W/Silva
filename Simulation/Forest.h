#pragma once

#ifndef FOREST
#define FOREST

#include "settings.h"
#include "RTREE.h"
#include "Dispersal.h"
#include "Competition.h"
#include <memory>
#include <iostream>



// Might be a bit of a obsolete method because of how much I am offloading to the simulation class :/
/* Justification:
*	I want the dispersal and competition classes to have access to the local settings of the forest because I may want to alter the properteies of the forests
*  for example, I may want forest to have a different sized which would just mean I would have to pass more and more thingymagigs to the functions if done in the simulation class
*  An alternative is to provide a settings struct but this would be clunky maybe
*/
class Forest : public rTree {
public:

	int& forestID; // Just to keep track of the ID of the thing 

	Forest(params& _params, int& _ID) : rTree(_params), forestID(_ID) {
		std::cout << "Created forest: " << forestID << "\n";
		searchResults.reserve(Pi*(searchArea*searchArea)*treeDensity*2); // Reserving twice the average expected number of trees returned in each search to prevent reallocation
		setMethods();
		setParams();
	}

	void localStep(); // Run a step of dispersal and competition, MAKE IT FAST ( Immigration is done first in the simulation class )

	void localExtinction(int&, std::vector<indiv>& spLib); // takes species to remove from the forest and then replaces it with a bunch of random trees :) << WILL BE VERY INTENSIVE

	std::vector<std::pair<int, value>> forestCapture(int&); // Takes in a timstep

	void buildFromForest(std::vector<value>&);

private:

	void setMethods(); // Gets the correct dispersal and competition based on settings

	void setParams();

	std::unique_ptr<dispersal> disp;
	std::unique_ptr<competition> comp;
	std::vector<value> searchResults;

};



// Why am I doing this << are u okay?

#endif // !FOREST



