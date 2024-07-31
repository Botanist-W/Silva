#pragma once
#include <map>
#include "settings.h"
#include "RTREE.h"
#include "Forest.h"


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

	void collectCaptureMaps(std::vector<std::shared_ptr<Forest>>& forests); // Reference to a vector of shared pointers... I'm so far down a rabbit hole, I have no idea what's going on but it keeps working

	void mapToCSV(); // TODO: Intergrate these two methods so I dont have to waste time creating one massive list with everything before writing...

private:

	std::string outputPath;

	std::string fileName;

	// Forest, timestep, value
	std::vector<std::tuple<int, int, int, value>> finalOutput; // Remember it is a vector of pointers to save on space 

};


