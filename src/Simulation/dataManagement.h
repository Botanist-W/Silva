#pragma once
#include <map>
#include "settings.h"
#include "RTREE.h"
#include "Forest.h"
#include "csv.h"


// Goal of this class
// Deal with the OUTPUT data and all that 
// Should it just be part of the settings class <-- probably 

class data {
public:

	data() {};
	~data() = default;

	void setPath(std::string& path);

	void setName(params& par);

	void saveResults(std::vector<observation>& result); // TODO: Intergrate these two methods so I dont have to waste time creating one massive list with everything before writing...

	static std::vector<value> getSample(int id);

private:

	std::string outputDirectory = "data/Output/";

	std::string outputFile = "data/Output/default.csv";

	int mCounter; // Keeps track of which line we on 
};


