
#include "RTREE.h"


void rTree::buildFromLib(std::vector<indiv>& spLib) {

    for (int i = 0; i < numIndiv; i++) 
        tree.insert(value(point(Crand::rand_double(0, bounds), Crand::rand_double(0, bounds)), spLib[Crand::rand_int(0, numSpecies-1)]));

};

void rTree::build(std::vector<value>& input) {

    for (int i = 0; i < numIndiv; i++)
        tree.insert(input[i]);

}


std::vector<value> rTree::search(point sought, double& searchArea) {

    std::vector<value> results;

    double sought_x = bg::get<0>(sought);
    double sought_y = bg::get<1>(sought);

    box query_box(point(sought_x - searchArea, sought_y - searchArea), point(sought_x + searchArea, sought_y + searchArea));

    tree.query(
        bgi::within(query_box) && // First is searching the box
        bgi::satisfies([&](value const& v) // Declaring a lamda function 
            {return bg::distance(v.first, sought) < searchArea; }), // Stating the logic of the funtion
        std::back_inserter(results)); // Adding true values to the results vector 


    return results;
};

void rTree::addTree(value add) {
    tree.insert(add);
    addedSp = add.second.species;
};

void rTree::removeTree(value remove) {
    tree.remove(remove);
    removedSp = remove.second.species;
};

/*
value rTree::randomTree() {

   // point point_rng = ;

    value result;
    tree.query(bgi::nearest(point(Crand::rand_double(0, bounds), Crand::rand_double(0, bounds)), 1), &result); ////// HERE'S THE PROBLEM!!!!!!! MAYBE
    return result;

};
*/

// Final attempt 

value rTree::randomTree() {

    double boxSize = 20;

    std::vector<value> results; // Learing new shizzle 

    while (results.empty()) {

        double minX = Crand::rand_double(-boxSize, bounds);
        double minY = Crand::rand_double(-boxSize, bounds);

        box randBox = box(point(minX, minY), point(minX + 20, minY + 20));

        tree.query(
            bgi::within(randBox), std::back_inserter(results));

        boxSize += 5; // No point in a check, takes more time

    }

    int index = Crand::rand_int(0, results.size() - 1);

    return results[index];

};


//value rTree::randomTree() {
  //  return *std::next(tree.begin(), Crand::rand_int(0, tree.size() - 1));
//};

std::vector<value> rTree::getValues() { // I know this is slow but I forgot how to do it propely 

    std::vector<value> results;

    for (auto& i : tree)
        results.emplace_back(i);

    return results;

};

