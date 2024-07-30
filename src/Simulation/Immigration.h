#pragma once


#include "settings.h"
#include "randFuncs.h"
#include "RTREE.h"
#include "Forest.h"

class Forest; // Some nice forward declaration  :)))

#ifndef IMMIGRATION
#define IMMIGRATION



class Immigration
{
public:

	Immigration(params& _params, std::vector<std::shared_ptr<Forest>> _forests) : mParams(_params) , mForests(_forests){
	};

	virtual void handleImmigration(int& step) = 0;

	virtual bool mOccurence(int& ID, int&step) = 0;

	virtual void buildMetaCom(std::vector<indiv> spLib) =0;

	params& mParams;
	std::vector<std::shared_ptr<Forest>> mForests;

	std::bernoulli_distribution mDist{ mParams.m }; // True or false for a chance of immigration << thanks standard lib

};


class metaImmigration : public Immigration {
public:
	
	metaImmigration(params& _params, std::vector<std::shared_ptr<Forest>> _forests) : Immigration(_params, _forests) {
	};

	void handleImmigration(int& step) override;

	bool mOccurence(int& ID, int& step) override;

	void buildMetaCom(std::vector<indiv> spLib) override;

private:
	bool doesImmigrationOccur = false;

	std::vector<indiv> metaCom;



};


class networkImmigration : public Immigration { // Passer class --> just preverves some code that is used in both network immigration senaios but not the metaCommunity immigatration 
public: 
	
	networkImmigration(params& _params, std::vector<std::shared_ptr<Forest>> _forests) : Immigration(_params, _forests) {
		temporalImmigrationMap = std::vector<std::vector<int>>(mParams.numFragments, std::vector<int>(mParams.timeSteps)); // Setting the size!
		temporalImmigrationBool = std::vector<std::vector<bool>>(mParams.numFragments, std::vector<bool>(mParams.timeSteps));

		createTemporaMap(mParams.nodeMap);

	}

	void handleImmigration(int& step) override;

	bool mOccurence(int& ID, int& step) override;

	void buildMetaCom(std::vector<indiv> spLib) override;

private:

	

	
	std::vector<std::vector<bool>> temporalImmigrationBool;// // To check whether immigration occurs at all for that Forest << Even nessasay ?? >> probs not 
	std::vector<std::vector<int>> temporalImmigrationMap;// { mSetting.numFragments, std::vector<int>(mSetting.timeSteps, 0) }; // To check where the immigration comes from // Reserving space and Set the whole lot to 0 cus most of them will be 
	// U might be wondering why I did this statically and the answer is I have no idea :( but it works so ...

	template<typename T>
	void logger(std::vector<std::vector<T>>& nodes);

	//template<typename T> // Take int or float
	void createTemporaMap(std::vector<std::vector<float>>&);

	
};





template<typename T>
void networkImmigration::logger(std::vector<std::vector<T>>& nodes) {
	std::cout << "\nCreating temp map: \n";
	std::cout << "node size: " << nodes[0].size() << " should be the same as: " << nodes[1].size() << "  check with nFragments:  " << mParams.numFragments << "\n\n";
	std::cout << "Check dimentions of immigration time map: \n numFragments: " << temporalImmigrationMap.size() << "\n";// << "\nTimeSteps: " << temporalImmigrationMap[1].size(); << "\n\n";

	std::cout << "Printing Node Map:  \n";
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes[i].size(); j++) {

			std::cout << nodes[i][j] << " ";

		}
		std::cout << "\n";
	}
	std::cout << "\n\n\n\n";

};







#endif // !IMMIGRATION