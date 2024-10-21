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

    virtual void handleImmigration(std::vector<std::shared_ptr<Forest>> forests) = 0;

    virtual bool mOccurence(size_t ID) = 0;

    virtual void buildMetaCom(std::vector<indiv> spLib) = 0;

protected:

    params& mParams;

   

};

class metaImmigration : public Immigration {
public:

    metaImmigration(params& _params);

    void handleImmigration(std::vector<std::shared_ptr<Forest>> forests) override;

    bool mOccurence(size_t ID) override;

    void buildMetaCom(std::vector<indiv> spLib) override;

private:

    bool doesImmigrationOccur = false;

    std::vector<indiv> metaCom;

    std::bernoulli_distribution mDist{ mParams.m };
};

class metaImmigrationComp : public Immigration{
public:

    metaImmigrationComp(params& _params);

    void handleImmigration(std::vector<std::shared_ptr<Forest>> forests) override;

    bool mOccurence(size_t ID) override;

    void buildMetaCom(std::vector<indiv> spLib) override;

private:

    bool doesImmigrationOccur = false;

    std::vector<indiv> metaCom;

    std::bernoulli_distribution mDist{ mParams.m };
}; // TODO

class networkImmigration : public Immigration {
public:

    networkImmigration(params& _params);

    void handleImmigration(std::vector<std::shared_ptr<Forest>> forests) override;

    bool mOccurence(size_t ID) override;

    void buildMetaCom(std::vector<indiv> spLib) override;


private:
    std::map<size_t, bool> mOccurrenceMap;
    std::vector<std::vector<double>> nodeWeights;
    std::mt19937 gen; // I wanted to keep random number generation together but this is actually way better but it's too late now 
    std::bernoulli_distribution mDist{ mParams.m}; // time the number of fragmnets because immigration chance should be set per fragment

};

class networkImmigrationMeta : public Immigration {
public:

    networkImmigrationMeta(params& _params);

    void handleImmigration(std::vector<std::shared_ptr<Forest>> forests) override;

    bool mOccurence(size_t ID) override;

    void buildMetaCom(std::vector<indiv> spLib) override;


private:
    std::map<size_t, bool> mOccurrenceMap;
    std::vector<std::vector<double>> nodeWeights;
    std::mt19937 gen; // I wanted to keep random number generation together but this is actually way better but it's too late now 
    std::bernoulli_distribution mDist{ mParams.m }; // time the number of fragmnets because immigration chance should be set per fragment
    std::bernoulli_distribution metamDist{ mParams.mort }; // Highjacking mort because it isnt being used as of now 
    std::vector<indiv> metaCom;
};

class networkImmigrationComp : public Immigration {
public:

    networkImmigrationComp(params& _params);

    void handleImmigration(std::vector<std::shared_ptr<Forest>> forests) override;

    bool mOccurence(size_t ID) override;

    void buildMetaCom(std::vector<indiv> spLib) override;

private:
    std::map<size_t, bool> mOccurrenceMap;
    std::vector<std::vector<double>> nodeWeights;
    std::mt19937 gen; // I wanted to keep random number generation together but this is actually way better but it's too late now 
    std::bernoulli_distribution mDist{ mParams.m }; // time the number of fragmnets because immigration chance should be set per fragment

   
};

class networkImmigrationCompMeta : public Immigration {
public:

    networkImmigrationCompMeta(params& _params);

    void handleImmigration(std::vector<std::shared_ptr<Forest>> forests) override;

    bool mOccurence(size_t ID) override;

    void buildMetaCom(std::vector<indiv> spLib) override;


private:
    std::map<size_t, bool> mOccurrenceMap;
    std::vector<std::vector<double>> nodeWeights;
    std::mt19937 gen; // I wanted to keep random number generation together but this is actually way better but it's too late now 
    std::bernoulli_distribution mDist{ mParams.m }; // time the number of fragmnets because immigration chance should be set per fragment
    std::bernoulli_distribution metamDist{ mParams.mort }; // Highjacking mort because it isnt being used as of now 
    std::vector<indiv> metaCom;
};

class noImmigration : public Immigration {
public:

    noImmigration(params& _params);

    void handleImmigration(std::vector<std::shared_ptr<Forest>> forests) override;

    bool mOccurence(size_t step) override;

    void buildMetaCom(std::vector<indiv> spLib) override;

};

#endif // IMMIGRATION
