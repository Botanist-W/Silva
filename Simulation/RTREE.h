
#include "Individual.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include "settings.h"
#include "randFuncs.h"



namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<point, indiv> value;
typedef bgi::rtree<value, bgi::quadratic<16>> RTREE; // TODO: Find the optimal sorting method 
//typedef boost::geometry::index::rtree<value, boost::geometry::index::quadratic<16>> RTREE; // TODO: Try makie this work!!! Tends to be more efficient

// Remember to leave this OUT of the #ifndef!!!

#ifndef RTREECLASS
#define RTREECLASS


class Build; // Foward dec



class rTree
{

public:

	RTREE tree; // R tree 

	// Constructor 
	rTree(params& s) : mParams(s) {

		
		
	};

	~rTree() {
		//std::cout << "Deleted R tree \n";
	};

	// Values copied from settings --> just to make my life easier in future classes <-- dont care if its bad practice at this point 
	float searchArea;
	float bounds;
	float treeDensity;
	float numIndiv;
	float numSpecies;
	float SD;
	float b1 = 0.01;
	float b2 = 7;
	float m = 0.03; // Immigration rate
	float HNDD = 0.1; // Default HNDD strength TODO: implement
	float CNDD = 0.1; // Default CNDD
	float b = 2;
	
	// Functions
	std::vector<value> search(point foo, float& searchArea);

	void addTree(value foo);

	void removeTree(value foo);

	value randomTree(); 

	void buildFromLib(std::vector<indiv>&);

	void build(std::vector<value>&);

private:
	params& mParams;

	friend class Forest;
};



#endif // RTREECLASS

