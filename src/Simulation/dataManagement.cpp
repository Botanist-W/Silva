#include "dataManagement.h"
#include "pch.h"

void data::setPath(const std::string& path) {
    outputDirectory = path;
}

void data::setName(params& par) {
    setCountName(par);
    setCaptureName(par);
}

void data::setCaptureName(params& par) {
    std::ostringstream oss;
    oss << outputDirectory << "/Result_"
        << "nf" << par.numFragments << "_"
        << "m" << par.m << "_"
        << "sp" << par.numSpecies << "_"
        << "td" << par.treeDensity << "_"
        << "t" << par.timeSteps << ".csv";
    outputFile = oss.str();

    std::ofstream file(outputFile);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open OUTPUT file for writing");
        return;
    }

    file << "Repeat,Forest,TimeStep,ID,Species,x,y\n";
}

void data::setCountName(params& par) {
    std::ostringstream oss;
    oss << outputDirectory << "/SpeciesCount_"
        << "nf" << par.numFragments << "_"
        << "m" << par.m << "_"
        << "sp" << par.numSpecies << "_"
        << "td" << par.treeDensity << "_"
        << "t" << par.timeSteps << ".csv";
    spCountOutFile = oss.str();

    std::ofstream file(spCountOutFile);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open species count file for writing");
        return;
    }

    file << "Repeat,Forest,TimeStep,SpCount\n";
    LOG_INFO("Set name of Count file");
}

void data::saveResults(const std::vector<observation>& result) {
    LOG_DEBUG("Size of output result: {}", result.size());

    std::ofstream file(outputFile, std::ios_base::app);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open OUTPUT file for writing");
        return;
    }

    for (const auto& obs : result) {
        file << obs.repeat + 1<< ","
            << obs.forest + 1<< ","
            << obs.timeStep << ","
            << obs.uniqueID << ","
            << obs.species << ","
            << obs.x << ","
            << obs.y << "\n";
    }
}

void data::setSampleDirectory(params& par, const std::string& path) {
    par.sampleDirectory = path;
    LOG_INFO("Loaded sample directory");
}

std::vector<value> data::getSample(const std::string& directory, float bounds) {
    std::ostringstream oss;
    LOG_DEBUG("loading smaple");
    int sampleIndex = Crand::rand_int(1, 1); //TODO: ADJUST

    oss << directory << "/sample_1.csv";
    std::string filename = oss.str();

    std::vector<value> result; // TODO: implement x and y with bg::box 
    try {
        io::CSVReader<7> in(filename);
        in.read_header(io::ignore_extra_column, "uniqueID", "species", "x", "y", "dispersal", "HNDD", "CNDD");

        float x, y;
        int uniqueID, species;
        float dispersal, HNDD, CNDD;

        while (in.read_row(uniqueID, species, x, y, dispersal, HNDD, CNDD)) {
            point p(x, y);
            indiv ind{ uniqueID, species, dispersal, HNDD, CNDD };
            result.emplace_back(p, ind);
        }


    }
    catch (const io::error::can_not_open_file& e) {
        LOG_ERROR("Cannot open file: {}", e.what());
    }
    catch (const io::error::header_missing& e) {
        LOG_ERROR("Header missing in file: {}", e.what());
    }

    LOG_DEBUG("Build from sample size of result: {}", result.size());
    return result;
}

void data::saveSpCount(const std::vector<std::tuple<int, int, int, int>>& result) {
    if(result.size() < 1)
        LOG_ERROR(" NOTHING IN SPEICES COUNT: Size of species count: {}", result.size());

    std::ofstream file(spCountOutFile, std::ios_base::app);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open species count file for writing");
        return;
    }

    for (const auto& obs : result) {
        file << std::get<0>(obs) + 1 << ","
            << std::get<1>(obs) + 1 << ","
            << std::get<2>(obs) << ","
            << std::get<3>(obs) << "\n";
    }
}
