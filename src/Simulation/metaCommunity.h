#pragma once

#ifndef META
#define META

#include "randFuncs.h"
#include "settings.h"
#include "Individual.h"

// Nice easy small class for now, May need expantion later on 
class metaComC {

public:

	metaComC(params& _params, std::vector<indiv>& _spLibrary);

	void step();

	std::vector<int> metaComL;

	std::vector<indiv>& spLibrary;

private:

	params& mParams;


	void buildMeta();
};



#endif