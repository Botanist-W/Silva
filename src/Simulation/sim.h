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



class Sim
{
public:

	Sim(const params& _params, int _repeat);

	~Sim() = default;

	void runModel(); 

	std::vector<observation> getResults();

    std::shared_ptr<Forest> getForest(int id); // could try weak pointer 

	// vectors for output << a sum of all individual forest counts >> R friendly data 
	// These are all just terrible -- > Be better
	

private:
	// Constructor stuff
	void setup();
	void buildSpLib();
	void build(); // Build from the samples  
	void setImmigration(); // Terrible naming as ususal
	// ~ Constructor stuff

	params mParams; // Do I need const & for thread access??  << Does this mean I need to give everthing a const reference? 
	// Just copy it, then ur good!
	std::vector<std::shared_ptr<Forest>> forests;

	std::unique_ptr<Immigration> immigration; // Immigration type

	std::vector<indiv> spLibrary;


	int& repeat;

	// The important bit
	std::vector<observation> results;

};


#endif // !SIM
