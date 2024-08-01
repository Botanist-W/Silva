
#ifndef SPACE_SETTINGS
#define SPACE_SETTINGS

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>

struct params { // 

    // Simulation Settings
    int timeSteps = 10000;
    int captureRate = 1000;
    int numRep = 10;  //number replicates
    bool buildFromSample = false;

    int numSpecies = 300; // number of species  
    float treeDensity = 0.04; // Number of trees per area or lambda for pois()

    // Immigration settings
    bool metaCommunityImmigration = false;
    int metaComSize = 5000;

    // Fragment interactions etc
    int numFragments = 3;
    bool equalFragmentSize = true;
    float size = 100; // L ... Size of simulation Usefull for continuous 
    std::vector<float> fragmentSizeList; // Is the equal Fragment size == false --> this this is used 
    std::vector<std::vector<float>> nodeMap;

    // Ecological settings
    bool fragmented = false; // Run with or without fragmentation
    bool neutralComp = true;

    float searchArea = 20; // ZOI
    float b1 = 6;
    float b2 = 7;
    float m = 0.001; // Immigration rate
    float dispersalDis = 20;
    float mort = 0.1; // mortality rate TODO : implement or remove 
    float HNDD = 9; // Default HNDD strength TODO: implement
    float CNDD = 9; // Default CNDD
    float extinctionRate = 0.0001;



};


class settings {
public:

    settings(params& _params) : mParams(_params) {};

    // TODO: Offload to the settings class
    std::string dataOutputPath = "";
    std::string settingsPath = "";
    std::string settingsDirectory = "";


    void generateDirectory();

    void saveParams();
    void loadParams();

    void loadNodeMap();
    void saveNodeMap();

    void loadSizeList();
    void saveSizeList();

private: 

    params& mParams;

    void ResizeMatrix(int newSize);
};



#endif // !SPACE_SETTINGS


