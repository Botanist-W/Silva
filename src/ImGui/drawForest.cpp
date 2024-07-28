#include "drawForest.h"


drawForest::drawForest(Forest& _forest) :
	mForest(_forest) 
{

}



ImVec4 drawForest::generateRandomColor() {
  
    return ImVec4(Crand::randFloat(0,1), Crand::randFloat(0, 1), Crand::randFloat(0, 1), 1.0f);
}

ImVec4 drawForest::getSpeciesColor(int speciesID) {
    if (speciesColors.find(speciesID) == speciesColors.end()) {
        speciesColors[speciesID] = generateRandomColor();
    }
    return speciesColors[speciesID];
}

void drawForest::visualizeForest() {
    

    std::vector<value> trees = mForest.getValues();

    ImGui::Begin("Forest Visualization");

    for (const auto& entry : trees) {
        const point& point = entry.first;
        const indiv& indiv = entry.second;

        ImVec4 color = getSpeciesColor(indiv.species);
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::Text("Species %d at (%.1f, %.1f)", indiv.species, point.get<0>(), point.get<1>());
        ImGui::PopStyleColor();
    }

    ImGui::End();
}