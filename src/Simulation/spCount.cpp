#include "spCount.h"
#include "pch.h"

speciesCount::speciesCount(Forest& _forest) : mForest(_forest) {
    nSpVec = firstCount();
    totalSpVec.push_back(firstRichness());
}

// Function to add and remove the respective species from the vector 
void speciesCount::countMod(int spToRemove, int spToAdd, int timeStep, int forestID, int repeat) {

    int& spToRemoveCount = nSpVec[spToRemove];
    int& spToAddCount = nSpVec[spToAdd];

    spToRemoveCount--;
    spToAddCount++;

    int newAbundance = totalSpVec.back();
    if (spToRemoveCount == 0) {
        newAbundance--;
    }

    totalSpVec.push_back(newAbundance);
    timeStepVec.push_back(timeStep);
    spCountList.emplace_back(repeat, forestID, timeStep, newAbundance);

}

// Function to first count all the species in the rTree
std::vector<int> speciesCount::firstCount() {
    

    std::vector<int> speciesCounts(mForest.numSpecies, 0);

    for (const auto& element : mForest.tree) {
        int species = element.second.species;
        //LOG_WARN("species: {} ", species);
        speciesCounts[species]++;
    }
    LOG_DEBUG("first count length: {}", speciesCounts.size());
    return speciesCounts;
    
}

// Function to find the total species abundance 
int speciesCount::firstRichness() {
    int speciesAbundance = 0;

    for (int count : nSpVec) {
        if (count > 0) {
            speciesAbundance++;
        }
    }

    LOG_DEBUG("Initial richness: {}", speciesAbundance);

    if (speciesAbundance < 1)
        LOG_ERROR("Error in first sp count");

    return speciesAbundance + 1;
}
