#include "sim.h"
#include "settings.h"
#include "dataManagement.h"



class Run {
public:

	Run();

	~Run() = default;

	void runRepeats(); // Multithreading? 

	//void outputResults(std::vector<observation>& result); // Upload just one repeat 


private:

	std::vector<std::unique_ptr<Sim>> mRepeats; // Hopefully as simple as that

	//All smallish classes, so it should be okay to live on the stack? hopefully 
	params mParams;

	settings mSettings { mParams };

	data mData;



};

