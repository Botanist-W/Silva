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


void metaImmigration::handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) {
    for (size_t i = 0; i < forests.size(); i++) {
        if (mDist(gen)) {
            forests[i]->removeTree(forests[i]->randomTree());
            forests[i]->addTree(value(point(Crand::randFloat(0, forests[i]->bounds), Crand::randFloat(0, forests[i]->bounds)), metaCom[Crand::rand_int(0, int(metaCom.size() - 1))]));
            doesImmigrationOccur = true;
            LOG_TRACE("Immigration has Occured");
        }
        else {
            doesImmigrationOccur = false;
        }
    }
};

bool metaImmigration::mOccurence(int ID, int step) {
    return doesImmigrationOccur;
}


// ~ Meta-community immigration


//--------------------------------------------------------------------------------------------------------------------------------------- Wow a sepertator fancy?


// Network immigration

networkImmigration::networkImmigration(params& _params) : Immigration(_params), gen(std::random_device{}()) {
    nodeWeights = _params.nodeMap;
    LOG_INFO("Using network immigration");
}


void networkImmigration::handleImmigration(int timeStep, std::vector<std::shared_ptr<Forest>> forests) {
    for (size_t i = 0; i < forests.size(); i++) {
        if (mDist(gen)) { // Does immigration occur?
            forests[i]->removeTree(forests[i]->randomTree());

            std::discrete_distribution<int> dist(nodeWeights[i].begin(), nodeWeights[i].end());
            int targetForestIndex = dist(gen);

            forests[i]->addTree(forests[targetForestIndex]->randomTree());

            LOG_TRACE("Immigration occurred from fragment: {} to {}", i, targetForestIndex);
        }
    }
}


bool networkImmigration::mOccurence(int ID, int step) {
    return mDist(gen);
}


// ~ Network immigration












// Some useless crap
// 
// 
// Just printing the head
void networkImmigration::printImmigrationMap() {

    // First 20 rows 
    for (int i = 0; i < 20; i++) {

        for (int j = 0; j < temporalImmigrationMap.size(); j++) {

            std::cout << " ," << temporalImmigrationMap[j][i];

        }
        std::cout << "\n";
    }



};



void networkImmigration::printNodes(std::vector<std::vector<float>>& nodes) {

    LOG_DEBUG("Printing nodes: ");

    for (int i = 0; i < nodes.size(); i++) {

        for (int j = 0; j < nodes[i].size(); j++) {

            std::cout << " ," << nodes[i][j];

        }
        std::cout << " ,\n";
    }
    std::cout << "\n";


    LOG_DEBUG("Printing Weights");

    std::vector<int> flatten_weights;
    for (const auto& row : nodes) {
        flatten_weights.insert(flatten_weights.end(), row.begin(), row.end());
    }

    int sum_weights = std::accumulate(flatten_weights.begin(), flatten_weights.end(), 0);

    if (sum_weights <= 0) {
        LOG_ERROR("Error The sum of weights is zero or negative: {}", sum_weights);
        return;
    }
    else {
        LOG_DEBUG("Weights should work: {}", sum_weights);
    }


};