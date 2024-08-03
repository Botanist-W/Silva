#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "drawForest.h"
#include "Simulation/settings.h"
#include "Simulation/Simulation.h"
#include "Simulation/dataManagement.h"
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

	App(settings& _settings, data& _data);

	~App();

	bool Run();

private:

	int setup();
    void cleanup();
    void Menu();
    void ResizeMatrix(int newSize);

   
    void DrawFragments();  //Drawing the fragments as squares n Stuff
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
  
    settings& mSettings;
    data& mData;

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



    // Debugging things
    std::shared_ptr<Forest> mForest;
    std::unique_ptr<drawForest> mDrawForest;
    bool drawAForest = false;
};


#endif // !APPLICATION