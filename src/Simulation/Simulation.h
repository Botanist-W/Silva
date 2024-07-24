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

	Simulation() {
		setup();
		outputCapture = std::vector<std::vector<std::pair<int, value>>>(mParams.numFragments, std::vector<std::pair<int, value>>(mParams.timeSteps));
		basicRun();
	}

	void run(); // Main function for this 

	void output();

	void basicRun(); // Test without threads

	// vectors for output << a sum of all individual forest counts >> R friendly data 
	std::vector<std::vector<std::pair<int, value>>> outputCapture;
	std::vector<std::tuple<int, int, int>>  outputSpCount;

private:
	// Constructor stuff
	void setup();
	void buildSpLib();
	void whichImmigration();
	//jytfiyf
	// ~ Constructor stuff

	// Run stuff
	void doThis();
	// ~ Run stuff

	// Output Stuff
	void capture(std::string&);
	// ~ Output stuff

	params mParams;

	std::vector<Forest> forests; // TODO: SET UP

	std::unique_ptr<Immigration> immigration; // Immigration type
	
	std::vector<indiv> spLibrary;

};


#endif // !SIM



