// Silva.cpp : Defines the entry point for the application.
//

#include "Silva.h"
#include "ImGui/App.h"
#include "Simulation/randFuncs.h"
#include "Log.h"
#include "Simulation/dataManagement.h"
#include "Simulation/Run.h"

boost::random::mt19937 rng;


/* TODOS:
	Intergrate UI and the simulation << NOW DO IT BETTER
	Sort out the directories bit, so I can more easily iterate through settings
	Get the data output sorted
		Alter simulation class to have a capture rate
		Create a capture folder named based on settings
		Get the output right and usefull
	Get a good debuging UI --> draw each tree as a point and draw whats going on, just to check
	Sort out Competition
	Offload the directories and data hangling to a seperate class

	Nice to Haves:
		Multithreading over each repeat 

*/



int main()
{
	Log::Init(); // Being lazy here 

	//App app;

	//app.Run();
	Run running;

	running.runRepeats();

	return 0;
}
