#include "drawForest.h"


drawForest::drawForest(std::shared_ptr<Forest> _forest) :
	mForest(_forest) 
{
    buildColourLib();
}

void drawForest::buildColourLib() {

    // Wow, these map things are pretty cool
    for (int i = 0; i < mForest->numSpecies; i++) {
        speciesColors[i] = generateRandomColor();
    }

};

ImVec4 drawForest::generateRandomColor() {
  
    return ImVec4(Crand::randFloat(0,1), Crand::randFloat(0, 1), Crand::randFloat(0, 1), 1.0f);
}

ImVec4 drawForest::getSpeciesColor(int speciesID) {
    return speciesColors[speciesID];
}

/* TEXT VERSION, Could be useful
void drawForest::visualizeForest() {
    

    std::vector<value> trees = mForest.getValues();

    ImGui::SetNextWindowPos(ImVec2(100, 0));
    ImGui::SetNextWindowSize(ImVec2(1000, 1000));

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
*/

void drawForest::visualizeForest() {
    std::vector<value> trees = mForest->getValues();

    // THERE will be no good sizing here
    ImGui::SetNextWindowPos(ImVec2(500, 0));
    ImGui::SetNextWindowSize(ImVec2(1000, 1000));

    ImGui::Begin("Forest Visualization");

   // Why have I switched to sname case? Because I got this code from chat GPT
   // And it works just fine ... mostly
    // Getiing fancy <-- whats being drawn??? 
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize(); // Whats the size??
    float window_padding = 10;

    // Getting some padding :)
    ImVec2 draw_area_min = ImVec2(window_pos.x + window_padding, window_pos.y + window_padding);
    ImVec2 draw_area_max = ImVec2(window_pos.x + window_size.x - window_padding, window_pos.y + window_size.y - window_padding);

    // Scaling 
    float scaleX = (draw_area_max.x - draw_area_min.x) / mForest->bounds;
    float scaleY = (draw_area_max.y - draw_area_min.y) / mForest->bounds;

    for (const auto& entry : trees) {
        const point& pt = entry.first;
        const indiv& individual = entry.second;

        ImVec4 color = getSpeciesColor(individual.species);

        // Convert the point position to ImGui coordinates
        ImVec2 point_pos = ImVec2(draw_area_min.x + pt.get<0>() * scaleX, draw_area_min.y + pt.get<1>() * scaleY);

        // Draw the point
        draw_list->AddCircleFilled(point_pos, 5.0f, ImColor(color), 12);
    }

   
    ImGui::End();
}