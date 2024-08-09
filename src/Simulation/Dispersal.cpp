
#include "Dispersal.h"
#include "pch.h"




// DISPERSAL

inline point dispersal::dispersalKernal(value& parent) {
    // remember that "a" (parent.dispersal) is not the raw mean dispersal distance but insead a transformed value...

    // This is the fancy part <-- iverse of a 2dt kernel
    double dispDis = parent.second.dispersal * sqrt(pow((1 - Crand::rand_double(0, 1)), (1 / (1 - 2))) - 1); // TODO: SET DISPERSALLLLL!!!

    double r_angle = Crand::rand_double(0, 2) * Pi; // 

    LOG_TRACE("Dispersal distance: : {}", dispDis);

    return point((parent.first.get<0>() + cos(r_angle) * dispDis), (parent.first.get<1>() + sin(r_angle) * dispDis));

};


// FRAGMENTED DISPERSAL
point fragmentedDisp::disperse(value& parent) {

    return dispersalKernal(parent); // Keeping it simple here

};

bool fragmentedDisp::inBounds(point& pos){
    if (0 < pos.get<0>() && pos.get<0>() < mForest.bounds &&
        0 < pos.get<1>() && pos.get<1>() < mForest.bounds)
        return true;
    else
        return false;
}
// ~ fragmented dispersal


// Fragmented Search
std::vector<value> fragmentedDisp::search(point& sought, double& searchArea) {
    return mForest.search(sought, searchArea); // just a pass along but it works I guesss
};
// ~ Fragmented Search


// CONTINUOUS DISPERSAL
point continuousDisp::disperse(value& parent) {
    point pos = dispersalKernal(parent);
    return dispTorus(pos);
};

// Better dispersal that can wrap several time 
inline point continuousDisp::dispTorus(point& pos) {
    double posX = pos.get<0>();
    double posY = pos.get<1>();

    posX = std::fmod(posX, mForest.bounds);
    posY = std::fmod(posY, mForest.bounds);

    if (posX < 0) {
        posX += mForest.bounds;
    }
    if (posY < 0) {
        posY += mForest.bounds;
    }

    //LOG_TRACE("Dispersed to position: ({}, {})", posX, posY);

    return point(posX, posY);
}

bool continuousDisp::inBounds(point& p) {
    return true;
}

// ~ Continuous dispersal

// CONTINUOUS SEARCH

std::vector<value> continuousDisp::search(point& sought, double& searchArea) {

    searchResults = mForest.search(sought, searchArea);

    periodicBoundy(sought);

    return searchResults;

};



enum continuousDisp::edgeCase
{
    none,

    top,
    bot,
    left,
    right,

    topRight,
    topLeft,
    botRight,
    botLeft
};

continuousDisp::edgeCase continuousDisp::whichEdge(const point& sought) {

    const double sought_x = bg::get<0>(sought);
    const double sought_y = bg::get<1>(sought);

    // Is it outside on an edge? 
    if (mForest.searchArea < sought_x && sought_x < mForest.bounds - mForest.searchArea &&
        mForest.searchArea < sought_y && sought_y < mForest.bounds - mForest.searchArea)
    {
        return edgeCase::none;
    }
    else {
        // Left?
        if (sought_x < mForest.searchArea) {
            // Just left & not top or bottom?
            if (mForest.searchArea < sought_y && sought_y < mForest.bounds - mForest.searchArea) {
                return edgeCase::left;
            }
            // Check if it also falls on the "top" edge
            else if (sought_y > mForest.bounds - mForest.searchArea) {
                return edgeCase::topLeft;
            }
            // Check if it also falls on the "bottom" edge
            else {
                return edgeCase::botLeft;
            }
        }
        // Right?
        else if (sought_x > (mForest.bounds - mForest.searchArea)) {
            // Just right & not top or bottom?
            if (mForest.searchArea < sought_y && sought_y < mForest.bounds - mForest.searchArea) {
                return edgeCase::right;
            }
            // Check if it also falls on the "top" edge
            else if (sought_y > mForest.bounds - mForest.searchArea) {
                return edgeCase::topRight;
            }
            // Check if it also falls on the "bottom" edge
            else {
                return edgeCase::botRight;
            }
        }
        // Just Bottom & not right or left? 
        else if (sought_y < mForest.searchArea) {
            return edgeCase::bot;
        }
        else if (sought_y > (mForest.bounds - mForest.searchArea)) { // Must be top if all else is false
            return edgeCase::top;
        }
    }
};

void continuousDisp::posMod(const point& sought, const point& edgeSought, std::vector<value>& input) { // Functions to alter the positions of the search funcs 

    // Find differences in thingy
    double dx = sought.get<0>() - edgeSought.get<0>();
    double dy = sought.get<1>() - edgeSought.get<1>();

    for (auto& val : input) {
        val.first.set<0>(val.first.get<0>() - dx);
        val.first.set<1>(val.first.get<1>() - dy);
    }

};

void continuousDisp::searchEdge(const point& edgeSought, const point& sought) { // 

    std::vector<value> searchEdgeResults = mForest.search(edgeSought, mForest.searchArea);

    posMod(edgeSought, sought, searchEdgeResults);

    searchResults.insert(searchResults.end(), searchEdgeResults.begin(), searchEdgeResults.end()); // This modifies the main search results so be careful 
};

void continuousDisp::periodicBoundy(const point& sought) {

    const double sought_x = bg::get<0>(sought);
    const double sought_y = bg::get<1>(sought);

    // Find Which edge the point lies on 
    switch (whichEdge(sought)) {

    case edgeCase::none:
        // Do nothing, return distances already calculated
        break;
    case edgeCase::left:
        searchEdge(point(mForest.bounds + sought_x, sought_y), sought);
        break;
    case edgeCase::right:
        searchEdge(point(-(mForest.bounds - sought_x), sought_y), sought);
        break;
    case edgeCase::top:
        searchEdge(point(sought_x, -(mForest.bounds - sought_y)), sought);
        break;
    case edgeCase::bot:
        searchEdge(point(sought_x, mForest.bounds + sought_y), sought);
        break;
    case edgeCase::topLeft:
        searchEdge(point(sought_x + mForest.bounds, -(mForest.bounds - sought_y)), sought); // Searching bottom right
        searchEdge(point(mForest.bounds + sought_x, sought_y), sought); // searching top right
        searchEdge(point(sought_x, -(mForest.bounds - sought_y)), sought); //searching bottom left
        break;
    case edgeCase::topRight:
        searchEdge(point(-(mForest.bounds - sought_x), -(mForest.bounds - sought_y)), sought); // Search bottom left
        searchEdge(point(sought_x, -(mForest.bounds - sought_y)), sought); // Search bottom right
        searchEdge(point(-(mForest.bounds - sought_x), sought_y), sought); // Search top left
        break;
    case edgeCase::botLeft:
        searchEdge(point(sought_x + mForest.bounds, sought_y + mForest.bounds), sought); // Top right
        searchEdge(point(sought_x + mForest.bounds, sought_y), sought); // Bot right
        searchEdge(point(sought_x, mForest.bounds + sought_y), sought); // Top left
        break;
    case edgeCase::botRight:
        searchEdge(point(-(mForest.bounds - sought_x), mForest.bounds + sought_y), sought); // Top left
        searchEdge(point(-(mForest.bounds - sought_x), sought_y), sought); // Bot left
        searchEdge(point(sought_x, mForest.bounds + sought_y), sought); // Top right
        break;
    }

};
// ~ continuous search9