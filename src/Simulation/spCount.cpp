#include "spCount.h"
#include "pch.h"
speciesCount::speciesCount(Forest& _forest) : mForest(_forest) {
    nSpVec = firstCount();
    totalSpVec.push_back(firstRichness());
    captureRate = mForest.timeSteps / 100;
    LOG_INFO("Capture rate: {}", captureRate);
    stepCounter = captureRate;
}

void speciesCount::countMod(int spToRemove, int spToAdd, int timeStep, int forestID, int repeat) {
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

    totalSpVec.push_back(newAbundance);
    timeStepVec.push_back(timeStep);

    if (stepCounter >= captureRate) {
        stepCounter = 0;
        int spRichness = totalSpVec.back();
        int step = timeStepVec.back();
        totalSpVec.clear();
        timeStepVec.clear();
        totalSpVec.push_back(spRichness);
        timeStepVec.push_back(step);
        spCountList.emplace_back(repeat, forestID, timeStep, newAbundance);
        LOG_INFO("TimeStep: {}", timeStepVec.back());
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