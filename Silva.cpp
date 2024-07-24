// Silva.cpp : Defines the entry point for the application.
//

#include "Silva.h"
#include "src/ImGui/App.h"


/* TODOS:
	Intergrate UI and the simulation
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
	App app;

	app.Run();

	return 0;
}
