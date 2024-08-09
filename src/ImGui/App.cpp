#include "App.h"
#include "pch.h"

App::App(settings& _settings, data& _data) : mSettings(_settings), mData(_data){
    
	setup();

};

App::~App() {

};


void App::newFragmentDraw() {

    float xPos = 0;
    float yPos = 20;
   
    for (int i = 0; i < fragmentPosCache.size(); ++i) {

        if (fragmentPosCache[i] == false) {
            
            xPos = fragmentCanvasPos + std::accumulate(par.fragmentSizeList.begin() + nextRow, par.fragmentSizeList.begin() + i, 0) + 20;

            yPos = par.fragmentSizeList[0] * row + 20;

            if (xPos > fragmentCanvasWidth) {
                
                row++;
                nextRow = i;
                xPos = fragmentCanvasPos + 20;// +std::accumulate(par.fragmentSizeList.begin() + nextRow, par.fragmentSizeList.begin() + i, 0) + 20;
      
                yPos = par.fragmentSizeList[0] * row + 20;
            }

            ImVec2 pos = ImVec2(xPos, yPos);

           fragmentPositions.emplace_back(pos);

           fragmentPosCache[i] = true;
        }
    }
}

void App::DrawFragments() {


    float oldFragmentCanvasPos = fragmentCanvasPos;

    fragmentCanvasPos = menuWidth;
    fragmentCanvasHeight = windowHeight;
    fragmentCanvasWidth = windowWidth - 100;// -menuWidth;

    for (auto& i : fragmentPositions) {
        i = ImVec2(i.x + (fragmentCanvasPos - oldFragmentCanvasPos), i.y);
        if (i.x > fragmentCanvasWidth)
            i.x = fragmentCanvasWidth - par.size - 10;
        if(i.y > fragmentCanvasHeight)
            i.y = fragmentCanvasHeight - par.size - 10;
    }
    

    // Adjust scale factor if necessary
    relFragmentScale = 1; // Example scale factor

    // Begin the fragment canvas
    ImGui::SetNextWindowPos(ImVec2(fragmentCanvasPos, 0));
    ImGui::SetNextWindowSize(ImVec2(fragmentCanvasWidth, fragmentCanvasHeight));

    ImGui::Begin("Fragment Canvas", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Ensure the fragment positions are updated
    while (fragmentPositions.size() < par.numFragments) {
        fragmentPosCache.push_back(false);
        newFragmentDraw();
    }

    // Draw lines based on nodeMap
    for (int row = 0; row < par.nodeMap.size(); ++row) {
        for (int col = 0; col < par.nodeMap[row].size(); ++col) {
            if (par.nodeMap[row][col] != 0) {
                ImVec2 startPos = ImVec2(fragmentPositions[row].x + (0.5 * par.fragmentSizeList[row]), fragmentPositions[row].y + (0.5 * par.fragmentSizeList[row]));
                ImVec2 endPos = ImVec2(fragmentPositions[col].x + (0.5 * par.fragmentSizeList[col]), fragmentPositions[col].y + (0.5 * par.fragmentSizeList[col]));
                ImGui::GetWindowDrawList()->AddLine(startPos, endPos, IM_COL32(0, 234, 123, 255), 2.0f);
            }
        }
    }

    for (int i = 0; i < par.numFragments; ++i) {
        // Calculate the position of the square
        ImVec2 pos = fragmentPositions[i];

        int fragSize = par.fragmentSizeList[i] * relFragmentScale;

        // Define the square corners
        ImVec2 p_min = pos;
        ImVec2 p_max = ImVec2(pos.x + fragSize, pos.y + fragSize);

        // Draw the square
        ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, IM_COL32(200, 200, 200, 255));
        ImGui::GetWindowDrawList()->AddRect(p_min, p_max, IM_COL32(0, 0, 0, 255));

        // Draw the fragment index
        std::string label = "Fragment " + std::to_string(i);
        ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + 10, pos.y + 10), IM_COL32(0, 0, 0, 255), label.c_str());

        // Draw the input box for fragment size
        ImGui::SetCursorScreenPos(ImVec2(pos.x + 10, pos.y + 30));
        ImGui::PushItemWidth(fragSize - 20);
        ImGui::InputDouble(("##size" + std::to_string(i)).c_str(), &par.fragmentSizeList[i]);
        ImGui::PopItemWidth();

        // Make the square draggable
        ImGui::SetCursorScreenPos(pos);
        ImGui::InvisibleButton(("##drag" + std::to_string(i)).c_str(), ImVec2(fragSize, fragSize));
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            fragmentPositions[i].x += delta.x;
            fragmentPositions[i].y += delta.y;
        }

        // Handle manual drag-and-drop connections
        if (ImGui::InvisibleButton(("##connect" + std::to_string(i)).c_str(), ImVec2(fragSize, fragSize))) {
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
                ImVec2 mousePos = ImGui::GetIO().MousePos;
                ImGui::GetWindowDrawList()->AddLine(pos, mousePos, IM_COL32(213, 231, 25, 25), 2.0f);
                if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
                    for (int j = 0; j < par.numFragments; ++j) {
                        if (i != j && ImGui::IsMouseHoveringRect(fragmentPositions[j], ImVec2(fragmentPositions[j].x + fragSize, fragmentPositions[j].y + fragSize))) {
                            par.nodeMap[i][j] = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    ImGui::End();
}


void App::Menu() {

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    menuWidth = windowWidth / 3.5;
    ImGui::SetNextWindowSize(ImVec2(menuWidth, windowHeight));
    //ImGui::SetNextWindowContentSize(ImVec2(menuWidth, windowHeight));

    ImGui::Begin("Setup window", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

    im::SeparatorText("Ecological settings");
    ImGui::PushItemWidth(menuWidth/5);
    im::InputDouble("Immigration rate: ", &par.m);
    im::InputDouble("Strength of HNDD: ", &par.HNDD);
    im::InputDouble("Strength of CNDD: ", &par.CNDD);
    im::InputDouble("Dispersal distance: ", &par.dispersalDis);
    im::InputDouble("Zone of influence radius: ", &par.searchArea);
    im::InputDouble("b1: ", &par.b1);
    im::InputDouble("b2: ", &par.b2);
    ImGui::PopItemWidth();

    im::SeparatorText("Fragment settings");
    ImGui::PushItemWidth(menuWidth / 4);
    if (par.numFragments == 0)
        par.numFragments = 1;
    im::InputInt("No. of Fragments: ", &par.numFragments);
    ImGui::PopItemWidth();

    if (par.numFragments > 1) {
        par.fragmentSizeList.emplace_back(par.size);
        if (!im::Checkbox("Equal Fragment Size? ", &par.equalFragmentSize)) {
            if (!par.equalFragmentSize) {
                ImGui::BeginChild("ScrollableRegion", ImVec2(0, 100), true, ImGuiWindowFlags_HorizontalScrollbar);
                if (im::CollapsingHeader("Set fragment sizes")) {
                    for (int i = 0; i < par.numFragments; i++) {
                        std::string label = "Fragment " + std::to_string(i) + ": ";
                        ImGui::InputDouble(label.c_str(), &par.fragmentSizeList[i]);

                    }
                }
                ImGui::EndChild();
            }
            else {
                ImGui::PushItemWidth(menuWidth / 4);
                im::InputDouble("Input Size", &par.size);
                ImGui::PopItemWidth(); 
            }
        }
        else {
            
            im::InputDouble("Input size: ", &par.size);

        }
    

        if (!im::Checkbox("open in new window? ", &openNewNodeMap)) {
            if (!openNewNodeMap) {
                if (im::CollapsingHeader("Node Map (Fragment Connectivity)")) {

                    ResizeMatrix(par.numFragments);

                    if (ImGui::BeginTable("Matrix", par.numFragments, ImGuiTableFlags_Borders)) {
                        for (int row = 0; row < par.numFragments; ++row) {
                            ImGui::TableNextRow();
                            for (int col = 0; col < par.numFragments; ++col) {
                                ImGui::TableSetColumnIndex(col);
                                std::stringstream ss;
                                ss << "##cell" << row << "_" << col;

                                if (row == col) {
                                    ImGui::Text("%.2f", par.nodeMap[row][col]);
                                }
                                else if (row > col) {
                                    // get lower stuff
                                    if (ImGui::InputDouble(ss.str().c_str(), &par.nodeMap[row][col])) {
                                        // Reflecty stuff
                                        par.nodeMap[col][row] = par.nodeMap[row][col];
                                    }
                                }
                                else {
                                    // Dont touch this
                                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // Grey color
                                    ImGui::Text("%.2f", par.nodeMap[col][row]);
                                    ImGui::PopStyleColor();
                                }
                            }
                        }
                        ImGui::EndTable();
                    }


                }
                
            }
            else {
                ImGui::SetNextWindowPos(ImVec2(menuWidth, 0));
                ImGui::Begin("Node Map: ", nullptr);
                ResizeMatrix(par.numFragments);

                if (ImGui::BeginTable("Matrix", par.numFragments, ImGuiTableFlags_Borders)) {
                    for (int row = 0; row < par.numFragments; ++row) {
                        ImGui::TableNextRow();
                        for (int col = 0; col < par.numFragments; ++col) {
                            ImGui::TableSetColumnIndex(col);
                            std::stringstream ss;
                            ss << "##cell" << row << "_" << col;

                            if (row == col) {
                                ImGui::Text("%.2f", par.nodeMap[row][col]);
                            }
                            else if (row > col) {
                                
                                if (ImGui::InputDouble(ss.str().c_str(), &par.nodeMap[row][col])) {
                                    // To the top, make it drop
                                    par.nodeMap[col][row] = par.nodeMap[row][col];
                                }
                            }
                            else {
                                
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // Grey color
                                ImGui::Text("%.2f", par.nodeMap[col][row]);
                                ImGui::PopStyleColor();
                            }
                        }
                    }
                    ImGui::EndTable();
                }

                im::End();
            }
        }
    }
    else {
        im::InputDouble("Input Forest size: ", &par.size);

        im::InputInt("Metacommunity size: ", &par.metaComSize);
     
    }


    im::SeparatorText("Simulation settings");
    ImGui::PushItemWidth(menuWidth / 4.2);
    im::InputInt("No. of time steps: ", &par.timeSteps);
    im::InputInt("Capture Rate: ", &par.captureRate);
    im::InputInt("No. of Repeats: ", &par.numRep);
    im::InputInt("No. of Species: ", &par.numSpecies);
    ImGui::PopItemWidth();
    im::Separator();


    char buffer[256];
    strncpy(buffer, settingsPath.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0'; 
    im::SeparatorText("Save settings File Path: ");
    if (ImGui::InputText("##SettingsPath", buffer, sizeof(buffer))) {
        settingsPath = std::string(buffer);
    }

    char setBuffer[256];
    strncpy(setBuffer, dataOutputPath.c_str(), sizeof(setBuffer));
    setBuffer[sizeof(setBuffer) - 1] = '\0'; 
    im::SeparatorText("Output File Path: ");
    if (ImGui::InputText("##DataOutputPath", setBuffer, sizeof(setBuffer))) {
        dataOutputPath = std::string(setBuffer);
        
    }

    char sampleBuffer[256];
    strncpy(sampleBuffer, sampleDirectory.c_str(), sizeof(sampleBuffer));
    sampleBuffer[sizeof(sampleBuffer) - 1] = '\0'; 
    im::SeparatorText("Sample Forest Directory: ");
    if (ImGui::InputText("##SampleDirectory", sampleBuffer, sizeof(sampleBuffer))) {
        sampleDirectory = std::string(sampleBuffer);
        mData.setSampleDirectory(par, sampleDirectory);
        
    }

    if (im::Button("Save settings"))
        saveSettings = true;

    if (im::Button("Load settings"))
        loadSettings = true;

    if (im::Button("Run")) {

        run = true;

        drawAForest = false;

    }

    if (im::Button("Debug? ")) {
        drawAForest = true;
        mForest = std::make_shared<Forest>(par, 10);
        mDrawForest = std::make_unique<drawForest>(mForest);

    }

    ImGui::End();

};



void App::ResizeMatrix(int newSize) {
    par.nodeMap.resize(newSize);
    for (auto& row : par.nodeMap) {
        row.resize(newSize, 0.0f);
    }
}

bool App::Run() {
   
   if(!glfwWindowShouldClose(mWindow))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        glfwGetWindowSize(mWindow, &windowWidth, &windowHeight);

        if (drawAForest) {
            mDrawForest->visualizeForest();
        }
      
        if (saveSettings) {
            mSettings.save(settingsPath, par);
            saveSettings = false; 
        }

        if (loadSettings) {
            mSettings.load(settingsPath);
            par = mSettings.get();
            loadSettings = false;
        }

        if (run) {
            cleanup();
            mData.setPath(dataOutputPath);
            mData.setName(par);
            mSettings.set(par);
            return false;
        }
        // My stuff
        Menu();
        DrawFragments();


        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(mWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(mWindow);

        return true;
    }
   else {
       cleanup();
       return false;
    }
  
};


void App::cleanup() {
    
    #ifdef __EMSCRIPTEN__
        EMSCRIPTEN_MAINLOOP_END;
    #endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mWindow);
    glfwTerminate();

};

// SETUP
int App::setup() {

    fragmentPosCache = std::vector<bool>(par.numFragments, false);
    par.fragmentSizeList = std::vector<double>(par.numFragments, par.size);

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    mWindow = glfwCreateWindow(1500, 1000, "Ficus: CNDD & Fragmentation Simulation", nullptr, nullptr);
    if (mWindow == nullptr)
        return 1;
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark(); // ALWAYSSSS
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);

    ImGui_ImplOpenGL3_Init(glsl_version);

    io.Fonts->AddFontDefault();
   
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

};

