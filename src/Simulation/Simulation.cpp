#include "Simulation.h"
#include "pch.h"


Simulation::Simulation(const params& _params, const int& _repeat) :
	mParams(_params), repeat(_repeat) {
	setup();

	if (repeat == 0)
		mTimer = std::make_unique<activeTimer>();
	else
		mTimer = std::make_unique<nullTimer>();

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

		int fileCount = data::getFileCount(mParams.sampleDirectory);

		int sampleIndex = Crand::rand_int(0, fileCount-1);

		for (int i = 0; i < mParams.numFragments; i++) {
			// Creating instances of Forest class and assigning them an ID just as a guard against stuff
			mForests.emplace_back(std::make_shared<Forest>(mParams, i));
			
			std::vector<value> samples = data::getSample(mParams.sampleDirectory, mParams.fragmentSizeList[i], repeat, sampleIndex);

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
	double a = mParams.dispersalDis * sqrt(1 / Pi);

	for (int i = 0; i <= mParams.numSpecies; i++) {

		indiv sp;

		sp.species = i;

		sp.dispersal = a; // TODO: DO THIS BETTER 

		sp.CNDD = 0;

		sp.HNDD = 0; // TODO :: IMPLEMENT 

		spLibrary.emplace_back(sp);
	}


};

void Simulation::setImmigration() {

	switch (mParams.metaCommunityImmigration)
	{
	case 0:
		immigration = std::make_unique<noImmigration>(mParams);
		break;

	case 1:
		immigration = std::make_unique<metaImmigration>(mParams);
		break;

	case 2:
		immigration = std::make_unique<metaImmigrationComp>(mParams);
		break;

	case 3:
		immigration = std::make_unique<networkImmigration>(mParams);
		break;

	case 4:
		immigration = std::make_unique<networkImmigrationMeta>(mParams);
		break;

	case 5:
		immigration = std::make_unique<networkImmigrationComp>(mParams);
		break;

	case 6:
		immigration = std::make_unique<networkImmigrationCompMeta>(mParams);
		break;


	default:
		
		LOG_ERROR("No immigration is being set");

		break;
	}

	immigration->buildMetaCom(spLibrary);

};

void Simulation::runModel() {
	//logParams(mParams);
	LOG_DEBUG("Getting to thr first loop?");

	int captures = mParams.timeSteps / mParams.captureRate;

	int timeStep = 0;

	int sizeSum = std::accumulate(mParams.fragmentSizeList.begin(), mParams.fragmentSizeList.begin(), 0);
	results.reserve(((sizeSum * sizeSum) / mForests.size()) * captures);



	int forest = 0;

	for (size_t capture = 0; capture < captures; capture++) {

		LOG_TRACE("Capture: {}", capture);

		for (size_t step = 0; step < mParams.captureRate; step++) { // using int becuase I aint changing things

			LOG_TRACE("time step: {}", timeStep);
			
			immigration->handleImmigration(mForests);

			auto randForest = Crand::rand_int(0, mForests.size()-1); // Choosing which forest will have a local step this time

			mForests[randForest]->localStep();
			//mForests[randForest]->counter(repeat, timeStep, true);

			for (size_t f = 0; f < mForests.size(); f++) {
				if (f == randForest)
					mForests[randForest]->counter(repeat, timeStep, true);
				else
					mForests[f]->counter(repeat, timeStep, false);

			}
			
			timeStep++;
			mTimer->logger(timeStep);
		}  // Step  (in between capture)



		for (auto& forest : mForests) {
			std::vector<observation> captures = forest->getCapture(repeat, timeStep);
			LOG_DEBUG("captures size {}", captures.size());
			results.insert(results.end(), // This might be where the terrible memory useage is coming from... scattering the results everywhere
				std::make_move_iterator(captures.begin()),
				std::make_move_iterator(captures.end()));
		}
	} // Capture 

	
	for (auto& forest : mForests) {
		forest->counter(repeat, timeStep, true);// Last capture hopefully
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











