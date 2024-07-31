#pragma once
#include "settings.h"
#include "Forest.h"
#include "Immigration.h"
#include "spCount.h"
#include <tuple>
#include <thread>
#include <chrono>

#ifndef SIM
#define SIM




class Simulation
{
public:

	Simulation(params& _params);

	~Simulation() = default;

	void run(); // Main function with threads hopefully

	void basicRun(); // Test without threads

	std::shared_ptr<Forest> getForest(int id); // could try weak pointer 

	// vectors for output << a sum of all individual forest counts >> R friendly data 
	// These are all just terrible -- > Be better
	std::vector<std::vector<std::pair<int, value>>> outputCapture; 
	std::vector<std::tuple<int, int, int>>  outputSpCount;

private:
	// Constructor stuff
	void setup();
	void buildSpLib();
	void whichImmigration(); // Terrible naming as ususal
	// ~ Constructor stuff

	params& mParams; // Do I need const & for thread access??  << Does this mean I need to give everthing a const reference? 

	std::vector<std::shared_ptr<Forest>> forests; 

	std::unique_ptr<Immigration> immigration; // Immigration type
	
	std::vector<indiv> spLibrary;

};


#endif // !SIM



