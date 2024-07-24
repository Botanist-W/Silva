#pragma once
#include "settings.h"
#include "RTREE.h"
#include <vector>
#include <iostream>
#include <iosfwd>
#include <fstream>


class speciesCount {
public:

    std::vector<int> totalSpVec;
    std::vector<int> timeStepVec;
    std::vector<std::vector<int>> outSpVec;

    std::vector<std::pair<int, value>> rtreeSnapshotVec;

    speciesCount(RTREE& tre, params& _params) : rtree(tre), mParams(_params) {
        totalSpVec.reserve(mParams.timeSteps);
        timeStepVec.reserve(mParams.timeSteps);
        rtreeSnapshotVec.reserve((mParams.numRep * mParams.timeSteps));
        nSpVec.reserve(mParams.numSpecies);

        nSpVec = firstCount();

        totalSpVec[0] = firstRichness();
    };


    // Funtion to add and remove the respective species from the vector 
    void countMod(int& spToRemove, int& spToAdd, int loop) {

        //std::cout << " \n Origonal abundance: " << vec[sp_rm] << "\n \n"; \

         // Adding that species to the vector 
        int sp_to_rm = (nSpVec[spToRemove]) - 1;

        // extra step here just to be good
        nSpVec[spToRemove] = sp_to_rm;

        int lower_than = nSpVec[spToRemove];

        //std::cout << "Species to remove" << sp_rm << "   ,new abundance: " << sp_to_rm << "\n \n";


        // adding 
        int sp_to_add = nSpVec[spToAdd] + 1;

        nSpVec[spToAdd] = sp_to_add;

        // std::cout << " Species being added: " << sp_add << "  Abundance:  " << sp_to_add << "\n \n";


         // This decreases the total number of species when there is a species abundance < 1
        if (lower_than == 0) {

            int sp_abundance_to_remove = totalSpVec[loop - 1]; // isolating the total count of species in the previous loop 

            // std::cout << "Species being removed...   " << sp_abundance_to_remove << "  < Abundance in previous loop \n"; // Printing that shizzle out 

            int new_abundance = sp_abundance_to_remove - 1;

            // std::cout << "new species abunace: " << new_abundance << "\n \n";

            totalSpVec.emplace_back(new_abundance);
        }
        else {



            int sp_abundance_no_change = totalSpVec[loop - 1]; // Lol, doesnt work for the first loop

            //std::cout << "No change in abundance: " << sp_abundance_no_change << "\n \n";

            totalSpVec.emplace_back(sp_abundance_no_change);

        }

        timeStepVec.emplace_back(loop);

    };



    void genCapture(int& generation) {

        for (const auto& i : rtree)
            rtreeSnapshotVec.emplace_back(std::pair<int, value>(generation, i));

    };


    void rtreeToCSV(const std::vector<std::pair<int, value>>& data, std::string& filePath) {

        std::ofstream outFile(filePath);

        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open the file " << filePath << " for writing." << std::endl;
            return;
        }

        // Write the header
        outFile << "Generation,ID,Species,X,Y\n";

        // Write the data
        for (const auto& item : data) {
            int number = item.first;
            const value& val = item.second;
            outFile << number << "," << val.second.uniqueID << "," << val.second.species << "," << val.first.get<0>() << "," << val.first.get<1>() << "\n";
        }

        outFile.close();

        if (!outFile.good()) {
            std::cerr << "Error: Could not write to the file " << filePath << "." << std::endl;
        }
        else {
            std::cout << "Successfully wrote to " << filePath << "." << std::endl;
        }
    };


    static void forestsToCSV(const std::vector<std::vector<std::pair<int, value>>>& data, std::string& filePath) {


        std::ofstream outFile(filePath);

        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open the file " << filePath << " for writing." << std::endl;
            return;
        }

        // Write the header
        outFile << "Forest ID,Generation,individual ID,Species,X,Y\n";

        // Write the data
        for (int forest = 0; forest < data.size(); forest++) {
            for (const auto& item : data[forest]) {
                int number = item.first;
                const value& val = item.second;
                outFile << forest << "," << number << "," << val.second.uniqueID << "," << val.second.species << "," << val.first.get<0>() << "," << val.first.get<1>() << "\n";
            }
        }

        outFile.close();

        if (!outFile.good()) {
            std::cerr << "Error: Could not write to the file " << filePath << "." << std::endl;
        }
        else {
            std::cout << "Successfully wrote to " << filePath << "." << std::endl;
        }


    }

    void writeCSV(const std::string& filename) {


        outSpVec[0] = timeStepVec;
        outSpVec[1] = totalSpVec;


        // Making a flile of the file name 
        std::ofstream file(filename);

        // If file is open alread it cannot be written to etc
        if (!file.is_open()) {
            std::cerr << "Unable to open the file " << filename << " for writing." << std::endl;
            return;
        }

        // Write the data to the file

        // Writing a loop that iterates for each row of the data frame 
        for (size_t i = 0; i < outSpVec[0].size(); ++i) {

            for (size_t j = 0; j < outSpVec.size(); ++j) {

                file << outSpVec[j][i]; // Write each element

                if (j != outSpVec.size() - 1) {

                    file << ","; // Add a comma if it's not the last element in the row
                }
            }
            file << "\n"; // Move to the next line after each row
        }

        file.close();
        std::cout << "CSV file " << filename << " has been created successfully." << std::endl;
    };



private:

    RTREE& rtree;
    params& mParams;


    int speicesRichness;
    std::vector<int> nSpVec;



    // Funtion to first count all the species in the rTree
    std::vector<int> firstCount() {



        // Make a vector of length equal to the number of species 
        for (int i = 0; i <= mParams.numSpecies; i++) {

            int val = 0;

            nSpVec.emplace_back(val);
        }


        // Adding the count of the species to the vector 
        for (const auto& element : rtree) {

            // Finding which specis the element belongs to
            int val = element.second.species;

            // Adding a count of this species to the n species vector 

            int tVal = nSpVec[val];

            nSpVec[val] = tVal + 1;

            // std::cout << "value:  " << val << "   abundance in vector;  " << nsp_vector[val] << "\n";
        }

        return nSpVec;


    };

    // Funtion to find the total species abundance 
    int firstRichness() {

        int species_abundance = 0;


        for (auto element : nSpVec) {
            if (element > 0)
                species_abundance = species_abundance + 1;
        }

        totalSpVec.emplace_back(species_abundance);
        std::cout << "initial richness: " << species_abundance;

        return species_abundance;
    };

};