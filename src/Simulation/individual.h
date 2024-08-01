#pragma once


#ifndef INDIV
#define INDIV

// R-tree should be just as effiecient no mantter what I put in it, just more memory use. ALso just would take a while to rewrite code
struct indiv {

    int uniqueID;
    int species;

    float dispersal;

    float HNDD;
    float CNDD;



    // Define the equality comparison operator
    bool operator==(const indiv& other) const {
        return uniqueID == other.uniqueID &&
            species == other.species &&
            dispersal == other.dispersal &&
            HNDD == other.HNDD &&
            CNDD == other.CNDD;
    } // Still don't know what this really does << do it for observation? 

};

//  The idea of the observation is a nice when tring to replicate "real data"
struct observation {
    observation(int _repeat, int _forest, int _timeStep, int _uniqueID, int _species, float _x, float _y) :
        repeat(_repeat),
        forest(_forest),
        timeStep(_timeStep),
        uniqueID(_uniqueID),
        species(_species),
        x(_x),
        y(_y) {}

    int repeat;
    int forest;
    int timeStep;
    int uniqueID;
    int species;
    float x, y;
};

#endif // !INDIV

