
struct TargetLocation
{
   long int targetSpeed;
   long int bearing;
   long int northPos;
   long int eastPos;
};

struct Cubic
{
  float a;
  float b;
  float c;
  float d;
};

struct Point
{
  float x;
  float y;
};

class Cubic2D
{
public:  

  /* Prefered constructor: 
   * takes start point (start position of the trike) and end point (final position)
   * start bearing and end bearing (compass directions
   * and arc adjust (this value makes the turn wider, a "less straight" line
   */
  Cubic2D(Point start, Point end, float startBearing, float endBearing, float arcAdjust)
  {
    Point startTanPoint = CalculateStartTangentPoint(startBearing);
    Point endTanPoint   = CalculateStartTangentPoint(endBearing);

    endTanPoint = TangentArcAdjustment(endTanPoint, arcAdjust);
    
    CalculateCubic(x, start.x, end.x, startTanPoint.x, endTanPoint.x);
    CalculateCubic(y, start.y, end.y, startTanPoint.y, endTanPoint.y);
  }

  /*
   * Calculation of the current x or y value at the time value passes in.
   */
  Point valueAtTime(float t)
  {
    Point retVal;
    retVal.x = ValueAtTime(x, t);
    retVal.y = ValueAtTime(y, t);
    return retVal;
  }

  /*
   * This is a basic 3rd degree to 2nd degree derivative function. This
   * will be used with the speed calculation.
   */
  Point derivativeValueAtT(float t)
  {
    Point retVal;
    retVal.x = DerivativeValueAtT(x, t);
    retVal.y = DerivativeValueAtT(y, t);
    return retVal;
  }

  /*
   * Calculation of the speed the trike will need to be traveling at
   * time t.
   */
  float SpeedAtT(float t)
  {
     float yPrime = DerivativeValueAtT(y,t);
     float xPrime = DerivativeValueAtT(x,t);
     return sqrt(sq(xPrime) + sq(yPrime));
  }

  /*
   * Calculation of the arc length at time t. This allows us to know
   * Where we are by how far we have traveled.
   */
  float ArcLength(float t,float deltaT, float current)
  {
    if(FloatComparison(t,0.00,2))
    {
      return 0;
    }
    else
    {
      float currentX = ValueAtTime(x,t);
      float currentY = ValueAtTime(y,t);
      float previousX = ValueAtTime(x,(t-deltaT));
      float previousY = ValueAtTime(y,(t-deltaT));
      
      return(current + (sqrt(sq(currentX - previousX)+sq(currentY - previousY))));
    }
  }
  
private:  
  /*
   * This is a basic 3rd degree to 2nd degree derivative function. This
   * will be used with the speed calculation.
   */
  float DerivativeValueAtT(Cubic x, float t)
  {
    Cubic xPrime;
    float retVal;
    xPrime.a = (x.a*(3*sq(t)));
    xPrime.b = (x.b*(2*t));
    xPrime.c = (x.c);
  
    retVal = (xPrime.a + xPrime.b + xPrime.c);
    return retVal;
  }
  

  
  
  /*
   * Calculation of the current x or y value at the time value passes in.
   */
   float ValueAtTime(Cubic x, float t)
  {
    Cubic cubicAtT;
    float retVal;
    cubicAtT.a = (x.a*(pow(t,3)));
    cubicAtT.b = (x.b*(sq(t)));
    cubicAtT.c = (x.c * t);
    cubicAtT.d = x.d;
  
    retVal = (cubicAtT.a + cubicAtT.b + cubicAtT.c + cubicAtT.d);
    return retVal;
  }
  

  /*
 * FirstCoefficient calculates the first coefficient of the Hermite cubic 
 * function. This requires input of the Tanget values adjusted for the arc 
 * length. the start point and end point are the map locations we want the bike 
 * to sit at or start at. This functioncan solve for both the x and y equations.
 */
  float FirstCoeffiecent(float endTangent, float endValue, float startValue, 
        float startTangent)
{
  float retVal = (endTangent - (2 * endValue) +(2 * startValue) + startTangent);
  return retVal;
}

