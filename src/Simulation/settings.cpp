#include "settings.h"
#include "pch.h"

void settings::set(const params& par) {
    
    mParams = par;

};

params settings::get() {

    return mParams;

};

void settings::save(std::string& directory, const params& par) {
    settingsPath = directory;
    generateDirectory();
    mParams = par;
    saveParams();
    saveNodeMap();
    saveSizeList();
}

params settings::load(std::string& directory) {
    settingsDirectory = directory;
    loadParams();
    loadNodeMap();
    loadSizeList();
    return mParams;
}

void settings::generateDirectory() {

    std::ostringstream oss;
    oss << settingsPath
        << "Settings_"
        << "nf" << mParams.numFragments << "_"
        << "m" << mParams.m << "_"
        << "sp" << mParams.numSpecies << "_"
        << "td" << mParams.treeDensity << "_"
        << "t" << mParams.timeSteps;

    std::filesystem::create_directories(oss.str());

    settingsDirectory = oss.str();

    //LOG_INFO("File directory: {}", settingsDirectory);

}

// Parameters
void settings::saveParams() {

    std::ofstream file(settingsDirectory + "/params.csv");


    if (!file.is_open()) {
        //LOG_ERROR("Failed to open file for writing: {}", file);
        return;
    }

    // Write headers
    file << "Parameter,Value\n";

    // Write simulation settings
    file << "timeSteps," << mParams.timeSteps << "\n";
    file << "numRep," << mParams.numRep << "\n";
    file << "captureRate," << mParams.captureRate << "\n";
    file << "numSpecies," << mParams.numSpecies << "\n";
    file << "treeDensity," << mParams.treeDensity << "\n";

    // Write immigration settings
    file << "metaCommunityImmigration," << mParams.metaCommunityImmigration << "\n";
    file << "metaComSize," << mParams.metaComSize << "\n";

    // Write fragment interactions
    file << "numFragments," << mParams.numFragments << "\n";
    file << "equalFragmentSize," << mParams.equalFragmentSize << "\n";
    file << "size," << mParams.size << "\n";

    // Write ecological settings
    file << "fragmented," << mParams.fragmented << "\n";
    file << "neutralComp," << mParams.neutralComp << "\n";
    file << "searchArea," << mParams.searchArea << "\n";
    file << "b1," << mParams.b1 << "\n";
    file << "b2," << mParams.b2 << "\n";
    file << "m," << mParams.m << "\n";
    file << "dispersalDis," << mParams.dispersalDis << "\n";
    file << "mort," << mParams.mort << "\n";
    file << "HNDD," << mParams.HNDD << "\n";
    file << "CNDD," << mParams.CNDD << "\n";
    file << "extinctionRate," << mParams.extinctionRate << "\n";

    file.close();
    //LOG_INFO("Settings saved to: {}", settingsDirectory);
}

void settings::loadParams() { // Super inefficient but just being safe I guess TOTO: use csv.h

    std::ifstream file(
        settingsDirectory + "/params.csv");

    if (!file.is_open()) {
        LOG_ERROR("Failed to open file for writing:");
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string parameter, value;
        std::getline(iss, parameter, ',');
        std::getline(iss, value, ',');

        if (parameter == "timeSteps") {
            mParams.timeSteps = std::stoi(value);
        }
        else if (parameter == "numRep") {
            mParams.numRep = std::stoi(value);
        }
        else if (parameter == "captureRate") {
            mParams.captureRate = std::stoi(value);
        }
        else if (parameter == "numSpecies") {
            mParams.numSpecies = std::stoi(value);
        }
        else if (parameter == "treeDensity") {
            mParams.treeDensity = std::stof(value);
        }
        else if (parameter == "metaCommunityImmigration") {
            mParams.metaCommunityImmigration = (value == "1");
        }
        else if (parameter == "metaComSize") {
            mParams.metaComSize = std::stoi(value);
        }
        else if (parameter == "numFragments") {
            mParams.numFragments = std::stoi(value);
        }
        else if (parameter == "equalFragmentSize") {
            mParams.equalFragmentSize = (value == "1");
        }
        else if (parameter == "size") {
            mParams.size = std::stof(value);
        }
        else if (parameter == "fragmented") {
            mParams.fragmented = (value == "1");
        }
        else if (parameter == "neutralComp") {
            mParams.neutralComp = (value == "1");
        }
        else if (parameter == "searchArea") {
            mParams.searchArea = std::stof(value);
        }
        else if (parameter == "b1") {
            mParams.b1 = std::stof(value);
        }
        else if (parameter == "b2") {
            mParams.b2 = std::stof(value);
        }
        else if (parameter == "m") {
            mParams.m = std::stof(value);
        }
        else if (parameter == "dispersalDis") {
            mParams.dispersalDis = std::stof(value);
        }
        else if (parameter == "mort") {
            mParams.mort = std::stof(value);
        }
        else if (parameter == "HNDD") {
            mParams.HNDD = std::stof(value);
        }
        else if (parameter == "CNDD") {
            mParams.CNDD = std::stof(value);
        }
        else if (parameter == "extinctionRate") {
            mParams.extinctionRate = std::stof(value);
        }
        else {
            std::cerr << "Unknown parameter: " << parameter << std::endl;
        }
    }

    file.close();
  

}
// ~ Parameters

// Node Map 
void settings::saveNodeMap() {

    std::ofstream file(settingsDirectory + "/nodeMap.csv");

    if (!file.is_open()) {
       LOG_ERROR("Failed to open node map file for writing");
        return;
    }

    ResizeMatrix(mParams.numFragments);

    for (int i = 0; i < mParams.numFragments; i++) {

        for (int j = 0; j < mParams.numFragments; j++) {

            file << mParams.nodeMap[i][j] << ",";

        }

        file << "\n";
    }

    file.close();

}

void settings::loadNodeMap() { // TODO : use csv.h

    std::ifstream file(settingsDirectory + "/nodeMap.csv");

    if (!file.is_open()) {
       // LOG_ERROR("Failed to open file for writing: {}", file);
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        std::vector<float> row;

        while (std::getline(ss, value, ',')) {

            row.push_back(std::stof(value)); // FLOAT == unbalanced immigration 

        }
        mParams.nodeMap.push_back(row);
    }

    // Close the file after reading
    file.close();

};
// ~ Node Map

// Size list
void settings::saveSizeList() {
    std::ofstream file(settingsDirectory + "/sizeList.csv");

    if (!file.is_open()) {
       LOG_ERROR("Failed to open size list file for writing");
        return;
    }

    for (int i = 0; i < mParams.numFragments; i++) {

        file << i << "," << mParams.fragmentSizeList[i] << "\n";

    }

    file.close();

}

void settings::loadSizeList() {

    std::string file(settingsDirectory + "/sizeList.csv");

    try {
        io::CSVReader<2> in(file);
        int fragment;
        float size;

        while (in.read_row(fragment, size)) {
            
            mParams.fragmentSizeList.emplace_back(size); // TODO: USE a map, seriously save some time 
        }
    }
    catch (const io::error::can_not_open_file& e) {
        LOG_ERROR("Cannot open file: ");
    }
    catch (const io::error::header_missing& e) {
        LOG_ERROR("Header missing in file: ");
    }

    LOG_DEBUG("has size list worked? size: {}", mParams.fragmentSizeList.size());
    LOG_DEBUG("First fragment: {}", mParams.fragmentSizeList[0]);
}

// Helper functions 
void settings::ResizeMatrix(int newSize) {
    mParams.nodeMap.resize(newSize);
    for (auto& row : mParams.nodeMap) {
        row.resize(newSize, 0.0f);
    }
}


