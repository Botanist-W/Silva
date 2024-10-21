#include "Immigration.h"
#include "pch.h"

// Meta-community immigration

metaImmigration::metaImmigration(params& _params) : Immigration(_params) {
    LOG_INFO("Using meta-community immigration"); 
}

void metaImmigration::buildMetaCom(std::vector<indiv> spLib) {
    metaCom.reserve(mParams.metaComSize); // good job 
    for (size_t i = 0; i < mParams.metaComSize; i++) { // Woww using size_t very cool
        metaCom.emplace_back(spLib[Crand::rand_int(0, int(spLib.size() - 1))]);
    }
    LOG_TRACE("Built meta-community {}", metaCom.size());
}


void metaImmigration::handleImmigration(std::vector<std::shared_ptr<Forest>> forests) {


    auto i = Crand::rand_int(0, forests.size() - 1);// Choosing a random forest 

    if (mDist(gen)) {
        forests[i]->removeTree(forests[i]->randomTree());
        forests[i]->addTree(value(point(Crand::rand_double(0, forests[i]->bounds), Crand::rand_double(0, forests[i]->bounds)), metaCom[Crand::rand_int(0, int(metaCom.size() - 1))]));
        doesImmigrationOccur = true;
        LOG_TRACE("Immigration from the meta community has Occured");
    }
    else {
        doesImmigrationOccur = false;
    }
    
};

bool metaImmigration::mOccurence(size_t ID) {
    return doesImmigrationOccur;
}


// ~ Meta-community immigration


//--------------------------------------------------------------------------------------------------------------------------------------- Wow a sepertator fancy?


// Meta-community immigration w/ competition

metaImmigrationComp::metaImmigrationComp(params& _params) : Immigration(_params) {
    LOG_INFO("Using meta-community immigration with competition");
}

void metaImmigrationComp::buildMetaCom(std::vector<indiv> spLib) {
    metaCom.reserve(mParams.metaComSize); // good job 
    for (size_t i = 0; i < mParams.metaComSize; i++) { // Woww using size_t very cool
        metaCom.emplace_back(spLib[Crand::rand_int(0, int(spLib.size() - 1))]);
    }
    LOG_TRACE("Built meta-community {}", metaCom.size());
}


void metaImmigrationComp::handleImmigration(std::vector<std::shared_ptr<Forest>> forests) {
   

    auto reciver = Crand::rand_int(0, forests.size() - 1);// Choosing a random forest to immigrate to 

    int attempt = 0;

    bool successfulM = false;

    if (mDist(gen)) {

        while (!successfulM) {

            indiv immigrant = metaCom[Crand::rand_int(0, int(metaCom.size() - 1))];

            successfulM = forests[reciver]->doCompetition(immigrant);
            attempt++;
            if (attempt > 1000) {
                LOG_INFO("Max attempts reached in trying immigration, doing local step");
                forests[reciver]->localStep();
            }


        }

        doesImmigrationOccur = true;
        LOG_TRACE("Immigration from a meta community has Occured");
    }
    else {
        doesImmigrationOccur = false;
    }
    
};

bool metaImmigrationComp::mOccurence(size_t ID) {
    return doesImmigrationOccur;
}


// ~ Meta-community immigration w/ Competition


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


void networkImmigration::buildMetaCom(std::vector<indiv> spLib) { // I'm too scared to not do this 
   
}


// ~ Network immigration





//--------------------------------------------------------------------------------------------------------------------------------------- Wow a sepertator fancy?




// Network immigration w/ meta

networkImmigrationMeta::networkImmigrationMeta(params& _params) : Immigration(_params), gen(std::random_device{}()) {
    nodeWeights = _params.nodeMap;

    for (size_t i = 0; i < mParams.numFragments; i++)
        mOccurrenceMap[i] = false;

    LOG_INFO("Using network immigration with a meta community");
}


void networkImmigrationMeta::handleImmigration(std::vector<std::shared_ptr<Forest>> forests) {

    auto i = Crand::rand_int(0, forests.size() - 1);// Choosing a random forest 

    //LOG_INFO("Meta immigration m rate: {} and metacom size{}", mParams.mort, metaCom.size());
    if (metamDist(gen)) {
        LOG_TRACE("Immigration from meta community");

        forests[i]->removeTree(forests[i]->randomTree());
        forests[i]->addTree(value(point(Crand::rand_double(0, forests[i]->bounds), Crand::rand_double(0, forests[i]->bounds)), metaCom[Crand::rand_int(0, int(metaCom.size() - 1))]));
        mOccurrenceMap[i] = true;
    }
    else {

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
        mOccurrenceMap[i] = false;
    }
}


bool networkImmigrationMeta::mOccurence(size_t ID) {
    return mOccurrenceMap[ID];
}


void networkImmigrationMeta::buildMetaCom(std::vector<indiv> spLib) { // I'm too scared to not do this 
    metaCom.reserve(mParams.metaComSize); // good job 
    for (size_t i = 0; i < mParams.metaComSize; i++) { // Woww using size_t very cool
        metaCom.emplace_back(spLib[Crand::rand_int(0, int(spLib.size() - 1))]);
    }
    LOG_ERROR("Built meta-community {}", metaCom.size());
}


