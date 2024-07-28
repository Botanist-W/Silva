#include "Simulation.h"


void Simulation::setup() {
	// Build a species library 
	buildSpLib();
	std::cout << "size of species library: " << spLibrary.size() << "\n";

	whichImmigration();
	
	for (int i = 0; i < mParams.numFragments; i++) {
		// Creating instances of Forest class and assigning them an ID just as a guard against stuff
		forests.emplace_back(Forest(mParams, i));
		forests[i].buildFromLib(spLibrary);
		std::cout << "Rtree size: " << forests[i].tree.size();
	}
	
	outputCapture.reserve(mParams.timeSteps);
	outputSpCount.reserve(mParams.timeSteps);

};


void Simulation::buildSpLib() {

	for (int i = 1; i <= mParams.numSpecies; i++) {

		indiv sp;

		sp.species = i;

		sp.dispersal = Crand::rand_double(5, 10); // TODO: move to settings 

		sp.survivalProb = Crand::rand_double(0.2, 1);

		sp.CNDD = 0;

		sp.HNDD = 0; // TODO :: IMPLEMENT 

		spLibrary.emplace_back(sp);
	}


};

void Simulation::whichImmigration() {
	
	if (mParams.metaCommunityImmigration)
		immigration = std::make_unique<metaImmigration>(mParams);
	else 
		immigration = std::make_unique<networkImmigration>(mParams, forests);
	

};

void Simulation::basicRun() {

	int timeStep = 0;

	for (int repeat = 0; repeat < mParams.numRep; repeat++) {
		// TODO: Implement multithreading for this repeat
		
		// TODO: set up a loop in which a capture occurs 

		for (int i = 0; i < mParams.timeSteps; i++) { 

			auto start = std::chrono::high_resolution_clock::now();
			immigration->handleImmigration(i);

			for (int f = 0; f < forests.size(); f++) { // using int for the ID in m Occurence 

				if (immigration->mOccurence(f, i) == false) {

					forests[f].localStep(); // TODO: pass in timestep here 

				}

			}
			timeStep++;

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = end - start;

			// Print the elapsed time
			std::cout << "Elapsed time: " << duration.count() << " seconds\n";

		}

		
		for (int i = 0; i < forests.size(); i++) 
			outputCapture[i] = forests[i].forestCapture(repeat); //problem
		

	}

	//speciesCount::forestsToCSV(outputCapture, mParams.rtreePath); // TODO : Implement a better output method :) 

};


Forest& Simulation::getForest(int id) {

	return forests[id];

}