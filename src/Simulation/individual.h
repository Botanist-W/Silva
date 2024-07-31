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
    } // Still don't know what this really does << do it for observation? 

};

// Note: I could reduce the size of the indiv struct because a lot of the member vars arent used but I might want to use them in the future 
//       and

//  The idea of the observation is a nice when tring to replicate "real data" << plus it saves on data over sotring the whole indiv 
struct observasion {
    int repeat, forest, timeStep, species;
    float x, y;
};

// What is better for memory? this or tuple<int,int,int,int,float,float>  VERY CLUMSY tho 



#endif // !INDIV