  /*
 * SecondCoefficient calculates the second coefficient of the Hermite cubic 
 * function This requires input of the Tanget values adjusted for the arc 
 * lenght. the start point and end point are the map locations we want the 
 * bike to sit at or start at. This function can solve for both 
 * the x and y equations.
 */
  float SecondCoeffiecent(float firstCoeffiecent, float tangentStartValue, 
        float endValue, float startValue)
  {
  float retVal = (-firstCoeffiecent-tangentStartValue-startValue+endValue);
  return retVal;
}

  /*
 * CalculateCubic will do all the work needed to calculate the cubic function 
 * it takes in a Cubic by reference and stores the values of a b c d in the 
 * struct. this requres input of The start point, end point, point of the 
 * direction of the end tangent adusted by the arc and direction of the start 
 * point adjusted by the arc.
 */
  void CalculateCubic(Cubic& function, float startValue, float endValue,
     float startTangent, float endTangent)
  {
  function.a = FirstCoeffiecent(endTangent, endValue, startValue,
      startTangent);
  function.b = SecondCoeffiecent(function.a, startTangent,
      endValue, startValue);
  function.c = startTangent;
  function.d = startValue;
}

  /*
 *  Calculate start tangent translates angle of the trike to a corresponding 
 *  point on a line that passes through the origin with the slope and compass
 *  direction representing the same angle. Example
 *  angle 0 degress = 1,0 or angle 90 degrees equals 0,1.
 */
  Point CalculateStartTangentPoint(float angleDegrees)
  {
  Point retVal;
  retVal.x= sin((angleDegrees * 0.0174533));
  retVal.y = cos((angleDegrees * 0.0174533));
  return retVal;
}

  /*
 * This function takes 2 points and translates the slope between the two
 * to a point that is on the equivalent slope line that passes through
 * the origin this allows us to calculate the tangent for the next point.
 */
  Point pointSlope(Point a, Point b)
  {
    float slope = (a.y - b.y) / (a.x - b.x);
    Point wrtOrigin;
    wrtOrigin.x = 1;
    wrtOrigin.y = slope;
    return wrtOrigin;
}

  /*
 * This point translates the tangent point value based on the arc length
 * passed in by the user to allow for different curve profiles which
 * allows for tighter or wider turns which will also change corner speed.
 */
  Point TangentArcAdjustment(Point target, float arcLength)
  {
      Point retVal;
      retVal.x = (target.x/sqrt(sq(target.x) + sq(target.y))) * arcLength;
      retVal.y = (target.y/sqrt(sq(target.x) + sq(target.y))) * arcLength;
      return retVal;
  }  

  bool FloatComparison(float a, float b, int places)
  {
    // values are cast to an integer for = comparison of
    // values.
    int aVal;
    int bVal;
    // each case represents the number of decimal places compared.
    switch(places)
    {
      case 1:
          aVal = a*10;
          bVal = b*10;
          break;
      case 2:
          aVal = a*100;
          bVal = b*100;
          break;
      case 3:
          aVal = a*1000;
          bVal = b*1000;
          break;
      case 4:
          aVal = a*10000;
          bVal = b*10000;
          break;
  
      default:
          aVal = a;
          bVal = b;
          break;
    }
    // return cases.
    if(aVal == bVal)
    {
      return true;
    }
    else 
    {
      return false;
    }
  }
  
  Cubic x;
  Cubic y;
};


////////////////////////////////////////////////////////////////////////////////
void setup() 
{  
  Serial.begin(9600); // for debugging
  
  Point start, end;
  start.x = 0;
  start.y = 0;
  end.x = 5;
  end.y = 10;
  Cubic2D turn(start, end, 90, 270, 100);
    
  for(int i = 10; i >= 0; i--)
  {
    Serial.println(i);
    delay(1000);
  }
  for(int i = 0; i < 100; i++)
  {
    Point current = turn.valueAtTime(i/100.0);  
    Serial.println(String(current.x) + "," + String(current.y));
  } 
}



void loop() 
{

}
