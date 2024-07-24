#pragma once


#ifndef INDIV
#define INDIV

// R-tree should be just as effiecient no mantter what I put in it, just more memory use. ALso just would take a while to rewrite code
struct indiv {

    int uniqueID;
    int species;

    float survivalProb;

    float dispersal;

    float HNDD;
    float CNDD;



    // Define the equality comparison operator
    bool operator==(const indiv& other) const {
        return uniqueID == other.uniqueID &&
            species == other.species &&
            survivalProb == other.survivalProb &&
            dispersal == other.dispersal &&
            HNDD == other.HNDD &&
            CNDD == other.CNDD;
    }

};



#endif // !INDIV

