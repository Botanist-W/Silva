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
	timeSteps = mParams.timeSteps;

	LOG_TRACE("search area {}", searchArea);
	LOG_TRACE("bounds {}", bounds);
	LOG_TRACE("number of indiv {}", numIndiv);

}

void Forest::initCounter() {
	mCounter = std::make_unique<speciesCount>(*this);
};


void Forest::localStep() {

	// Remove a random tree
	value rmTree = randomTree();

	removeTree(rmTree); // Where should this go << who really cares?

	searchResults.clear(); // just being nice and safe 
	int attempt = 0;

	while (true) { // Successful competition?
		attempt++;

		
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

		//LOG_TRACE("Parent position: ({}, {})", parent.first.get<0>(), parent.first.get<1>());
		LOG_TRACE("Recruit position: ({}, {})", recPos.get<0>(), recPos.get<1>());
		LOG_TRACE("Number of search results: {}", searchResults.size());

		double NCI = 0;

		if(searchResults.size() < 2){	
			LOG_TRACE("No trees in surrounding area, assured recruitment");
		}
		else {
			NCI = comp->compIndex(searchResults, parent, recPos); // Main TODO: Figure out how this works 
		}

		double pNCI = 1 / (1 + NCI);

		if (attempt >= 100) {
			LOG_ERROR("MAX ATTEMPTS REACHED, pNCI: {}, Species: {}", pNCI, parent.second.species);

			LOG_INFO("size of R tree: {}", tree.size());

			LOG_INFO("Parent position: {}, {}", parent.first.get<0>(), parent.first.get<1>());
			addTree(randomTree());
			break;
		}

		LOG_TRACE ("Probability of success: {} ", pNCI);

		if (tree.empty())
			LOG_ERROR("NO Trees!!!!");

		// Main check for whether recruitment was successfull 
		if (pNCI > Crand::rand_double(0,1)) { // Can alter this later ://
			LOG_TRACE("Recruitment SUCCESS");
			addTree(value(recPos, parent.second));
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
			removeTree(element);
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

			tree.insert(value(point(Crand::rand_double(0, bounds), Crand::rand_double(0, bounds)), newID)); //replacing with some random indiv in the 

		}


	}

	
};



std::vector<observation> Forest::getCapture(int repeat, int timeStep) {
	
	std::vector<observation> capture;
	capture.reserve(tree.size());

	for (const auto& element : tree)
		// Repeat, Forest, timestep, ID, Sp, x, y
		capture.emplace_back(repeat, forestID, timeStep, int(element.second.uniqueID), int(element.second.species), double(element.first.get<0>()), double(element.first.get<1>())); // I hope this ends up emplacing in the right place

	LOG_DEBUG("Size of forest {} captures: {}", forestID, capture.size());

	return capture;

};




// TODO : Implement
void Forest::buildFromForest(std::vector<value>& input) {

	// Populate an rtree with the sample data
	RTREE sampleTree;
	for (auto& element : input)
		sampleTree.insert(element);
	// Find the bounds of the sample forest
	box sampleBox = sampleTree.bounds(); // returns the box that can contain all the elements in the sample Tree 
	double maxX = sampleBox.max_corner().get<0>(); 
	double maxY = sampleBox.max_corner().get<1>();

	double xOffset; // Declare an offset of where to sample
	double yOffset;

	try {
		xOffset = Crand::rand_double(0, maxX - bounds);
		yOffset = Crand::rand_double(0, maxY - bounds);
	}
	catch (...){
		LOG_ERROR("input size too big to build from sample");
	}

	// Find a box within the sample which to sample from 
	box boundingBox(point(xOffset, yOffset), point(xOffset + bounds, yOffset + bounds)); // A box randomly chosen from the larger box within the r tree 

	std::vector<value> sampleResults;

	sampleTree.query(
		bgi::within(boundingBox), // Stating the logic of the funtion
		std::back_inserter(sampleResults)); // Adding true values to the results vector 

	
	for (auto& element : sampleResults) {
		// Alter the samples x and y coords to match this forest
		element.first.set<0>(element.first.get<0>() - xOffset);
		element.first.set<1>(element.first.get<1>() - yOffset);

		tree.insert(element); // Fixed that 
	}


	// Check 
	box boundCheck = tree.bounds();
	point minCorner = boundCheck.min_corner();
	point maxCorner = boundCheck.max_corner();

	LOG_DEBUG("Bounding area: x{} , x{}, y{}, y{}", minCorner.get<0>(), maxCorner.get<0>(), minCorner.get<1>(), maxCorner.get<1>());

	// Not always even distributiuon, so just an easy workaround 

	if (sampleResults.size() != numIndiv) {
		LOG_WARN("Non equal population sizes: sample results - {} Desired size - {} ", sampleResults.size(), numIndiv);
		while (tree.size() > numIndiv) {
			tree.remove(randomTree());
			LOG_TRACE("Removing tree");
		}
		while (tree.size() < numIndiv) {
			tree.insert(randomTree());
			LOG_TRACE("Adding tree");
		}
	}

};


void Forest::counter(int repeat, int timeStep) {

	mCounter->countMod(removedSp, addedSp, timeStep, forestID, repeat);

}

std::vector<std::tuple<int, int, int, int>> Forest::getSpCount() {
	return mCounter->spCountList;
};