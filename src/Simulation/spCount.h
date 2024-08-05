#pragma once

#include "Forest.h"
#include <vector>



#ifndef SP_COUNT
#define SP_COUNT
class Forest;


class speciesCount {
public:

    // Terrible, terrible memory management, but need to get this done!!!!
    std::vector<std::tuple<int, int, int, int>> spCountList = {};
  


    speciesCount(Forest& _forest);


    // Funtion to add and remove the respective species from the vector 
    void countMod(int spToRemove, int spToAdd, int timeStep, int forestID, int repeat);

  


private:

    Forest& mForest;

    int speicesRichness = 0;
    std::vector<int> nSpVec = {};
    std::vector<int> totalSpVec = {};
    std::vector<int> timeStepVec = {};
    std::vector<std::vector<int>> outSpVec = {};


    // Funtion to first count all the species in the rTree
    std::vector<int> firstCount();

    // Funtion to find the total species abundance 
    int firstRichness();

};



#endif // !SP_COUNT