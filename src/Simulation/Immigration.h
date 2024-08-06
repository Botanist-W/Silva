#pragma once

#include "settings.h"
#include "randFuncs.h"
#include "RTREE.h"
#include "Forest.h"
#include "Log.h"
#include <memory>
#include <vector>
#include <random>
#include <numeric>
#include <iostream>

class Forest; // Forward declaration just in case 

#ifndef IMMIGRATION
#define IMMIGRATION

class Immigration {
public:

    Immigration(params& _params) : mParams(_params) {}

    virtual void handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) = 0;

    virtual bool mOccurence(int ID, int step) = 0;

    virtual void buildMetaCom(std::vector<indiv> spLib) = 0;

protected:
    params& mParams;

    std::bernoulli_distribution mDist{ mParams.m };

};




class metaImmigration : public Immigration {
public:

    metaImmigration(params& _params);

    void handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) override;

    bool mOccurence(int ID, int step) override;

    void buildMetaCom(std::vector<indiv> spLib) override;

private:

    bool doesImmigrationOccur = false;

    std::vector<indiv> metaCom;

};





class networkImmigration : public Immigration {
public:

    networkImmigration(params& _params);

    void handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) override;

    bool mOccurence(int ID, int step) override;

    void buildMetaCom(std::vector<indiv> spLib) override;


private:
    std::vector<std::vector<float>> nodeWeights;
    std::mt19937 gen; // I wanted to keep random number generation together but this is actually way better but it's too late now 


    // Keeping just in case but TODO: remove
    void printNodes(const std::vector<std::vector<float>>& nodes); 
    void printImmigrationMap();
};

#endif // IMMIGRATION
