#include "Run.h"
#include "pch.h"


Run::Run() {
	
	// Run app

	// Read in settings

	// Reserve space for data

	// Init repeats
	for (int i = 0; i < mParams.numRep; i++) {
		mRepeats.emplace_back(std::make_unique<Sim>(mParams, i));
	}

};