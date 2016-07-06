/* The obstacle avoidance routine recieves an array of integers
 * from the C5_Sensor board which specifies the distance to a  
 * potential obstacle in each of the 7 directional sectors 
 * currently managed by the sensor setup.
 * Obstacle avoidance uses this data to determine how the bike
 * should respond, and integrates the response into the pilot
 * where it can be managed with respect to the planned course,
 * destination and location.
 */


// Permanent Sensor Locations
//(currently no rear facing sonar)
int* leftSensors = {1,2,3}
int* forwardSensors = {3,4,5}  
int* rightSensors = {5,6,7}



/* obstacleResponse:
 * takes in the rangeData from the sonar board
 * and the current speed of the bike and determines
 * the response that the pilot should take given
 * the proximity of obstacles in each direction
 * at the current speed.
 * Returns an integer which corresponds to the 
 * suggested obstacle response as follows:
 * 
 * 0 : NO RESPONSE 
 * 1 : FULL BRAKE
 * 2 : HARD RIGHT
 * 3 : HARD LEFT
 */
int obstacleResponse(int* rangeData, int Speed)
{
  int threshold = calcThreshold(Speed);
  bool leftResponse = rangeDetect(rangeData, leftSensors, threshold);
  bool forwardResponse = rangeDetect(rangeData, forwardSensors, threshold);
  bool rightResponse = rangeDetect(rangeData, rightSensors, threshold);

  if(leftResponse || forwardResponse || rightResponse)
  {
    if(leftResponse && forwardResponse && rightResponse)
    {
      return 1; // FULL BRAKE
    }
    else if(leftResponse && frontResponse)
    {
      return 2; // HARD RIGHT
    }
    else if(rightResponse && frontResponse)
    {
      return 3; // HARD LEFT
    }
    else if(leftResponse && rightResponse)
    {
      return 0; // NO RESPONSE 
    }
  }
  return 0; // NO RESPONSE
}


/*  calcThreshold:
 *  Calculates the threshold sensor
 *  range for which the bike should
 *  respond while travelling at the
 *  current speed
 */
int calcThreshold(int velocity)
{
  // Logic unavailable
  return 1500;
}


/* rangeDetect:
 * Uses a collection of sonar locations and  
 * a threshold to isolate the obstacle detection
 * to a specific direction and return whether
 * any obstacles are within the threshold range
 */
bool rangeDetect(int* rangeData, int* sonars, int threshold)
{
  //loop through specified sensors in rangeData
  for(int i = sonars[0]; i <= sonars[2]; i++)
  {
    //can be expanded for multi-hit weighing
    if(rangeData[i] < threshold)
    {
      return true;
    }
  }
  return false;
}



