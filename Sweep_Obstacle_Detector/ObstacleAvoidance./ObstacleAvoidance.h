#include <vector>
/*
 * This class recieves an array of data from sensors. Then we can use various methods in this class 
 * to develop algorithm to avoid obstacles depending upon how far the obstacles are from a trike
 *
 * Sujit Neupane
 * 06/10/2019
 */

# ifndef OBSTACLEAVOIDANCE_H
# define OBSTACLEAVOIDANCE_H

using namespace std;

class ObstacleAvoidance {
	public:
		ObstacleAvoidance();

		// in_data is vector with range data from sensors
		// assuming sensors will give me an array with ranges of different obstacles
		ObstacleAvoidance(vector<float> in_data);

		// When vehicle moves, we will have the data before moving and we will have data after moving
		// lastData is the data before movement, newData is data in current position after movement
		ObstacleAvoidance(vector<float> lastData, vector<float> newData);

		ObstacleAvoidance(vector<float> in_data, int groupLength);

		// destructor
		virtual ~ObstacleAvoidance();
		
		// gets the vector of data
		const vector<float> getData() {
			return data;
		}

		/*int getGroupLength() {
			return groupLength;
		}*/		
		
		// makes groups of data from a big array of sensor data based on the provided length for the group
		// it helps us to find clear passage on our way
		// make groups should be changed so that it can make groups in such a manner that if ranges are not apart by
		//  2 meters, they should be considered same group
		//  e.g. [0, 0, 0, 3, 3.4, 4, 6.6, 6, 7, 8, 8, 9], here [0, 0, 0], [3, 3.4, 4], [6.6, 6, 7], and [8, 8, 9] are
		//  respective groups
		void makeGroups();

		vector<vector<float>> getGroups() {
			return groups;
		}

		// gets a sub group from a vector of groups. Index number is provided
		vector<float> getAGroup(int index); 

		// this method checks if the group is clear
		// if the group is clear to move, trike steer towards the group and move towards it
		// if nothing is clear return 0
		bool groupClear(vector<float> group);

		// checks if these ranges are clear
		// give range as 30, and this method will check if there is anything in the group
		// smaller than 30, if some value in the group is below 30, 
		// then it will return false, means there is obstacle within 30 meters
		bool checkRange(vector<float> group, float range);

		// After a clear group is found, we need to calculate steer angle 
		// so that the trike steer towards the clear group
		// calculates steer angle in degrees and returns it
		float getSteerAngle(int groupIndex, int straightIndex);		

		// if the data between startIndex and endIndex indicates nothing in the path, then the path is clear, 
		// if the path is clear, it returns true, else returns false
		// if the data is zero in between these postions then the path is considered clear
		bool clearAhead(const int startIndex, const int endIndex);

	private:
		vector<float> data;
		vector<float> newData;

		int groupLength; // group length is the length of small arrays broken apart as groups from a big array of data from sensors. Only gets set when makeGroups method is called
		vector<vector<float>> groups;

};
#endif
