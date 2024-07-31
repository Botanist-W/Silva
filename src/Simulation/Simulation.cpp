#include "Simulation.h"


Simulation::Simulation(params& _params) :
	mParams(_params) {
	setup();
}


void Simulation::setup() {
	// Build a species library 
	buildSpLib();
	LOG_INFO("size of species library: {} ", spLibrary.size());

	whichImmigration();
	
	for (int i = 0; i < mParams.numFragments; i++) {
		// Creating instances of Forest class and assigning them an ID just as a guard against stuff
		forests.emplace_back(std::make_shared<Forest>(mParams, i));
		forests[i]->buildFromLib(spLibrary);
		LOG_INFO("Rtree size (N): {}", forests[i]->tree.size());
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
		LOG_INFO("Repeat: {}", repeat);

		int timeStep = 0;

		for (size_t capture = 0; capture < captures; capture++) {

			LOG_INFO("Capture: {}", capture);

			// TODO: set up a loop in which a capture occurs 
			auto start = std::chrono::high_resolution_clock::now();

			for (int step = 0; step < mParams.captureRate; step++) { // using int becuase I aint changing things

				LOG_INFO("time step: {}", timeStep);

				immigration->handleImmigration(step);

				for (int forest = 0; forest < forests.size(); forest++) { // using int for the ID in m Occurence 

					LOG_INFO("Forest: {}", forest);

					if (immigration->mOccurence(forest, step) == false) {

						forests[forest]->localStep(); // TODO: pass in timestep here << SHOULD BE USING A MAP OMG

					}

				}
				timeStep++;
		

			}  // Step  (in between capture)

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = end - start;


			LOG_INFO("Elapsed time between captures: {} seconds", duration.count());

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