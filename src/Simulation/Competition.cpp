#include "competition.h"


// 
double competition::densityKernel(point& recruit, point& neighbour, float& c) { // Double may actually be useful here, because need some fine stuff, but watch out for performance 
	// This is to be modelled off of Kalyuzky et al 2023 (but with a few moddifications) 
	return c/(1 + pow((bg::distance(recruit, neighbour) / mForest.b1), mForest.b2));
};



// Neutral
double neutralComp::compIndex(std::vector<value>& searchResults, value& parent, point& recPos) {

	double cumDensity = 0;

	for (auto& i : searchResults) 
		cumDensity += densityKernel(i.first, recPos, mForest.HNDD); // Neutral so no worries

	return cumDensity;


};


// ~ Neutral


// CNDD

double cnddComp::compIndex(std::vector<value>& searchResults, value& parent, point& recPos) {

	double cumDensity = 0;

	const int sp = parent.second.species;

	for (auto& i : searchResults) { // This is way smarter!!
		if(i.second.species == sp)
			cumDensity += densityKernel(i.first, recPos, mForest.CNDD); // Wow this is much smarter!!
		else
			cumDensity += densityKernel(i.first, recPos, mForest.HNDD); 
	}
	return cumDensity;

}



// OBSOLETE!!!!!
std::vector<int> cnddComp::speciesFilter(std::vector<value>& searchResults, value& parent, bool Conspecific) {
	// returns a map of the conspecics and heterspecifics

	std::vector<int> output;

	const int sp = parent.second.species;

	for (int i = 0; i < searchResults.size(); i ++) {

		if ((searchResults[i].second.species == sp) == Conspecific) // Nice!
			output.emplace_back(i);

	}

	return output;

};