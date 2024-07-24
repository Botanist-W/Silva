
#include "Forest.h"

#ifndef COMP
#define COMP



class competition {
public:

	competition(Forest& _forest) : 
		mForest(_forest) {
		
	};

	virtual double compIndex(std::vector<value>&, value& parent, point& recPos) = 0;


protected:

	Forest& mForest;

	double densityKernel(point& recruit, point& neighbour, float& c);

	friend class neutralComp;
	friend class cnddComp;

};

class neutralComp : public competition {
public:

	neutralComp(Forest& _forest) :
		competition(_forest) {


	};

	double compIndex(std::vector<value>&, value& parent, point& recPos) override;

private:


};


class cnddComp : public competition {
public:

	cnddComp(Forest& _forest) :
		competition(_forest) {


	};

	double compIndex(std::vector<value>&, value& parent, point& recPos) override;

private:

	std::vector<int> speciesFilter(std::vector<value>&, value& parent, bool Conspecific);
};


#endif // !COMP

