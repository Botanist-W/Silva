#include "Run.h"
#include "pch.h"


Run::Run() {
	
	init(); // WARNING: Can run everthing 

	// Read in settings

	// Reserve space for data

	// Init repeats
	for (int i = 0; i < mParams.numRep; i++) {
		mRepeats.emplace_back(std::make_unique<Sim>(mParams, i));
		LOG_DEBUG("Created forest: {}", i);
	}

};


void Run::init() {
	// Run app
	std::cout << "use gui? \n";
	char gui;
	std::cin >> gui;
	if (gui == 'y') {
		LOG_INFO("Using gui: ");
		mApplication = std::make_unique<App>();
	}
	else {
		LOG_INFO("No gui: ");
		noGUI();
	}
};


void Run::runSimulation() {

	// Move the while loop to here
	while (true) {
		mApplication->Run();
	} 
	// Close the gui
	
	// Run the simulation
	



};

void Run::runRepeats() {
	
	std::vector<std::future<void>> futures;

	// Launch the processes in parallel
	for (auto& sim : mRepeats) {
		// Launch each `process` method asynchronously
		futures.push_back(std::async(std::launch::async, &Sim::test, sim.get()));
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