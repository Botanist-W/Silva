
#include "dataManagement.h"
#include "pch.h"


void data::setPath(std::string& path) {
    outputDirectory = path;
};

void data::setName(params& par) {

    std::ostringstream oss;

    oss << outputDirectory
        << "/Result_"
        << "nf" << par.numFragments << "_"
        << "m" << par.m << "_"
        << "sp" << par.numSpecies << "_"
        << "td" << par.treeDensity << "_"
        << "t" << par.timeSteps << ".csv";

 

    outputFile = oss.str();

    // Write the header of the file here?? not much point is making another method for this
    std::ofstream file(outputFile);

    //  R will thank you 
    file << "Repeat" << "," << "Forest" << "," << "TimeStep" << "," << "ID" << "," << "Species" << "," << "x" << "," << "y" << "\n";

};


void data::saveResults(std::vector<observation>& result) {
    
    LOG_DEBUG("size of output result: {}", result.size());

    std::ofstream file;

    file.open(outputFile, std::ios_base::app);

    if (!file.is_open()) {
        LOG_ERROR("Failed to open OUTPUT file for writing");
        return;
    }

    for (const auto& obs : result) {
        file << obs.repeat << ","
            << obs.forest << ","
            << obs.timeStep << ","
            << obs.uniqueID << ","
            << obs.species << ","
            << obs.x << ","
            << obs.y << "\n";
    }
   

};


std::vector<value> data::getSample(int id) {
    std::vector<value> result;
    try{
        io::CSVReader<7> in("C:/dev/test.csv");
        in.read_header(io::ignore_extra_column, "x", "y", "uniqueID", "species", "dispersal", "HNDD", "CNDD");

        float x, y;
        int uniqueID, species;
        float dispersal, HNDD, CNDD;

        while (in.read_row(x, y, uniqueID, species, dispersal, HNDD, CNDD)) {
            point p(x, y);
            indiv ind{ uniqueID, species, dispersal, HNDD, CNDD };
            result.emplace_back(p, ind);
        }
    }
    catch (const io::error::can_not_open_file& e) {
        LOG_ERROR("Cannot open file: ");
    }
    catch (const io::error::header_missing& e) {
        LOG_ERROR("Header missing in file: ");
    }

    return result;

};