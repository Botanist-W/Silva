#include "sim.h"
#include "settings.h"
#include "dataManagement.h"
#include <future> 
#include <thread>
#include <ImGui/App.h>


class Run {
public:

	Run();

	~Run() = default;

	void runSimulation();

	void runRepeats(); // Multithreading? 

	//void outputResults(std::vector<observation>& result); // Upload just one repeat 


private:
	void init();
	void noGUI();

	std::vector<std::unique_ptr<Sim>> mRepeats; // Hopefully as simple as that

	//All smallish classes, so it should be okay to live on the stack? hopefully 
	params mParams;

	settings mSettings; // Here you should edit everthing 

	data mData; //WHAT DOES THIS EVEN DO

	std::unique_ptr<App> mApplication;

};

