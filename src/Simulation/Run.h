#pragma once
#include "settings.h"
#include "dataManagement.h"

#include <ImGui/App.h>


class Run {
public:

	Run();

	~Run() = default;

	void runSimulation();

	void runRepeats(); // Multithreading? 


	void runBasic();

private:
	void init();
	void noGUI();

	std::vector<std::unique_ptr<Simulation>> mRepeats; // Hopefully as simple as that

	//All smallish classes, so it should be okay to live on the stack? hopefully 
	params mParams;

	settings mSettings; // Here you should edit everthing 

	data mData; //WHAT DOES THIS EVEN DO

	std::unique_ptr<App> mApplication;

};

