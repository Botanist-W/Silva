#pragma once
#include "settings.h"
#include "Forest.h"
#include "Immigration.h"
#include "spCount.h"
#include <tuple>
#include <thread>
#include <chrono>
#include "dataManagement.h"

#ifndef SIMULATION
#define SIMULATION



class Simulation
{
public:

	Simulation(const params& _params, const int& _repeat);

	~Simulation() = default;

	void runModel(); 

	std::vector<observation>& getResults();
	std::vector<std::tuple<int, int, int, int>>& getSpCount();

    std::shared_ptr<Forest> getForest(int id); // Just for drawing >> probs remove tho


private:
	// Constructor stuff
	void setup();
	void buildSpLib();
	void build(); // Build from the samples  
	void setImmigration(); // Terrible naming as ususal
	// ~ Constructor stuff

	params mParams; // Do I need const & for thread access??  << Does this mean I need to give everthing a const reference? << SHOULDT BE BLANK

	// Just copy it, then ur good!
	std::vector<std::shared_ptr<Forest>> mForests;

	std::unique_ptr<Immigration> immigration; // Immigration type

	std::vector<indiv> spLibrary;


	const int repeat;

	// The important bit
	std::vector<observation> results;
	std::vector<std::tuple<int, int, int, int>> spCountResults;

};


#endif // !SIM
