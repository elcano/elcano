#include "ObstacleAvoidance.h"
#include <iostream>
#include <assert.h>

using namespace std;

int main() {

	vector<float> a;

	for (unsigned i = 0; i < 20; i++) {
		if (i > 10 && i < 16)
			a.push_back(0);
		else
			a.push_back(i);
	}
	ObstacleAvoidance* car = new ObstacleAvoidance(a);
	assert(car->clearAhead(11, 15) == true); // tests clearAhead method
	
	//cout<< car->getAGroup(1);
	// test make_groups
	//assert(car->getGroupLength() == 2);
	//assert((car->getGroups().size()) == 10);
	
	// testing group_clear()
	assert(car->groupClear(a) == false);
	cout << "Number of Groups: "<< car->getGroups().size() << endl;

	vector<float> b = {4, 4, 5, 7, 7, 8, 9, 10, 10.3, 0, 0, 1, 4, 3.3, 4.1};
	ObstacleAvoidance* car2 = new ObstacleAvoidance(b);
	cout << "Number of Groups: "<< car2->getGroups().size() << endl;

	cout << "Test Finished" << endl;
	return 0;
}
