#pragma once





#ifndef RAND
#define RAND


#include <random>
#include "boost/random.hpp"


extern int seed;
extern boost::random::mt19937 rng;

static std::random_device rd;
static std::mt19937 gen(rd());


class Crand {

public:

	// Could use template<typename T> etc... Buttt, i won't ... Why?? ¯\_("")_/¯

	static double rand_double(double min, double max) {

		boost::random::uniform_real_distribution<double> dist(min, max);

		double val = dist(rng);

		return val;

	};

	static float randFloat(float min, float max) {

		boost::random::uniform_real_distribution<float> dist(min, max);

		float val = dist(rng);

		return val;

	};


	static int rand_int(int min, int max) {

		boost::random::uniform_int_distribution<> dist(min, max);

		int val = dist(rng);

		return val;

	};



};

const float Pi = 3.14159;
const float sqrtTau = sqrt(2 * Pi);

#endif // RAND