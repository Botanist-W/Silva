#pragma once


#include "settings.h"
#include "randFuncs.h"
#include "RTREE.h"
#include "Forest.h"
#include "Log.h"

class Forest; // Some nice forward declaration  :)))

#ifndef IMMIGRATION
#define IMMIGRATION



class Immigration
{
public:

	Immigration(params& _params): mParams(_params){

	};

	virtual void handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) = 0;

	virtual bool mOccurence(int& ID, int&step) = 0;

	virtual void buildMetaCom(std::vector<indiv> spLib) =0;

	params& mParams;
	
	std::bernoulli_distribution mDist{ mParams.m }; // True or false for a chance of immigration << thanks standard lib

};


class metaImmigration : public Immigration {
public:
	
	metaImmigration(params& _params);

	void handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) override;

	bool mOccurence(int& ID, int& step) override;

	void buildMetaCom(std::vector<indiv> spLib) override;

private:
	bool doesImmigrationOccur = false;

	std::vector<indiv> metaCom;



};


class networkImmigration : public Immigration { // Passer class --> just preverves some code that is used in both network immigration senaios but not the metaCommunity immigatration 
public: 
	
	networkImmigration(params& _params);

	void handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) override;

	bool mOccurence(int& ID, int& step) override;

	void buildMetaCom(std::vector<indiv> spLib) override;

private:

	
	std::vector<std::vector<bool>> temporalImmigrationBool;// // To check whether immigration occurs at all for that Forest << Even nessasay ?? >> probs not 
	std::vector<std::vector<int>> temporalImmigrationMap;// { mSetting.numFragments, std::vector<int>(mSetting.timeSteps, 0) }; // To check where the immigration comes from // Reserving space and Set the whole lot to 0 cus most of them will be 
	// U might be wondering why I did this statically and the answer is I have no idea :( but it works so ...

	void printNodes(std::vector<std::vector<float>>& nodes);
	void printImmigrationMap();

	//template<typename T> // Take int or float
	void createTemporaMap(std::vector<std::vector<float>>&);

	
};











#endif // !IMMIGRATION