#include "Forest.h"


void Forest::setMethods() {

	// Setting up which tpes of interactions should be used -->> This could be expanded for more options easily (***hopefully)
	if (mParams.fragmented == false)
		disp = std::make_unique <continuousDisp> (*this);
	else
		disp = std::make_unique <fragmentedDisp> (*this);
	

	if (mParams.CNDD == mParams.HNDD) // The neutral thing is a little obsolete but there is a little performance boost I guess 
		comp = std::make_unique<neutralComp>(*this);
	else
		comp = std::make_unique<cnddComp>(*this);
		
		
	// Am I doing this right or should these be subclasses << NO, idiot << what?

}

void Forest::setParams() { // TODO: stop doing this 

	searchArea = mParams.searchArea;
	bounds = mParams.fragmentSizeList[forestID]; 
	treeDensity = mParams.treeDensity;
	numIndiv = (bounds * bounds) * treeDensity;
	numSpecies = mParams.numSpecies;
	b1 = mParams.b1;
	b2 = mParams.b2;

	LOG_TRACE("search area {}", searchArea);
	LOG_TRACE("bounds {}", bounds);
	LOG_TRACE("number of indiv {}", numIndiv);

}


// Step after immigration has been decided in the 
void Forest::localStep() {

	// Remove a random tree
	tree.remove(randomTree()); // Where should this go << who really cares?

	searchResults.clear(); // just being nice and safe 

	while (true) { // Successful competition?

		// Get random parent
		value parent = randomTree();

		// Disperse
		point recPos = disp->disperse(parent); // TODO : Work out how to deal with fragmented dispersal :/ <<  when dispersal occurs outside of a space :???
		// Maybe offload the parent to the disperse method? 
		// OR
		// OR
		// I could be lazy :)
		if (disp->inBounds(recPos) == false) {
			LOG_TRACE ("Dispersal out of bounds");
			continue; // Restart dispersal if not in bounds 
		}
		
		searchResults = search(recPos, searchArea);

		LOG_TRACE("Parent position: ({}, {})", parent.first.get<0>(), parent.first.get<1>());
		LOG_TRACE("Recruit position: ({}, {})", recPos.get<0>(), recPos.get<1>());
		LOG_TRACE("Number of search results: {}", searchResults.size());

		float NCI = 0;

		if(searchResults.size() < 2){	
			LOG_TRACE("No trees in surrounding area, assured recruitment");
		}
		else {
			NCI = comp->compIndex(searchResults, parent, recPos); // Main TODO: Figure out how this works 
		}

		float pNCI = 1 / (1 + NCI);

		LOG_TRACE ("Probability of success: {} ", pNCI);

		// Main check for whether recruitment was successfull 
		if (pNCI > Crand::randFloat(0,1)) { // Can alter this later ://
			LOG_TRACE("Recruitment SUCCESS");
			tree.insert(value(recPos, parent.second));
			break;
		} 
		else
		{
			LOG_TRACE("Recruitment FAIL");
			searchResults.clear();
			continue;
		}

	}

};


// TODO: Go back in time and be more prepared for your deadline 
void Forest::localExtinction(int& extSp, std::vector<indiv>& spLib) {

	for (const auto& element : tree) {

		if (element.second.species == extSp) {
			//This bout to be hella inefficient but I litterally dont know how else to do it without being actually smart
			tree.remove(element);
			int newSp;
			indiv newID;

			while (true) {
				newSp = Crand::rand_int(1, numSpecies ); // Correct one off error?? 
				if (newSp == extSp) {
					continue;
				}
				else
				{
					newID = spLib[newSp];
					break;
				}
			}

			tree.insert(value(point(Crand::randFloat(0, bounds), Crand::randFloat(0, bounds)), newID)); //replacing with some random indiv in the 

		}


	}

	
};



std::vector<observation> Forest::getCapture(int repeat, int timeStep) {
	
	std::vector<observation> capture;
	capture.reserve(tree.size());

	for (const auto& element : tree)
		// Repeat, Forest, timestep, ID, Sp, x, y
		capture.emplace_back(repeat, forestID, timeStep, int(element.second.uniqueID), int(element.second.species), float(element.first.get<0>()), float(element.first.get<1>())); // I hope this ends up emplacing in the right place

	LOG_DEBUG("Size of forest {} captures: {}", forestID, capture.size());

	return capture;

};




// TODO : Implement
void Forest::buildFromForest(std::vector<value>& input) {

	// If the input is larger than this forest instance then it can use a random sample from the input 
	int setSize = Crand::rand_int(0, int(input.size() - bounds));

	for (int i = setSize; i < numIndiv + setSize; i++)
		tree.insert(input[i]); // This would be interesting to see how this works :/ because of the R tree not being linear

};


