#pragma once
#include <map>
#include "settings.h"


// Goal of this class
// Takes in a capture and stores it in a thingy 
// I dont know 

class data {
public:
	
	data(mParams& _params) : mParams(_params);

	~data() = default;

	void captureForest(); // Takes just one forest and stores it to a vector;

	void outputData();

private:
	params& mParams;

	std::map<int


	//std::vector

};