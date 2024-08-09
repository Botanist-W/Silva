
#include "Forest.h"

#ifndef DISP
#define DISP




class dispersal // Abstract base class
{
public:

	dispersal(Forest& _forest) :
		mForest(_forest) {};

	virtual point disperse(value& parent) = 0; // Passes in the parent so I can work out all the shizzle I need from that

	virtual bool inBounds(point&) = 0;

	virtual std::vector<value> search(point&, double&) = 0;

	virtual ~dispersal() = default;

protected:

	point dispersalKernal(value& parent);

	Forest& mForest;

	friend class continuousDisp;
	friend class fragmentedDisp;

};

class continuousDisp :
	public dispersal
{
public:

	continuousDisp(Forest& _forest) :
		dispersal(_forest) {
		LOG_INFO("Continuous Dispersal");
	};

	point disperse(value& parent) override;

	std::vector<value> search(point&, double&) override;

	bool inBounds(point&) override;

private:

	std::vector<value> searchResults;

	point dispTorus(point& pos);
	enum edgeCase;
	edgeCase whichEdge(const point& sought);
	void posMod(const point& sought, const point& edgeSought, std::vector<value>& input);
	void searchEdge(const point& edgeSought, const point& sought);
	void periodicBoundy(const point& sought);


};

class fragmentedDisp :
	public dispersal
{
public:

	fragmentedDisp(Forest& _forest) : dispersal(_forest) {};

	point disperse(value& parent) override;

	std::vector<value> search(point&, double&) override;

	bool inBounds(point&);

private:

};

#endif

