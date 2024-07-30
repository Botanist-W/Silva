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
		
		
	// Am I doing this right or should these be subclasses << NO, idiot 

}

void Forest::setParams() {

	searchArea = mParams.searchArea;
	bounds = mParams.fragmentSizeList[forestID]; // TODO BE USEFUL
	treeDensity = mParams.treeDensity;
	numIndiv = (bounds * bounds) * treeDensity;
	numSpecies = mParams.numSpecies;
	b1 = mParams.b1;
	b2 = mParams.b2;

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
			std::cout << "out of bounds\n";
			continue; // Restart dispersal if not in bounds 
		}
		
		searchResults = search(recPos, searchArea);

		std::cout << "parent position: " << parent.first.get<0>() << " , " << parent.first.get<1>() << "\n"; // Working
		std::cout << "recruit position: " << recPos.get<0>() << " , " << recPos.get<1>() << "\n"; // go back to construction 
		std::cout << "No. search results: " << searchResults.size() << "\n";

		if(searchResults.size() > 2){
			std::cout << "Searching\n";
		}
		else
		{
			std::cout << "error in searching, no trees around, returning random tree  \nFOREST:  " << forestID << "\n";
			tree.insert(value(point(Crand::rand_double(0, bounds), Crand::rand_double(0, bounds)), randomTree().second));
			break;
		}

		float NCI = comp->compIndex(searchResults, parent, recPos); // Main TODO: Figure out how this works 

		float pNCI = 1 / (1 + NCI);
		std::cout << "NCI:  " << NCI << "\n";
		float compare = Crand::randFloat(0, 1);

		// Main check for whether recruitment was successfull 
		if (pNCI < Crand::randFloat(0,1)) { // Can alter this later ://
			std::cout << "Yipee" << pNCI << "\n";
			tree.insert(value(recPos, parent.second));
			break;
		} 
		else
		{
			std::cout << "Oh dear" << pNCI << "\n";
			searchResults.clear();
			continue;
		}

	}

};



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


// Take a snapshot of the thing 
std::vector<std::pair<int, value>> Forest::forestCapture(int& timeStep) {

	std::vector<std::pair<int, value>> Capture;

	for (const auto& element : tree) 
		Capture.emplace_back(std::pair<int, value>(timeStep, element));
	
	return Capture;

};

// TODO : Implement
void Forest::buildFromForest(std::vector<value>& input) {

	// If the input is larger than this forest instance then it can use a random sample from the input 
	int setSize = Crand::rand_int(0, (input.size() - bounds));

	for (int i = setSize ; i < numIndiv + setSize; i++) 
		tree.insert(input[i]); // This would be interesting to see how this works :/ because of the R tree not being linear

};


