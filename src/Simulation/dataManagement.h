#pragma once
#include <map>
#include "settings.h"
#include "RTREE.h"
#include "Forest.h"
#include "csv.h"
#include "randFuncs.h"


// Goal of this class
// Deal with the OUTPUT data and all that 
// Should it just be part of the settings class <-- probably 

class data {
public:

	data() {};
	~data() = default;

	void setPath(const std::string& path);

	void setName(params& par);

	void saveResults(const std::vector<observation>& result); // TODO: Intergrate these two methods so I dont have to waste time creating one massive list with everything before writing...

	void saveSpCount(const std::vector<std::tuple<int, int, int, int>>& result);

	void setSampleDirectory(params& par, const std::string& );

	static std::vector<value> getSample(const std::string& directory, double bounds, int repeat, int sampleIndex);

	static int getFileCount(const std::string& directory);

private:

	void setCaptureName(params& par);
	void setCountName(params& par);

	std::string sampleDirectory = "../../Samples";

	std::string outputDirectory = "../../";

	std::string outputFile = "";

	std::string spCountOutFile = "";

	int mCounter; // Keeps track of which line we on 

};


