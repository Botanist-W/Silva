#pragma once
#include <map>
#include "settings.h"
#include "RTREE.h"
#include "Forest.h"
#include "csv.h"


// Goal of this class
// Takes in a capture and stores it in a thingy 
// I dont know 
// Should it just be part of the forest class <-- probably 

class data {
public:

	data() {};
	~data() = default;

	void setPath(std::string& path);

	void setName(std::string& name);

	//static void saveResults(std::vector<observation>& result); // TODO: Intergrate these two methods so I dont have to waste time creating one massive list with everything before writing...

	static std::vector<value> getSample(int id);

private:

	std::string outputPath;

	std::string fileName;

	// Forest, timestep, value
	std::vector<std::tuple<int, int, int, value>> finalOutput; // Remember it is a vector of pointers to save on space 

};


