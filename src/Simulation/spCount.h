#pragma once

#include "Forest.h"
#include <vector>
#include <unordered_map>


#ifndef SP_COUNT
#define SP_COUNT
class Forest;



class speciesCount {
public:
    speciesCount(Forest& _forest);

    void countMod(int spToRemove, int spToAdd, int timeStep, int forestID, int repeat);

    std::vector<std::tuple<int, int, int, int>> spCountList;

private:
    Forest& mForest;
    std::unordered_map<int, int> nSpVec; // Look at me, using all the cool coding stuff
    std::vector<int> totalSpVec;
    std::vector<int> timeStepVec;
    
    int captureRate;
    int stepCounter;

    std::unordered_map<int, int> firstCount(); 
    int firstRichness();
};



#endif // !SP_COUNT