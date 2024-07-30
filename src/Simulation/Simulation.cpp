#include "Simulation.h"


void Simulation::setup() {
	// Build a species library 
	buildSpLib();
	std::cout << "size of species library: " << spLibrary.size() << "\n";

	whichImmigration();
	
	for (int i = 0; i < mParams.numFragments; i++) {
		// Creating instances of Forest class and assigning them an ID just as a guard against stuff
		forests.emplace_back(std::make_shared<Forest>(mParams, i));
		forests[i]->buildFromLib(spLibrary);
		std::cout << "Rtree size (N): " << forests[i]->tree.size() << "\n";
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
	{
		immigration = std::make_unique<metaImmigration>(mParams, forests);
		immigration->buildMetaCom(spLibrary);
	}
	else 
		immigration = std::make_unique<networkImmigration>(mParams, forests);
	

};

void Simulation::basicRun() {
	int captures = mParams.timeSteps / mParams.captureRate;

	int rep = 0; // I dont like all the naming conven

	// A bunch of nested loops are a sign of a good coder right?? 
	for (size_t repeat = 0; repeat < mParams.numRep; repeat++) {
		// TODO: Implement multithreading for the repeat

		int timeStep = 0;

		for (size_t capture = 0; capture < captures; capture++) {
			// TODO: set up a loop in which a capture occurs 
			auto start = std::chrono::high_resolution_clock::now();
			for (int step = 0; step < mParams.captureRate; step++) { // using int becuase I aint changing things

				immigration->handleImmigration(step);

				for (int forest = 0; forest < forests.size(); forest++) { // using int for the ID in m Occurence 

					if (immigration->mOccurence(forest, step) == false) {

						forests[forest]->localStep(); // TODO: pass in timestep here << SHOULD BE USING A MAP OMG

					}

				}
				timeStep++;
		

			}  // Step  (in between capture)

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = end - start;
			std::cout << "Elapsed time: " << duration.count() << " seconds\n";

			for (auto& forest : forests)
				forest->captureForest(rep, timeStep); // Beautiful ...... I hope

		} // Capture 

		// Big desitions here
		// Add the captures to the data class

	} // Repeat

	//speciesCount::forestsToCSV(outputCapture, mParams.rtreePath); // TODO : Implement a better output method :) 

};


std::shared_ptr<Forest> Simulation::getForest(int id) {

	return forests[id];

}