// ~ Network immigration w/ meta 




//--------------------------------------------------------------------------------------------------------------------------------------- 




// Network immigration w/ COMP

networkImmigrationComp::networkImmigrationComp(params& _params) : Immigration(_params), gen(std::random_device{}()) {
    nodeWeights = _params.nodeMap;

    for (size_t i = 0; i < mParams.numFragments; i++)
        mOccurrenceMap[i] = false;

    LOG_INFO("Using network immigration with competition");
}


void networkImmigrationComp::handleImmigration(std::vector<std::shared_ptr<Forest>> forests) {
    
    bool successfulM = false;

    auto reciver = Crand::rand_int(0, forests.size() - 1);// Choosing a random forest to immigrate to 

    int attempt = 0;

    if (mDist(gen)) { // Does immigration occur?

       
        std::discrete_distribution<int> dist(nodeWeights[reciver].begin(), nodeWeights[reciver].end());

        int donor = dist(gen); // Selecting random tree for immigrant to come from 

        value immigrant = forests[donor]->randomTree();

        while (!successfulM) {
            successfulM = forests[reciver]->doCompetition(immigrant.second);
            attempt++;
            if (attempt > 1000) {
                LOG_INFO("Max attempts reached in trying immigration, doing local step");
                forests[reciver]->localStep();
            }

                
        }
      
        LOG_TRACE("Immigration occurred from fragment: {} to {}", donor, reciver);


        mOccurrenceMap[reciver] = true;
    }
    else {
        mOccurrenceMap[reciver] = false;
    }
   
    
}


bool networkImmigrationComp::mOccurence(size_t ID) {
    return mOccurrenceMap[ID];
}

void networkImmigrationComp::buildMetaCom(std::vector<indiv> spLib) { // I'm too scared to not do this 

}

// ~ Network immigration w/ Competition



//--------------------------------------------------------------------------------------------------------------------------------------- 



// Network immigration w/ Competition & meta community

networkImmigrationCompMeta::networkImmigrationCompMeta(params& _params) : Immigration(_params), gen(std::random_device{}()) {
    nodeWeights = _params.nodeMap;

    for (size_t i = 0; i < mParams.numFragments; i++)
        mOccurrenceMap[i] = false;

    LOG_INFO("Using network immigration with competition and a meta community");
}


void networkImmigrationCompMeta::handleImmigration(std::vector<std::shared_ptr<Forest>> forests) {

    auto reciver = Crand::rand_int(0, forests.size() - 1);// Choosing a random forest 
    int attempt = 0;
    bool successfulM = false;

    //LOG_INFO("Meta immigration m rate: {} and metacom size{}", mParams.mort, metaCom.size());
    if (metamDist(gen)) {



        LOG_TRACE("Immigration from meta community");

        forests[reciver]->removeTree(forests[reciver]->randomTree());
        forests[reciver]->addTree(value(point(Crand::rand_double(0, forests[reciver]->bounds), Crand::rand_double(0, forests[reciver]->bounds)), metaCom[Crand::rand_int(0, int(metaCom.size() - 1))]));
        mOccurrenceMap[reciver] = true;
    }
    else {

        if (mDist(gen)) { // Does immigration occur?


            std::discrete_distribution<int> dist(nodeWeights[reciver].begin(), nodeWeights[reciver].end());

            int donor = dist(gen); // Selecting random tree for immigrant to come from 

            value immigrant = forests[donor]->randomTree();

            while (!successfulM) {
                successfulM = forests[reciver]->doCompetition(immigrant.second);
                attempt++;
                if (attempt > 1000) {
                    LOG_INFO("Max attempts reached in trying immigration, doing local step");
                    forests[reciver]->localStep();
                }


            }

            LOG_TRACE("Immigration occurred from fragment: {} to {}", donor, reciver);


            mOccurrenceMap[reciver] = true;
        }
        else {
            mOccurrenceMap[reciver] = false;
        }
     
    }
}


bool networkImmigrationCompMeta::mOccurence(size_t ID) {
    return mOccurrenceMap[ID];
}

void networkImmigrationCompMeta::buildMetaCom(std::vector<indiv> spLib) { // I'm too scared to not do this 
    metaCom.reserve(mParams.metaComSize); // good job 
    for (size_t i = 0; i < mParams.metaComSize; i++) { // Woww using size_t very cool
        metaCom.emplace_back(spLib[Crand::rand_int(0, int(spLib.size() - 1))]);
    }
    LOG_TRACE("Built meta-community {}", metaCom.size());
}


// ~ Network immigration w/ Competition & meta community



//--------------------------------------------------------------------------------------------------------------------------------------- 



// No immigration

noImmigration::noImmigration(params& _params) : Immigration(_params) {
    LOG_INFO("No immigration here");

};

// Just a class that does nothing if I forgot or purposly dont want have any immigration

void noImmigration::handleImmigration(std::vector<std::shared_ptr<Forest>> forests) {
}

bool noImmigration::mOccurence(size_t ID) {
    return false;
}

void noImmigration::buildMetaCom(std::vector<indiv> spLib) {
}

// ~ No immigration 

