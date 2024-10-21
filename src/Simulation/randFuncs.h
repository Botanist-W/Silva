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

    static double randFloat(float min, float max) {
        thread_local static std::mt19937 randg{ std::random_device{}() }; 
        std::uniform_real_distribution<float> dist(min, max);
        return dist(randg);
    }

    static double rand_double(double min, double max) { // Great naming ass-wipe
        thread_local static std::mt19937 randg{ std::random_device{}() };
        std::uniform_real_distribution<double> dist(min, max);
        return dist(randg);
    }

    static int rand_int(int min, int max) {
        thread_local static std::mt19937 randg{ std::random_device{}() }; 
        std::uniform_int_distribution<int> dist(min, max);
        return dist(randg);
    }

};

const double Pi = 3.14159;
const double sqrtTau = sqrt(2 * Pi);

#endif // RAND