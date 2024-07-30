#include "settings.h"

void settings::generateDirectory() {

    std::ostringstream oss;
    oss << settingsPath
        << "Settings_"
        << "nf" << par.numFragments << "_"
        << "m" << par.m << "_"
        << "sp" << par.numSpecies << "_"
        << "td" << par.treeDensity << "_"
        << "t" << par.timeSteps;

    std::filesystem::create_directories(oss.str());

    settingsDirectory = oss.str();

}

// Parameters
void settings::saveParams() {

    std::ofstream file(settingsDirectory + "/params.csv");


    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << std::endl;
        return;
    }

    // Write headers
    file << "Parameter,Value\n";

    // Write simulation settings
    file << "timeSteps," << par.timeSteps << "\n";
    file << "numRep," << par.numRep << "\n";
    file << "numSpecies," << par.numSpecies << "\n";
    file << "treeDensity," << par.treeDensity << "\n";

    // Write immigration settings
    file << "metaCommunityImmigration," << par.metaCommunityImmigration << "\n";
    file << "metaComSize," << par.metaComSize << "\n";

    // Write fragment interactions
    file << "numFragments," << par.numFragments << "\n";
    file << "equalFragmentSize," << par.equalFragmentSize << "\n";
    file << "size," << par.size << "\n";

    // Write ecological settings
    file << "fragmented," << par.fragmented << "\n";
    file << "neutralComp," << par.neutralComp << "\n";
    file << "searchArea," << par.searchArea << "\n";
    file << "b1," << par.b1 << "\n";
    file << "b2," << par.b2 << "\n";
    file << "m," << par.m << "\n";
    file << "dispersalDis," << par.dispersalDis << "\n";
    file << "mort," << par.mort << "\n";
    file << "HNDD," << par.HNDD << "\n";
    file << "CNDD," << par.CNDD << "\n";
    file << "extinctionRate," << par.extinctionRate << "\n";

    file.close();
    std::cout << "Settings saved to " << std::endl;
}

void settings::loadParams() { // Super inefficient but just being safe I guess

    std::ifstream file(
        settingsDirectory + "/params.csv");

    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << settingsDirectory << std::endl;
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
    std::cout << "Settings loaded from " << settingsDirectory << std::endl;
}
// ~ Parameters

// Node Map 
void settings::saveNodeMap() {

    std::ofstream file(settingsDirectory + "/nodeMap.csv");

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << settingsDirectory << std::endl;
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

void settings::loadNodeMap() {

    std::ifstream file(settingsDirectory + "/nodeMap.csv");

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << settingsDirectory << std::endl;
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
        std::cerr << "Failed to open file for writing: " << std::endl;
        return;
    }

    for (int i = 0; i < mParams.numFragments; i++) {

        file << i << "," << mParams.fragmentSizeList[i] << "\n";

    }

    file.close();

}

void settings::loadSizeList() {

    std::ifstream file(settingsDirectory + "/sizeList.csv");

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << std::endl;
        return;
    }

    mParams.fragmentSizeList.resize(mParams.numFragments);

    std::string line;
    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string value, fragment;
        std::getline(ss, fragment, ',');
        std::getline(ss, value, ',');


        mParams.fragmentSizeList.push_back(std::stof(value));

    }

    file.close();


}
