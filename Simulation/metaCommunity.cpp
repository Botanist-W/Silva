#include "metacommunity.h"


metaComC::metaComC(params& _params, std::vector<indiv>& _spLibrary) :
	mParams(_params),
	spLibrary(_spLibrary) {

	metaComL.reserve(mParams.metaComSize);

	buildMeta();

}

void metaComC::buildMeta() {
	for (int i = 0; i < mParams.metaComSize; i++)
		metaComL.emplace_back(Crand::rand_int(1, mParams.numSpecies));
}

void metaComC::step() { // No species will go extinct here. 
	metaComL[Crand::rand_int(0, mParams.metaComSize - 1)] = Crand::rand_int(1, mParams.numSpecies);
}
