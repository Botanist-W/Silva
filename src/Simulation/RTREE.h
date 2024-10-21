
#include "Individual.h"
#include <boost/geometry.hpp>

#include "settings.h"
#include "randFuncs.h"




namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<point, indiv> value;
typedef bgi::rtree<value, bgi::rstar<16>> RTREE; // TODO: Find the optimal sorting method 
//typedef boost::geometry::index::rtree<value, boost::geometry::index::quadratic<16>> RTREE; // TODO: Try makie this work!!! Tends to be more efficient

// Remember to leave this OUT of the #ifndef!!! for Some reason unknown to me 





#ifndef RTREECLASS
#define RTREECLASS





class Build; // Foward dec



class rTree
{

public:

	RTREE tree; // R tree 

	// Constructor 
	rTree(const params& s) : mParams(s) {

		if (bounds > 100)
			randomBoxSize = 20;
		else
			randomBoxSize = bounds;
		
	};

	~rTree() {
		
	};

	// Values copied from settings --> just to make my life easier in future classes <-- dont care if its bad practice at this point << STOP THIS 
	double searchArea;
	double bounds;
	double treeDensity;
	int numIndiv;
	int numSpecies;
	double b1 = 0.01;
	double b2 = 7;
	double m = 0.03; // Immigration rate
	double HNDD = 0.1; // Default HNDD strength TODO: implement
	double CNDD = 0.1; // Default CNDD
	double b = 2;
	int timeSteps = 0;
	
	// Functions
	std::vector<value> search(point foo, double& searchArea);

	void addTree(value foo);

	void removeTree(value foo);

	value randomTree(); 

	std::vector<value> getValues();

	void buildFromLib(std::vector<indiv>&);

	void build(std::vector<value>&); // This is the important bit 

private:
	const params& mParams;

	int addedSp;
	int removedSp;

	int randomBoxSize;

	friend class Forest;
};



#endif // RTREECLASS

