#include "ObstacleAvoidance.h"
#include <cmath>



ObstacleAvoidance::ObstacleAvoidance(vector<float> in_data) {
	for (unsigned i = 0; i < in_data.size(); i++) {
		this->data.push_back(in_data[i]);
	}
	makeGroups();
}

ObstacleAvoidance::ObstacleAvoidance(vector<float> lastData, vector<float> newData) {
	for (unsigned i = 0; i < lastData.size(); i++) { //lastData is assigned to data
		this->data.push_back(lastData[i]);
	}
	for (unsigned i = 0; i < newData.size(); i++) { // newData is assigned to newData
		this->newData.push_back(newData[i]);
	}

}

/*ObstacleAvoidance::ObstacleAvoidance(vector<float> in_data, int len) {
	for (unsigned i = 0; i < in_data.size(); i++) {
		this->data.push_back(in_data[i]);
	}
	makeGroups();

}*/

ObstacleAvoidance::~ObstacleAvoidance() {
	data.clear();
	newData.clear();
}

void ObstacleAvoidance::makeGroups() {
	int j = 0;
	vector<float> oneGroup;
	for (int i = 0; i < data.size(); i++) {
		float value = std::abs(data[i - 1] - data[i] );
		if (value < 2 || i == 0) {
			oneGroup.push_back(data[i]);
		} else {
			groups.push_back(oneGroup);
			oneGroup.clear();
			oneGroup.push_back(data[i]);	
		}
	}
	groups.push_back(oneGroup);
	/*groupLength = length;
	int i = 0;
	while (i < data.size()) {
		int j = 0;
		vector<float> oneGroup;
		while (j < groupLength) {
			oneGroup.push_back(data[i]);
			i++;
			j++;
		}
		groups.push_back(oneGroup);
	}*/
}

vector<float> ObstacleAvoidance::getAGroup(int index) {
	return groups[index];
}

// if the group has all zero ranges, means there is nothing in the path, then trike moves towards this group
// group suggests angle to steer
bool ObstacleAvoidance::groupClear(vector<float> group) {
	for (int i = 0; i < group.size(); i++) {
		if (group[i] > 0) { // 0 should be replaced by maiximum range of sensor, group[i] < maximum_range
			return false; // there is something in the group
		}
	}
	return true;
}

bool ObstacleAvoidance::checkRange(vector<float> group, float range) {
	for (int i = 0; i < group.size(); i++) {
		if (group[i] < range) {
			return false;
		}
	}
	return true;

}

float ObstacleAvoidance::getSteerAngle(int groupIndex, int straightIndex) {
	// groupNumber is 1 to groupLength
	// in groups, however, they are stored from 0 to (groupLength - 1)
	// float angle = (groupLength * groupNumber) - (groupLength / 2);
	float angle = 0;
	if (straightIndex > groupIndex) {
		angle = ((straightIndex - groupIndex) * groupLength) - (groupLength/2);
		return angle;
	}
	angle = ((groupIndex - straightIndex) * groupLength) - (groupLength/2);
	return angle;
}

// checks if the data in between the provided index are zero or not
// zero means nothing is infront of the sensor, if it is other than zero, we need to change it
// We use this method only if it is all clear
bool ObstacleAvoidance::clearAhead(const int startIndex, const int endIndex) {
	for (int i = startIndex; i <= endIndex; i++) {
		if (data[i] != 0) { // 0 should be replaced by maximum range of sensor, data[i] < maximum_range
			return false;
		} 
	}
	return true;
}

