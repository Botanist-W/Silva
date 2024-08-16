
#ifndef SPACE_SETTINGS
#define SPACE_SETTINGS

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <csv.h>

struct params { // 

    // Simulation Settings
    int timeSteps = 10000;
    int captureRate = 1000;
    int numRep = 10;  //number replicates
    bool buildFromSample = false;

    int numSpecies = 300; // number of species  
    double treeDensity = 0.014888; // Number of trees per area or lambda for pois()

    // Immigration settings
    bool metaCommunityImmigration = false;
    int metaComSize = 5000;

    // Fragment interactions etc
    int numFragments = 3;
    bool equalFragmentSize = true;
    double size = 100; // L ... Size of simulation Usefull for continuous 
    std::vector<double> fragmentSizeList; // Is the equal Fragment size == false --> this this is used 
    std::vector<std::vector<double>> nodeMap; // TODO: improve memeory useage << or decide if I need to because it's not used too much

    // Ecological settings
    bool fragmented = false; // Run with or without fragmentation
    bool neutralComp = true;

    double searchArea = 20; // ZOI
    double b1 = 7;
    double b2 = 6;
    double m = 0.001; // Immigration rate
    double dispersalDis = 20;
    double mort = 0.1; // mortality rate TODO : implement or remove 
    double HNDD = 9; // Default HNDD strength TODO: implement
    double CNDD = 9; // Default CNDD
    double extinctionRate = 0.0001;


    std::string sampleDirectory = "../../Samples";

};


class settings {
public:

    settings() {};

    std::string settingsPath = ""; // Only used by the generate directory method
    std::string settingsDirectory = "/Params"; // TODO: be able to iterate settings

    void generateDirectory(); // Generates the folder name where the files will hopefully be 

    void set(const params&); // When in code 
    params get();

    void save(std::string&, const params&); // When files r invlolved 
    params load(std::string&);

    void saveParams();
    void loadParams();

    void saveNodeMap();
    void loadNodeMap();
    
    void saveSizeList();
    void loadSizeList();
    

private: 

    params mParams;

    void ResizeMatrix(int newSize);

    void logger();
};

#endif // !SPACE_SETTINGS


