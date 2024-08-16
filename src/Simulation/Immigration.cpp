#include "Immigration.h"
#include "pch.h"

// Meta-community immigration

metaImmigration::metaImmigration(params& _params) : Immigration(_params) {
    LOG_DEBUG("Using meta-community immigration"); 
}

void metaImmigration::buildMetaCom(std::vector<indiv> spLib) {
    metaCom.reserve(mParams.metaComSize); // good job 
    for (size_t i = 0; i < mParams.metaComSize; i++) { // Woww using size_t very cool
        metaCom.emplace_back(spLib[Crand::rand_int(0, int(spLib.size() - 1))]);
    }
    LOG_TRACE("Built meta-community {}", metaCom.size());
}

void networkImmigration::buildMetaCom(std::vector<indiv> spLib) { // I'm too scared to not do this 
}


void metaImmigration::handleImmigration(std::vector<std::shared_ptr<Forest>> forests) {
    for (size_t i = 0; i < forests.size(); i++) {
        if (mDist(gen)) {
            forests[i]->removeTree(forests[i]->randomTree());
            forests[i]->addTree(value(point(Crand::rand_double(0, forests[i]->bounds), Crand::rand_double(0, forests[i]->bounds)), metaCom[Crand::rand_int(0, int(metaCom.size() - 1))]));
            doesImmigrationOccur = true;
            LOG_TRACE("Immigration has Occured");
        }
        else {
            doesImmigrationOccur = false;
        }
    }
};

bool metaImmigration::mOccurence(size_t ID) {
    return doesImmigrationOccur;
}


// ~ Meta-community immigration


//--------------------------------------------------------------------------------------------------------------------------------------- Wow a sepertator fancy?


// Network immigration

networkImmigration::networkImmigration(params& _params) : Immigration(_params), gen(std::random_device{}()) {
    nodeWeights = _params.nodeMap;

    for (size_t i = 0; i < mParams.numFragments; i++)
        mOccurrenceMap[i] = false;

    LOG_INFO("Using network immigration");
}


void networkImmigration::handleImmigration(std::vector<std::shared_ptr<Forest>> forests) {
    
    auto i = Crand::rand_int(0, forests.size() - 1);// Choosing a random forest 

    if (mDist(gen)) { // Does immigration occur?

        mOccurrenceMap[i] = true;

        forests[i]->removeTree(forests[i]->randomTree());

        std::discrete_distribution<int> dist(nodeWeights[i].begin(), nodeWeights[i].end());
        int targetForestIndex = dist(gen); // Selecting random tree for immigrant to come from 

        forests[i]->addTree(forests[targetForestIndex]->randomTree());

        LOG_TRACE("Immigration occurred from fragment: {} to {}", i, targetForestIndex);
    }
    else {
        mOccurrenceMap[i] = false;
    }
    
}


bool networkImmigration::mOccurence(size_t ID) {
    return mOccurrenceMap[ID];
}


// ~ Network immigration




// No immigration

noImmigration::noImmigration(params& _params) : Immigration(_params) {
    LOG_INFO("No immigration here");

};

// Just a class that does nothing if I forgot or purposly dont want have any immigration

void noImmigration::handleImmigration( std::vector<std::shared_ptr<Forest>> forests) {
}

bool noImmigration::mOccurence(size_t ID) {
    return false; 
}

void noImmigration::buildMetaCom(std::vector<indiv> spLib) {
}

// ~ No immigration 





