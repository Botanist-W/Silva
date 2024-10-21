#include "spCount.h"
#include "pch.h"
speciesCount::speciesCount(Forest& _forest) : mForest(_forest) {
    nSpVec = firstCount();
    totalSpVec.push_back(firstRichness());
    captureRate = mForest.timeSteps / 1000;
    //LOG_INFO("Capture rate: {}", captureRate);
    stepCounter = captureRate;
}




void speciesCount::countMod(int spToRemove, int spToAdd, int timeStep, int forestID, int repeat) {
    timeStepVec.push_back(timeStep);


    if (totalSpVec.back() <= 1) {
        
        if (stepCounter >= captureRate) {
            stepCounter = 0;
            int step = timeStepVec.back();
            totalSpVec.clear();
            timeStepVec.clear();
            totalSpVec.push_back(1);
            timeStepVec.push_back(step);
            LOG_TRACE("Just one species left");
            spCountList.emplace_back(repeat, forestID, timeStep, 1);
        }
        stepCounter++;
        return;
    }

    if (spToRemove > 320) { // Quick fix but i think it works quite well
        //spCountList.emplace_back(repeat, forestID, timeStep, totalSpVec.back());

        if (stepCounter >= captureRate) {
            stepCounter = 0;
            int spRichness = totalSpVec.back();
            int step = timeStepVec.back();
            totalSpVec.clear();
            timeStepVec.clear();
            totalSpVec.push_back(spRichness);
            timeStepVec.push_back(step);
            spCountList.emplace_back(repeat, forestID, timeStep, spRichness);
            
            

            
        }
        stepCounter++;
        return;
    }

    int& spToRemoveCount = nSpVec[spToRemove];
    int& spToAddCount = nSpVec[spToAdd];

    spToRemoveCount--;
    spToAddCount++;

    int newAbundance = totalSpVec.back();
    if (spToRemoveCount == 0) {
        newAbundance--;
    }
    if (spToAddCount == 1) {
        newAbundance++;
    }
    //LOG_INFO("Species: {}", newAbundance);

    totalSpVec.push_back(newAbundance);
   

    if (stepCounter >= captureRate) { // TERRIBLE
        stepCounter = 0;
        int spRichness = totalSpVec.back();
        int step = timeStepVec.back();
        totalSpVec.clear();
        timeStepVec.clear();
        totalSpVec.push_back(spRichness);
        timeStepVec.push_back(step);
        spCountList.emplace_back(repeat, forestID, timeStep, newAbundance);
        //LOG_INFO("Species: {}", spRichness);
    }

    stepCounter++;
}

std::unordered_map<int, int> speciesCount::firstCount() {
    std::unordered_map<int, int> speciesCounts;

    for (const auto& element : mForest.tree) {
        int species = element.second.species;
        speciesCounts[species]++;
    }
    LOG_DEBUG("First count length: {}", speciesCounts.size());
    return speciesCounts;
}

int speciesCount::firstRichness() {
    int speciesAbundance = 0;
    for (const auto& count : nSpVec) {
        if (count.second > 0) {
            speciesAbundance++;
        }
    }
    LOG_DEBUG("Initial richness: {}", speciesAbundance);
    if (speciesAbundance < 1) {
        LOG_ERROR("Error in first species count");
    }
    return speciesAbundance;
}