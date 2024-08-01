
#include "dataManagement.h"
#include "pch.h"


void data::setPath(std::string& path) {

};

void data::setName(std::string& name) {

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