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

	int timeStep = 0;

	// A bunch of nested loops are a sign of a good coder right?? 
	for (size_t repeat = 0; repeat < mParams.numRep; repeat++) {
		// TODO: Implement multithreading for this repeat
		
		for (size_t capture = 0; capture < captures; capture++) {
			// TODO: set up a loop in which a capture occurs 

			for (int i = 0; i < mParams.captureRate; i++) { // using int becuase I aint changing things

				auto start = std::chrono::high_resolution_clock::now();

				immigration->handleImmigration(i);

				for (int f = 0; f < forests.size(); f++) { // using int for the ID in m Occurence 

					if (immigration->mOccurence(f, i) == false) {

						forests[f]->localStep(); // TODO: pass in timestep here 

					}

				}
				timeStep++;

				auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> duration = end - start;

				// Print the elapsed time
				std::cout << "Elapsed time: " << duration.count() << " seconds\n";

			}  // Step  (in between capture)

		} // Capture 

		
		//for (int i = 0; i < forests.size(); i++) 
			//outputCapture[i] = forests[i]->forestCapture(repeat); //problem
		

	} // Repeat

	//speciesCount::forestsToCSV(outputCapture, mParams.rtreePath); // TODO : Implement a better output method :) 

};


std::shared_ptr<Forest> Simulation::getForest(int id) {

	return forests[id];

}