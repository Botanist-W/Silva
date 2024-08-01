#include "sim.h"
#include "pch.h"

void Sim::test() {


	for (int i = 0; i < 10000000; i++) {
		double h = sqrt(i * i) * pow(2, i);
		LOG_DEBUG("val {} thread {} ", h, repeat);
	}

};

Sim::Sim(const params& _params, const int& _repeat) :
	mParams(_params), repeat(_repeat) {
	setup();
}


void Sim::setup() {
	
	//test();
	build();
	setImmigration();

	

};


void Sim::build() {
	
	if (mParams.buildFromSample) {
		for (int i = 0; i < mParams.numFragments; i++) {
			// Creating instances of Forest class and assigning them an ID just as a guard against stuff
			mForests.emplace_back(std::make_shared<Forest>(mParams, i));
			
			int sample = Crand::rand_int(0, 10); // Planning of having 10 samples

			//forests[i]->buildFromForest(data::getSample(0));

			LOG_DEBUG("Rtree size (N): {}", mForests[i]->tree.size());
			LOG_DEBUG("Built from sample");
		}
	}
	else {

		buildSpLib();
		LOG_INFO("size of species library: {} ", spLibrary.size());

		for (int i = 0; i < mParams.numFragments; i++) {
			// Creating instances of Forest class and assigning them an ID just as a guard against stuff
			mForests.emplace_back(std::make_shared<Forest>(mParams, i));

			mForests[i]->buildFromLib(spLibrary);

			LOG_DEBUG("Rtree size (N): {}", mForests[i]->tree.size());
			LOG_DEBUG("Built from Library");
		}
	}

};


// TODO: REMOVE!
void Sim::buildSpLib() {

	for (int i = 1; i <= mParams.numSpecies; i++) {

		indiv sp;

		sp.species = i;

		sp.dispersal = Crand::rand_double(5, 10); // TODO: move to settings 

		sp.CNDD = 0;

		sp.HNDD = 0; // TODO :: IMPLEMENT 

		spLibrary.emplace_back(sp);
	}


};

void Sim::setImmigration() {


	if (mForests.size() < 2)
	{
		immigration = std::make_unique<metaImmigration>(mParams);
		immigration->buildMetaCom(spLibrary);
	}
	else
		immigration = std::make_unique<networkImmigration>(mParams);


};

void Sim::runModel() {

	int captures = mParams.timeSteps / mParams.captureRate;

	int timeStep = 0;

	for (int capture = 0; capture < captures; capture++) {

		LOG_TRACE("Capture: {}", capture);

		// TODO: set up a loop in which a capture occurs 
		auto start = std::chrono::high_resolution_clock::now();

		for (int step = 0; step < mParams.captureRate; step++) { // using int becuase I aint changing things

			LOG_TRACE("time step: {}", timeStep);

			immigration->handleImmigration(step, mForests);

			for (int forest = 0; forest < mForests.size(); forest++) { // using int for the ID in m Occurence 

				LOG_TRACE("Forest: {}", forest);

				if (immigration->mOccurence(forest, step) == false) {

					mForests[forest]->localStep(); // TODO: pass in timestep here << SHOULD BE USING A MAP OMG

				}


			}
			timeStep++;


		}  // Step  (in between capture)

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		LOG_INFO("Elapsed time between captures: {} seconds", duration.count());

		//for (auto& forest : mForests)
			//results.emplace_back(forest->getCapture(repeat, timeStep)); // Beautiful ...... I hope

	} // Capture 

};


std::shared_ptr<Forest> Sim::getForest(int id) {

	return mForests[id];

}

std::vector<observation> Sim::getResults() {
	return results;
};

