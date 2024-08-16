#pragma once

#ifndef FOREST
#define FOREST

#include "settings.h"
#include "RTREE.h"
#include "Dispersal.h"
#include "Log.h"
#include "Competition.h"
#include <memory>
#include <iostream>
#include "spCount.h"

class speciesCount;

// TODO: just merge the rtree and the forest class

// MAIN TODO: Make a base Forest class with subclasses of this type of forest and then a metacommunity. This will allow meta communities to be linked into the simulation 


class Forest : public rTree {
public:

	const int forestID; // Just to keep track of the ID of the thing 

	Forest(const params& _params, const int& _ID) : rTree(_params), forestID(_ID) { 
		LOG_DEBUG("Created forest: {}", forestID);
		//searchResults.reserve(Pi*(searchArea*searchArea)*treeDensity*2); // Reserving twice the average expected number of trees returned in each search to prevent reallocation
		setParams();
		setMethods();

		
	}

	~Forest() {
		LOG_DEBUG("Forest {} destroyed", forestID);
	}

	void localStep(); // Run a step of dispersal and competition, MAKE IT FAST ( Immigration is done first in the simulation class )

	void localExtinction(int&, std::vector<indiv>& spLib); // takes species to remove from the forest and then replaces it with a bunch of random trees :) << WILL BE VERY INTENSIVE

	void buildFromForest(std::vector<value>&);

	void initCounter();
	
	std::vector<observation> getCapture(int repeat, int timeStep); 

	void counter(int repeat, int timeStep, bool active);

	std::vector<std::tuple<int, int, int, int>> getSpCount();

private:

	void setMethods(); // Gets the correct dispersal and competition based on settings

	void setParams();

	

	std::unique_ptr<dispersal> disp;
	std::unique_ptr<competition> comp;

	std::unique_ptr<speciesCount> mCounter;

	// These search results are constantly used so keep on stack or something, idk
	std::vector<value> searchResults;

	friend class metaForest;
};



// You can only make this work if you actually intergrate the R tree and the forest class then make a base class for these two. Then from this you still have a lot of work to do.
// TODO: do this ... 

class metaForest : public Forest {
public:
	metaForest(const params& _params, const int& _ID) : Forest(_params, _ID);


	







};




// Why am I doing this << are u okay?

#endif // !FOREST



