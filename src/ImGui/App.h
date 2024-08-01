#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "drawForest.h"
#include "Simulation/settings.h"
#include "Simulation/Simulation.h"
#include <stdio.h>
#include <memory>
#include <vector>
#include <numeric>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <filesystem>

#ifndef APPLICATION
#define APPLICATION




#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif // CAREFUL

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace im = ImGui;







class App {
public:

	App();

	~App();

	void Run();

    params getParams();

private:

	int setup();

    void Menu();
    void ResizeMatrix(int newSize);


    // Drawing 
    void DrawFragments();
    void newFragmentDraw();
    std::vector<ImVec2> fragmentPositions = {};
    std::vector<bool> fragmentPosCache;
    // ~ Drawing 

    // Main window & sizes
	GLFWwindow* mWindow;
    int windowWidth, windowHeight;
    float menuWidth, menuHeight, menuPos;
    float fragmentCanvasWidth, fragmentCanvasHeight, fragmentCanvasPos;
    float relFragmentScale;
    float fragmentBufferX = 0;
    float fragmentBufferY = 50;
    // ~

    int row = 0;
    int nextRow = 0;

	ImGuiIO io;
	ImVec4 clear_color;
    
    void updateSim();
    Simulation* mSim;
    std::unique_ptr<drawForest> mDrawForest;// { mSim.getForest(0) };
    bool drawAForest = false;

	params par;
	bool run = false;
	bool useDefaults =true;
    bool saveSettings = false;
    bool loadSettings = false;
    bool openNewNodeMap = false;


    // TODO: Offload to the settings class
    std::string dataOutputPath = "";
    std::string settingsPath = "";
    std::string settingsDirectory = "";


    void generateDirectory();

    void saveParams();
    void loadParams();

    void loadNodeMap();
    void saveNodeMap();

    void loadSizeList();
    void saveSizeList();

};


#endif // !APPLICATION