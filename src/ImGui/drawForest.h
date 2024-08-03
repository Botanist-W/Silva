
#include "Simulation/Simulation.h"
#include "Simulation/randFuncs.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <map>

class drawForest {
public:

    drawForest(std::shared_ptr<Forest> _forest);

    // Method to visualize the forest with a given ID
    void visualizeForest();

private:
    std::shared_ptr<Forest> mForest;

    std::map<int, ImVec4> speciesColors;  // Maps speciesID to a unique color

    void buildColourLib();


    // Helper method to generate a random color
    ImVec4 generateRandomColor();

    // Helper method to get or assign color to a speciesID
    ImVec4 getSpeciesColor(int speciesID);

    void buildSpLib();
    std::vector<indiv> spLibrary;
};
