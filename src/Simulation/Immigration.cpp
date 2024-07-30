#include "Immigration.h"


void metaImmigration::buildMetaCom(std::vector<indiv> spLib) {
	
	metaCom.reserve(mParams.metaComSize);

	for (size_t i = 0; i < mParams.metaComSize; i++) {
	
		metaCom.emplace_back(spLib[Crand::rand_int(0, spLib.size() - 1)]);
		
	}

};

void networkImmigration::buildMetaCom(std::vector<indiv> spLib) {
}

void metaImmigration::handleImmigration(int& step) {

	for (size_t i = 0; i < mForests.size(); i++) {
		if (mDist(gen)) {
			mForests[i]->addTree(value(point(Crand::randFloat(0, mForests[i]->bounds), Crand::randFloat(0, mForests[i]->bounds)), metaCom[Crand::rand_int(0, metaCom.size() - 1)]));
			doesImmigrationOccur = true;
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

void networkImmigration::handleImmigration(int& step) {

	for (size_t i = 0; i < mForests.size(); i++) {

		if (temporalImmigrationBool[i][step]) { // Does immigration occur?
			// Immigration occurs , so this will be the step
			mForests[i]->removeTree(mForests[i]->randomTree());
			// Take a random individual from the target forest
			mForests[i]->addTree(mForests[temporalImmigrationMap[i][step]]->randomTree());
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

		//std::cout << "node: " << i << "\n";
		std::discrete_distribution<int> dist(nodes[i].begin(), nodes[i].end()); // Store the immigration probability to other fragments here

		for (int j = 0; j < mParams.timeSteps; j++) { // careful here, remeber this isnt the node map were making 

			//std::cout << "timestep: " << j << " ";

			if (mDist(gen)) {  // Generate random number & check if there is any immigration 

				temporalImmigrationBool[i][j] = true;
				temporalImmigrationMap[i][j] = dist(gen);

			}
			else {
				temporalImmigrationBool[i][j] = false;
				temporalImmigrationMap[i][j] = i;
			}
		}
	}
};
