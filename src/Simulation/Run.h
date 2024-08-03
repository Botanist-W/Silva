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

	std::vector<observation> getObvs(); //Trying to get stuff thzat the app can print out 

	//void outputResults(std::vector<observation>& result); // Upload just one repeat 


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

