#include "Run.h"
#include "pch.h"


Run::Run() {
	
	init(); // WARNING: Can run everthing 

	// Read in settings

	// Reserve space for data

	
};


void Run::init() {
	// Run app
	//std::cout << "use gui? \n";
	//char gui;
	//std::cin >> gui;
	//if (gui == 'y') {
		//LOG_INFO("Using gui: ");
		mApplication = std::make_unique<App>(mSettings, mData); // Is this best way to manage the settings/data?????
	//}
	//else {
		//LOG_INFO("No gui: ");
		//noGUI();
	//}
};


void Run::runSimulation() {

	// Move the while loop to here
	while (true) {
		if(!mApplication->Run())
			break;
	} 

	// Get params
	mParams = mSettings.get();

	if (mParams.sampleDirectory != "")
		mParams.buildFromSample = true;

	// Init repeats
	for (int i = 0; i < mParams.numRep; i++) {
		mRepeats.emplace_back(std::make_unique<Simulation>(mParams, i));
		LOG_DEBUG("Created forest: {}", i);
	}

	// Run 
	// TODO: set up a loop in which a capture occurs 
	auto start = std::chrono::high_resolution_clock::now();

	runRepeats();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	LOG_INFO("FINSIHED SIMULATION... UPLOADING {} ", duration.count());


	for (int i = 0; i < mRepeats.size(); i++) {
		LOG_DEBUG("Saving result: {}", i); 
		mData.saveResults(mRepeats[i]->getResults());
		mData.saveSpCount(mRepeats[i]->getSpCount());
	}

};

void Run::runRepeats() {
	
	std::vector<std::future<void>> futures;

	// Launch the processes in parallel
	for (auto& sim : mRepeats) {
		// Launch each `process` method asynchronously
		futures.push_back(std::async(std::launch::async, &Simulation::runModel, sim.get()));
	}

	// Wait for all futures to complete
	for (auto& future : futures) {
		future.get(); // This will block until the future is ready
	}

};


void Run::noGUI() {
	std::string input;
	std::string output;
	std::cout << "Input settings path: \n";
	std::cin >> input;
	std::cout << "\nInput output path: \n";
	std::cin >> output;

	// Load settings
	mSettings.load(input);

	// Run

	// output
	// mData.output(output);
};