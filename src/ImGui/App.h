#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "csv.h"
#include <stdio.h>
#include <memory>
#include <vector>
#include <sstream>
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





struct params { // 

    // Simulation Settings
    int timeSteps = 10000;
    int numRep = 1;  //number replicates

    int numSpecies = 300; // number of species  
    float treeDensity = 0.04; // Number of trees per area or lambda for pois()

    // Immigration settings
    bool metaCommunityImmigration = false;
    int metaComSize = 50000;

    // Fragment interactions etc
    int numFragments = 3;
    bool equalFragmentSize = true;
    float size = 50; // L ... Size of simulation Usefull for continuous 
    std::vector<float> fragmentSizeList; // Is the equal Fragment size == false --> this this is used 
    std::vector<std::vector<float>> nodeMap;

    // Ecological settings
    bool fragmented = false; // Run with or without fragmentation
    bool neutralComp = true;

    float searchArea = 100; // ZOI
    float b1 = 0.01;
    float b2 = 7;
    float m = 0.03; // Immigration rate
    float dispersalDis = 20;
    float mort = 0.1; // mortality rate TODO : implement or remove 
    float HNDD = 0.1; // Default HNDD strength TODO: implement
    float CNDD = 0.1; // Default CNDD
    float extinctionRate = 0.0001;


   
};




class App {
public:

	App();

	~App();

	void Run();

private:

	int setup();

	void Menu();


    void ResizeMatrix(int newSize);

    void DrawFragments();
    void newFragmentDraw();


    std::vector<ImVec2> fragmentPositions = {};
    std::vector<bool> fragmentPosCache;

	GLFWwindow* mWindow;
    int windowWidth, windowHeight;
    float menuWidth, menuHeight, menuPos;
    float fragmentCanvasWidth, fragmentCanvasHeight, fragmentCanvasPos;
    float relFragmentScale;
    float fragmentBufferX = 0;
    float fragmentBufferY = 50;
    int row = 0;
    int nextRow = 0;

	ImGuiIO io;

	bool m_show_demo_window = true; // I know
	bool m_show_another_window = false;
	ImVec4 clear_color;

	params par;
	bool run;
	bool useDefaults;
    bool saveSettings = false;
    bool loadSettings = false;
    bool openNewNodeMap = false;

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