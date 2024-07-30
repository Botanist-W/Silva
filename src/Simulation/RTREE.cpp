
#include "RTREE.h"


void rTree::buildFromLib(std::vector<indiv>& spLib) {

    for (int i = 0; i < numIndiv; i++) 
        tree.insert(value(point(Crand::randFloat(0, bounds), Crand::randFloat(0, bounds)), spLib[Crand::rand_int(0, numSpecies-1)]));

};

void rTree::build(std::vector<value>& input) {

    for (int i = 0; i < numIndiv; i++)
        tree.insert(input[i]);

}


std::vector<value> rTree::search(point sought, float& searchArea) {

    std::vector<value> results;

    float sought_x = bg::get<0>(sought);
    float sought_y = bg::get<1>(sought);

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
};

void rTree::removeTree(value remove) {
    tree.remove(remove);
};

value rTree::randomTree() {

    point point_rng = point(Crand::randFloat(0, bounds), Crand::randFloat(0, bounds));

    value result;
    tree.query(bgi::nearest(point_rng, 1), &result);
    return result;

};


std::vector<value> rTree::getValues() { // I know this is slow but I forgot how to do it propely 

    std::vector<value> results;

    for (auto& i : tree)
        results.emplace_back(i);

    return results;

};

