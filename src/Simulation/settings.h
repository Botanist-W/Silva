
#ifndef SPACE_SETTINGS
#define SPACE_SETTINGS

#include <string>
#include <vector>

struct params { // 

    // Simulation Settings
    int timeSteps = 1000;
    int numRep = 1;  //number replicates

    int numSpecies = 300; // number of species  
    float treeDensity = 0.04; // Number of trees per area or lambda for pois()

    // Immigration settings
    bool metaCommunityImmigration = false;
    int metaComSize = 5000;

    // Fragment interactions etc
    int numFragments = 3;
    bool equalFragmentSize = true;
    float size = 50; // L ... Size of simulation Usefull for continuous 
    std::vector<float> fragmentSizeList; // Is the equal Fragment size == false --> this this is used 
    std::vector<std::vector<float>> nodeMap;

    // Ecological settings
    bool fragmented = false; // Run with or without fragmentation
    bool neutralComp = true;

    float searchArea = 100; // ZOI
    float b1 = 0.01;
    float b2 = 7;
    float m = 0.03; // Immigration rate
    float dispersalDis = 20;
    float mort = 0.1; // mortality rate TODO : implement or remove 
    float HNDD = 0.1; // Default HNDD strength TODO: implement
    float CNDD = 0.1; // Default CNDD
    float extinctionRate = 0.0001;



};



#endif // !SPACE_SETTINGS


