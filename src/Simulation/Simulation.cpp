#include "Simulation.h"
#include "pch.h"


Simulation::Simulation(const params& _params, const int& _repeat) :
	mParams(_params), repeat(_repeat) {
	setup();
}


void Simulation::setup() {
	
	//test();
	build();
	setImmigration();

	//size_t ComunnitySizes = std::accumulate(mParams.fragmentSizeList.begin(), mParams.fragmentSizeList.end(), 0);
	//size_t resultSize = (ComunnitySizes * ComunnitySizes * mParams.treeDensity)/mParams.numFragments; // Idk if this is the fasted way but its correct
	//results.reserve(resultSize); // No copies for u;
	
	LOG_DEBUG("struggling with reservation?");
};


void Simulation::build() {

	buildSpLib();

	// swap ifs and for loops 
	if (mParams.buildFromSample) {
		for (int i = 0; i < mParams.numFragments; i++) {
			// Creating instances of Forest class and assigning them an ID just as a guard against stuff
			mForests.emplace_back(std::make_shared<Forest>(mParams, i));
			
			int sample = Crand::rand_int(0, 10); // Planning of having 10 samples

			std::vector<value> samples = data::getSample(mParams.sampleDirectory, mParams.fragmentSizeList[i]);

			mForests[i]->buildFromForest(samples);
			mForests[i]->initCounter();

			LOG_DEBUG("Rtree size (N): {}", mForests[i]->tree.size());
			LOG_DEBUG("Built from sample");
		}
	}
	else {

		LOG_DEBUG("size of species library: {} ", spLibrary.size());

		for (int i = 0; i < mParams.numFragments; i++) {
			// Creating instances of Forest class and assigning them an ID just as a guard against stuff
			mForests.emplace_back(std::make_shared<Forest>(mParams, i));

			mForests[i]->buildFromLib(spLibrary);
			mForests[i]->initCounter();

			LOG_DEBUG("Rtree size (N): {}", mForests[i]->tree.size());
			LOG_DEBUG("Built from Library");
		}
	}

};


// TODO: REMOVE!
void Simulation::buildSpLib() {

	for (int i = 0; i <= mParams.numSpecies; i++) {

		indiv sp;

		sp.species = i;

		sp.dispersal = 11.2837; // TODO: DO THIS BETTER 

		sp.CNDD = 0;

		sp.HNDD = 0; // TODO :: IMPLEMENT 

		spLibrary.emplace_back(sp);
	}


};

void Simulation::setImmigration() {


	if (mForests.size() < 2)
	{
		immigration = std::make_unique<metaImmigration>(mParams);
		immigration->buildMetaCom(spLibrary);
	}
	else
		immigration = std::make_unique<networkImmigration>(mParams);


};

void Simulation::runModel() {

	LOG_DEBUG("Getting to thr first loop?");

	int captures = mParams.timeSteps / mParams.captureRate;

	int timeStep = 0;

	int sizeSum = std::accumulate(mParams.fragmentSizeList.begin(), mParams.fragmentSizeList.begin(), 0);
	results.reserve(((sizeSum * sizeSum) / mForests.size()) * mParams.timeSteps);


	for (int capture = 0; capture < captures; capture++) {

		LOG_TRACE("Capture: {}", capture);

		// TODO: set up a loop in which a capture occurs 
		auto start = std::chrono::high_resolution_clock::now();

		for (int step = 0; step < mParams.captureRate; step++) { // using int becuase I aint changing things

			LOG_TRACE("time step: {}", timeStep);

			immigration->handleImmigration(step, mForests);

			for (int forest = 0; forest < mForests.size(); forest++) { // using int for the ID in m Occurence 

				
				if (immigration->mOccurence(forest, step) == false) {

					mForests[forest]->localStep(); // TODO: pass in timestep here << SHOULD BE USING A MAP OMG
					/// Multithreading is slower for this bit which is a piss take :/
				} 

				mForests[forest]->counter(repeat, timeStep);
				
			}

			timeStep++;
		
		}  // Step  (in between capture)

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		LOG_TRACE("Elapsed time between captures: {} seconds", duration.count());

		for (auto& forest : mForests) {
			std::vector<observation> captures = forest->getCapture(repeat, timeStep);
			LOG_DEBUG("captures size {}", captures.size());
			results.insert(results.end(), // This might be where the terrible memory useage is coming from... scattering the results everywhere
				std::make_move_iterator(captures.begin()),
				std::make_move_iterator(captures.end()));
		}
	} // Capture 

	
	for (auto& forest : mForests) {
		forest->counter(repeat, timeStep);// Last capture hopefully
		LOG_DEBUG("Uploading forests");
		std::vector<std::tuple<int, int, int, int>> counts = forest->getSpCount();
		LOG_DEBUG("Species Count size {}", counts.size());
		spCountResults.insert(spCountResults.end(),
			std::make_move_iterator(counts.begin()),
			std::make_move_iterator(counts.end()));

	}

};


std::shared_ptr<Forest> Simulation::getForest(int id) {

	return mForests[id];

}

std::vector<observation>& Simulation::getResults() {
	return results;
};

std::vector<std::tuple<int, int, int, int>>& Simulation::getSpCount() {
	return spCountResults;
};