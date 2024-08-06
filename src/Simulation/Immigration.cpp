#include "Immigration.h"
#include "pch.h"

// Constructors

metaImmigration::metaImmigration(params& _params) : Immigration(_params) {
	LOG_INFO("Using meta-community immigration");
};



// Here is the poor memory use!! 
networkImmigration::networkImmigration(params& _params) : Immigration(_params) {
	temporalImmigrationMap = std::vector<std::vector<int>>(mParams.numFragments, std::vector<int>(mParams.timeSteps)); // Setting the size!
	temporalImmigrationBool = std::vector<std::vector<bool>>(mParams.numFragments, std::vector<bool>(mParams.timeSteps));
	createTemporaMap(mParams.nodeMap);
	LOG_INFO("Using network immigration");
}





void metaImmigration::buildMetaCom(std::vector<indiv> spLib) {
	
	metaCom.reserve(mParams.metaComSize);

	for (size_t i = 0; i < mParams.metaComSize; i++) {
		
		metaCom.emplace_back(spLib[Crand::rand_int(0, int(spLib.size() - 1))]);
		
	}
	LOG_TRACE("Built meta-community {}", metaCom.size());
};

void networkImmigration::buildMetaCom(std::vector<indiv> spLib) {
} 

void metaImmigration::handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) {

	for (size_t i = 0; i < forests.size(); i++) {
		if (mDist(gen)) {
			forests[i]->removeTree(forests[i]->randomTree());
			forests[i]->addTree(value(point(Crand::randFloat(0, forests[i]->bounds), Crand::randFloat(0, forests[i]->bounds)), metaCom[Crand::rand_int(0, int(metaCom.size() - 1))]));
			doesImmigrationOccur = true;
			LOG_TRACE("Immigration has Occured");
		}
		else
		{
			doesImmigrationOccur = false;
		}

	}
};

bool metaImmigration::mOccurence(int& ID, int& step) {
	if (doesImmigrationOccur)// NOPE ! Unlesss!
		return true;
	else
		return false;
};

void networkImmigration::handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) {
	
	for (int i = 0; i < forests.size(); i++) {
		
		if (temporalImmigrationBool[i][timeStep]) { // Does immigration occur?
			// Immigration occurs , so this will be the step
			forests[i]->removeTree(forests[i]->randomTree());

			// Take a random individual from the target forest
			forests[i]->addTree(forests[temporalImmigrationMap[i][timeStep]]->randomTree());

			LOG_TRACE("Immigration occured from fragment: {} to {}", i , temporalImmigrationMap[i][timeStep]);
		}
	}
	
} // And that's it :) if immigration does not occur, it doesn't matter , nothing else has to happen 

// Important note: This DOES NOT prevent a normal timestep from occuring!! Implement that into each thread!!! it could mean a bit quicker maybe??? 


// Nice simple accesor, typing to be good and not make data member accessable outside the class even tho I really want to 
bool networkImmigration::mOccurence(int& ID, int& step) {

	if (temporalImmigrationBool[ID][step])
		return true;
	else
		return false;

};



void networkImmigration::createTemporaMap(std::vector<std::vector<float>>& nodes) { // This may be a little slow for the int based thing but not much of a differences because very scalable 
	
	for (size_t i = 0; i < nodes.size(); i++) {

		std::discrete_distribution<int> dist(nodes[i].begin(), nodes[i].end()); // Store the immigration probability to other fragments here

		for (int j = 0; j < mParams.timeSteps; j++) { // careful here, remeber this isnt the node map were making 


			if (mDist(gen)) {  // Generate random number & check if there is any immigration 

				temporalImmigrationBool[i][j] = true;
				temporalImmigrationMap[i][j] = dist(gen);

			}
			else {
				temporalImmigrationBool[i][j] = false;
				temporalImmigrationMap[i][j] = i; // Just setting it to the same fragment as a 
			}
		}
	}
	LOG_TRACE("Built temporal immigration map");
	//printNodes(nodes); NOT THREAD SAFE!!!! SO watch out 
	//printImmigrationMap();
};




// Just printing the head
void networkImmigration::printImmigrationMap() {
	
	// First 20 rows 
	for (int i = 0; i < 20; i++) {

		for (int j = 0; j < temporalImmigrationMap.size(); j++) {

			std::cout << " ," << temporalImmigrationMap[j][i];

		}
		std::cout << "\n";
	}



};



void networkImmigration::printNodes(std::vector<std::vector<float>>& nodes) {

	LOG_DEBUG("Printing nodes: ");

	for (int i = 0; i < nodes.size(); i++) {

		for (int j = 0; j < nodes[i].size(); j++) {

			std::cout << " ," << nodes[i][j];

		}
		std::cout << " ,\n";
	}
	std::cout << "\n";


	LOG_DEBUG("Printing Weights");

	std::vector<int> flatten_weights;
	for (const auto& row : nodes) {
		flatten_weights.insert(flatten_weights.end(), row.begin(), row.end());
	}

	int sum_weights = std::accumulate(flatten_weights.begin(), flatten_weights.end(), 0);

	if (sum_weights <= 0) {
		LOG_ERROR("Error The sum of weights is zero or negative: {}", sum_weights);
		return;
	}
	else {
		LOG_DEBUG("Weights should work: {}", sum_weights);
	}


};