 /* PlotSegments routine for C3
   Tyler Folsom 4/10/14
   
   Part of basic automated motion test
   
   C6 finds speed and passes it to C4. C4 passes speed to C3.
   
   The speed/odometer reading comes in as a serial SENSOR {speed nnnn} command.
   In C3,  Plot Segments coordinates an odometer with segments that describe the vehicle's
   desired speed.  Each piece is governed by a structure that specifies the distance to
   maintain a specific speed. Rates of acceleration and deceleration can also be specified.
   Based on how far the vehicle has traveled, this routine has the job of finding the desired speed.
   The desired speed goes out as a DRIVE {speed nnnn} serial command to C2. 
   
   The C2 computer has the task of controlling the vehicle to the desired speed.
   */
#include <stdlib.h>
#include <arduino.h>

void setupWheelRev();

#define STEPS 3
#define LOOP_TIME_MS 250
#define WHEEL_DIAMETER_MM 397
#define BUFFER_SIZE 80

/*
  The standard design uses several microcontrollers:
  C6 D2 gets the wheel click interrupt; C6 gets other speed and location information.
  C6 Tx2 -> C4 Rx; C4 reads the destination and map and finds a route
  C4 Tx -> C3 Rx; C3 which comes up with speed and steering to follow the route
  C3 Tx -> C2 Rx3, which controls the actuators to the desired speed and steering.
  
  C4 is not yet part of this test, so the serial output Tx2 of C6 can be jumpered directly to C3 Rx.
  
  This MotionPilot rountine is the speed part of C3. In operation, it will receive actual and
  desired speeds over the serial line. C3 is designed to run an Arduino Micro or Nano.
  
  For testing, both the WheelClick functions of C6 and the functions of C3 can be combined on a single
  Arduino. If SINGLE_BOARD is defined, the WheelClick routine is computed on an a single Arduino,
  which sends the resulting speed out on its SerialODoOut line, which must be jumpered to its SerialDrive 
  receive line.
  
  The single board test can be done with or without the MegaShieldTrio board. Without, connect 
  Wheel click input to D2, interrupt 0.
  SerialOdoOut Tx to SerialDrive Rx; on Mega, Tx2:D16 to Rx1:D19
  SerialDrive Tx to C2 Rx3
  
  The Trio board lets us plug wheel click into the jack. 
  Two pins connected on the Trio (such as D45 and D48) takes care of SerialOdoOut Tx to SerialDrive Rx
  Connecting a cable between the MegaSieldTrio and MegaShieldDB takes care of SerialDrive Tx to C2 Rx3

*/
#define DUEMILLENOVE 1
#define MEGA         2
#define LEONARDO     3
#define TRIO         4

#define SINGLE_BOARD MEGA

#define SerialMonitor Serial

#if (SINGLE_BOARD  == TRIO)
// TRIO board used with a MEGA
#include <SoftwareSerial.h>
// Trio board C6 connections: 26 Tx -> connector 18 -> C2 Rx3
// 50 Tx -> 11 Rx 
// connector 19 -> 17, Rx = Rx2 (unused)
SoftwareSerial SerialDrive(11, 26);
SoftwareSerial SerialOdoOut (17, 50);

#elif (SINGLE_BOARD  == MEGA) // Mega without TRIO board
#define SerialOdoOut  Serial2
#define SerialDrive   Serial1

#elif (SINGLE_BOARD == LEONARDO)  // OK for Micro or Leonardo
#define SerialOdoOut  Serial1
#define SerialDrive   Serial1
// TO DO: Are we using speed in mm/sec or revolutions/sec?
#define SpeedCyclometer_revPs  sensor_speed_mmPs

#elif (SINGLE_BOARD == DUEMILLENOVE) // or all others
#include <SoftwareSerial.h>
SoftwareSerial SerialDrive(6, 7);
SoftwareSerial SerialOdoOut(4, 5);

#else
// multiple processors
#define SerialOdoOut  Serial2
#define SerialDrive   Serial  // C4

#endif

// globals
long int sensor_speed_mmPs = 0;
long int distance_travelled_mm = 0;
char IncomingMessage[BUFFER_SIZE];
int InIndex=0;  // Input side, current character of SerialDrive message
//int StartIndex=0; // Input side, 1st character of SerialDrive message

const long speed_tolerance_mmPs = 75;  // about 0.2 mph
// find bike stopping distances on http://www.exploratorium.edu/cycling/brakes2.html
// Use this to compute typical deceleration
const long max_decel_mmPs2 =     30000;
const long typical_decel_mmPs2 = 17630;
const long min_decel_mmPs2 =     10000;
const long min_accel_mmPs2 =      1000;
const long typical_accel_mmPs2 =  6000;
const long max_accel_mmPs2 =     20000;
const long max_mult = 0x7FFF;  // multiplying bigger numbers might overflow.
/*---------------------------------------------------------------------------------------*/ 

struct rung
{
    /* distance of this step is measured starting at the point where previous step has covered its distance.
        If there was no previous step, start from zero.
        distance terminates when we have reached the speed of the next step.
        If there is no next step, come to a stop when distance is covered.
    */
    long pause_ms;    // if vehicle is not moving, stay at rest this long before starting.
    long distance_mm;  // distance within this segment
     // all other distances are cummulative
    long distance2steady_mm;  // at this point we expect to be at steady speed 
    long slow_distance_mm; // when we get here, enter transition_out state.
    long cummulative_distance_mm;  // distance desired to travel in the previous segments. 
    long speed_mmPs;  // when in steady state, move at this speed.
    long accel_mmPs2;  // desired rate of acceleration
    long decel_mmPs2;  // desired rate of deceleration
    int state;
//    enum state {initial, starting, steady, finishing, completed};
#define STATE_1st_TIME  0
#define STATE_WAITING   1
#define STATE_INITIAL   2
#define STATE_STARTING  3
#define STATE_STEADY    4
#define STATE_FINISHING 5
#define STATE_COMPLETED 6
};
/*---------------------------------------------------------------------------------------*/ 

struct ladder
{
    struct ladder *previous;
    rung *data;
    struct ladder *next;
};

ladder profile[STEPS];
rung    speeds[STEPS];

void InitializeSpeed (ladder* seg, long measured_speed_mmPs);
long int Find_Speed (ladder* seg, long int measured_speed_mmPs);
/*---------------------------------------------------------------------------------------*/ 
// Create links between rungs of speed profile.
// Fill in 
void SetSpeeds()
{
    SerialMonitor.println("Setting speeds ");
    for (int i = 0; i < STEPS; i++)
    {
        if (i > 0)    
        {
            profile[i].previous = &profile[i-1];
        }
        else
        {
            profile[i].previous = NULL;        
        }
        if (i+1 < STEPS)
        {
            profile[i].next = &profile[i+1];
        }
        else
        {
            profile[i].next = NULL;
        }
        profile[i].data = &speeds[i];
        speeds[i].pause_ms = 0;
        speeds[i].state = STATE_1st_TIME;
        speeds[i].accel_mmPs2 = typical_accel_mmPs2;
        speeds[i].decel_mmPs2 = typical_decel_mmPs2;
        // Here put in the desired values for the speed profile
        switch(i)
        {
          case 0:
            speeds[i].pause_ms = 3000;  // wait 3 sec before starting
            speeds[i].distance_mm = 2000; // 2 m @ 1.1 mph
            speeds[i].speed_mmPs = 500;
            break;
          case 1:
            speeds[i].distance_mm = 3000;  // 3 m
            speeds[i].speed_mmPs = 2500;  // 2500 mm/s = 5.6 mile/hr
            break;
          case 2:
          default:
            speeds[i].distance_mm = 30000;  // 30 m
            speeds[i].speed_mmPs = 0;
            break;        
        }
/*
        SerialMonitor.print("Pause: ");   SerialMonitor.println(speeds[i].pause_ms);
        SerialMonitor.print("State: ");   SerialMonitor.println(speeds[i].state);
        SerialMonitor.print("Accel: ");   SerialMonitor.println(speeds[i].accel_mmPs2);
        SerialMonitor.print("Decel: ");   SerialMonitor.println(speeds[i].decel_mmPs2);
        SerialMonitor.print("Dist:  ");   SerialMonitor.println(speeds[i].distance_mm);
        SerialMonitor.print("Speed: ");   SerialMonitor.println(speeds[i].speed_mmPs); */
    }
}
/*---------------------------------------------------------------------------------------*/ 

void InitializeSpeed (ladder* seg, long measured_speed_mmPs)
{
    long delta_speed_mmPs;
        // check acceleration
    if (seg->data->decel_mmPs2 < 0)
    {
        seg->data->decel_mmPs2 = -seg->data->decel_mmPs2;
    }
    if (seg->data->decel_mmPs2 > max_decel_mmPs2)
    {
        seg->data->decel_mmPs2 = max_decel_mmPs2;
    }    
    if (seg->data->decel_mmPs2 < min_decel_mmPs2)
    {
        seg->data->decel_mmPs2 = min_decel_mmPs2;
    }
    if (seg->data->accel_mmPs2 < 0)
    {
        seg->data->accel_mmPs2 = -seg->data->accel_mmPs2;
    }
    if (seg->data->accel_mmPs2 > max_accel_mmPs2)
    {
        seg->data->accel_mmPs2 = max_accel_mmPs2;
    }
    if (seg->data->accel_mmPs2 < min_accel_mmPs2)
    {
        seg->data->accel_mmPs2 = min_accel_mmPs2;
    }
    seg->data->state = STATE_STARTING;
        
    if (seg->previous == NULL)
    {   // no prior ladder
        seg->data->cummulative_distance_mm = seg->data->distance_mm;
    }
    else
    {
        seg->data->cummulative_distance_mm = seg->data->distance_mm + 
          seg->previous->data->cummulative_distance_mm;
    }
    // compute expected distance to come up to speed
    delta_speed_mmPs = seg->data->speed_mmPs - measured_speed_mmPs;
    if (delta_speed_mmPs >= 0)
    {   // speeding up
        if (delta_speed_mmPs < max_mult)  // no overflow
            seg->data->distance2steady_mm = 
                 (delta_speed_mmPs * delta_speed_mmPs) / seg->data->accel_mmPs2;
        else
        {
            float temp = delta_speed_mmPs;
            temp *= temp;
            temp /= seg->data->accel_mmPs2;
            seg->data->distance2steady_mm = temp;
        }
    }
    else
    {   // slowing down
        if (-delta_speed_mmPs < max_mult)
            seg->data->distance2steady_mm =  
                (delta_speed_mmPs * delta_speed_mmPs) / seg->data->decel_mmPs2;
        else
            seg->data->distance2steady_mm =  
                (delta_speed_mmPs/ seg->data->decel_mmPs2) * delta_speed_mmPs;        
    }
    SerialMonitor.print("to Steady   (mm): ");  SerialMonitor.println(seg->data->distance2steady_mm);
    seg->data->distance2steady_mm += distance_travelled_mm;
    SerialMonitor.print("to Steady   (mm): ");  SerialMonitor.println(seg->data->distance2steady_mm);
        
    // compute expected stopping distance
    if (seg->next == NULL)
    {  // no next ladder; come to a halt
        delta_speed_mmPs = seg->data->speed_mmPs;
    } 
    else
    {
        delta_speed_mmPs = max(0, (seg->data->speed_mmPs - seg->next->data->speed_mmPs));
               /*  if (deta_speed_mmPs < 0)
         // speed up on next segment. There is no deceleration at the end of this segment.
        */          
    }
            
    if (delta_speed_mmPs < max_mult)
        seg->data->slow_distance_mm =  seg->data->cummulative_distance_mm - 
        (delta_speed_mmPs * delta_speed_mmPs) / seg->data->decel_mmPs2;
    else
        seg->data->slow_distance_mm =  seg->data->cummulative_distance_mm - 
         (delta_speed_mmPs/ seg->data->decel_mmPs2) * delta_speed_mmPs;
    SerialMonitor.print("to Slow     (mm): ");
    SerialMonitor.println(seg->data->slow_distance_mm);
    if (seg->data->slow_distance_mm < seg->data->distance2steady_mm)
    { // We cannot get up to speed before we need to slow down
        if (seg->data->speed_mmPs - measured_speed_mmPs < 0)
        {   // First part of rung was slowing down.
             seg->data->distance2steady_mm = 
             seg->data->slow_distance_mm = distance_travelled_mm;
             seg->data->state = STATE_FINISHING;      
        }
        else
        {  // replace trapezoid with triangle
          // Find where the acceleration and decelration curves meet.
            seg->data->distance2steady_mm = 
             (seg->data->cummulative_distance_mm - distance_travelled_mm) * 
             seg->data->accel_mmPs2 / (seg->data->accel_mmPs2 + seg->data->decel_mmPs2);
            seg->data->slow_distance_mm =
              (seg->data->cummulative_distance_mm - distance_travelled_mm) * 
              seg->data->decel_mmPs2 / (seg->data->accel_mmPs2 + seg->data->decel_mmPs2);      
          }
    }
    if (seg->data->cummulative_distance_mm <= distance_travelled_mm)
        {  // Have already travelled past this segment.
            seg->data->state = STATE_COMPLETED;  // move to the next segment.
        }
    SerialMonitor.print("Travelled   (mm): ");  SerialMonitor.println(distance_travelled_mm);
    SerialMonitor.print("to Steady   (mm): ");  SerialMonitor.println(seg->data->distance2steady_mm);
    SerialMonitor.print("to Slow     (mm): ");  SerialMonitor.println(seg->data->slow_distance_mm);
    SerialMonitor.print("Cummulative (mm): ");  SerialMonitor.println(seg->data->cummulative_distance_mm);
    SerialMonitor.print("State: ");             SerialMonitor.println(seg->data->state);
}

/*---------------------------------------------------------------------------------------*/ 

long int Find_Speed (ladder* seg, long int measured_speed_mmPs)
{
    long set_speed_mmPs = measured_speed_mmPs;
    long final_speed_mmPs = 0;
    static unsigned wait_time_ms;
    distance_travelled_mm += (measured_speed_mmPs * LOOP_TIME_MS / 1000); 
    SerialMonitor.print("State: ");
    SerialMonitor.println(seg->data->state);
    SerialMonitor.print(" Distance travelled (mm): ");
    SerialMonitor.println(distance_travelled_mm);
    SerialMonitor.flush();
    switch (seg->data->state)
    {
      case STATE_1st_TIME:
        wait_time_ms = millis();
        if (abs(measured_speed_mmPs) < speed_tolerance_mmPs && seg->data->pause_ms > 0)
        {
            wait_time_ms +=  seg->data->pause_ms;
            seg->data->state = STATE_WAITING;
        }
        else
        {
            seg->data->state = STATE_INITIAL;
            goto Initial;
        }
        break;
        
      case STATE_WAITING:
        if (millis() >= wait_time_ms || abs(measured_speed_mmPs) > speed_tolerance_mmPs)
        {
            seg->data->state = STATE_INITIAL;
            goto Initial;
        }
        break;

      case STATE_INITIAL:  // initializing
Initial:    InitializeSpeed (seg, measured_speed_mmPs);  // routine sets state.
        // fall through
      case STATE_STARTING:    // coming up to speed    
        SerialMonitor.print(" Distance 2 steady (mm): ");
        SerialMonitor.println(seg->data->distance2steady_mm);
        SerialMonitor.print(" Target Speed (mm/s): ");
        SerialMonitor.println(seg->data->speed_mmPs);
 
        if (abs(measured_speed_mmPs - seg->data->speed_mmPs) < speed_tolerance_mmPs)            
        {  /*  have reached target speed */
            seg->data->state = STATE_STEADY;   // steady;
            set_speed_mmPs = seg->data->speed_mmPs;    
        }
        else
        {
            if (distance_travelled_mm >= seg->data->distance2steady_mm)             
            {  /* have completed the ramp-up on the trapezoid */ 
                seg->data->state = STATE_STEADY;   // steady;
                set_speed_mmPs = seg->data->speed_mmPs;    
            } 
            else 
            {  /* coming up to speed */
                long int dist_left_mm = (seg->data->distance2steady_mm - distance_travelled_mm);
                long int time_left_ms = measured_speed_mmPs > 0? 
                    (1000 * dist_left_mm) / measured_speed_mmPs: 1000 * dist_left_mm;
                long int time_steps = time_left_ms / LOOP_TIME_MS;
                long int delta_speed_mmPs = abs(seg->data->speed_mmPs - measured_speed_mmPs);
                SerialMonitor.print(" dist left (mm): ");
                SerialMonitor.println(dist_left_mm);
                SerialMonitor.print(" time left (ms): ");
                SerialMonitor.println(time_left_ms);
                SerialMonitor.print(" time steps: ");
                SerialMonitor.println(time_steps);
                if (time_steps <= 1)
                {  /* no time for ramp */ 
                    seg->data->state = STATE_STEADY;   // steady;
                    set_speed_mmPs = seg->data->speed_mmPs;    
                }                   
                else if (measured_speed_mmPs < seg->data->speed_mmPs)
                {  // accelerating
                    set_speed_mmPs = measured_speed_mmPs + delta_speed_mmPs / time_steps;
                    SerialMonitor.print(" + speed (mm/s): ");
                    SerialMonitor.println(set_speed_mmPs);
                    set_speed_mmPs = min(seg->data->speed_mmPs, set_speed_mmPs);
                }
                else
                {  // decelerating
                    set_speed_mmPs = measured_speed_mmPs - delta_speed_mmPs / time_steps;
                    SerialMonitor.print(" - speed (mm/s): ");
                    SerialMonitor.println(set_speed_mmPs);
                    set_speed_mmPs = max(0, set_speed_mmPs);
                }
            }
        }
        if (distance_travelled_mm >= seg->data->slow_distance_mm)
            seg->data->state = STATE_FINISHING;   // slowing;
        break;
    
      case STATE_STEADY:  // steady speed
        SerialMonitor.print(" Slow Distance (mm): ");
        SerialMonitor.println(seg->data->slow_distance_mm);
        set_speed_mmPs = seg->data->speed_mmPs;  // for case 2
        if (distance_travelled_mm >= seg->data->slow_distance_mm)
            seg->data->state = STATE_FINISHING;   // slowing; fall through
        else break;
        
      case STATE_FINISHING: // slowing
        if (seg->next)
            final_speed_mmPs = seg->next->data->speed_mmPs;
        if (distance_travelled_mm >= seg->data->cummulative_distance_mm)
        /* have come too far */
        {
            seg->data->state = STATE_COMPLETED; // completed
            set_speed_mmPs = final_speed_mmPs;
        } 
        else 
        {  /* coming up to speed */
            long int dist_left_mm = (seg->data->cummulative_distance_mm - distance_travelled_mm);
            long int time_left_ms = (1000 * dist_left_mm) / measured_speed_mmPs;
            long int time_steps = time_left_ms / LOOP_TIME_MS;
            long int delta_speed_mmPs = abs(final_speed_mmPs - measured_speed_mmPs);
            SerialMonitor.print(" dist left (mm): ");
            SerialMonitor.println(dist_left_mm);
            SerialMonitor.print(" time left (ms): ");
            SerialMonitor.println(time_left_ms);
            SerialMonitor.print(" time steps: ");
            SerialMonitor.println(time_steps);
            if (time_steps <= 1)
            {  /* no time for ramp */ 
                seg->data->state = STATE_COMPLETED;
                set_speed_mmPs = final_speed_mmPs;    
            }
            else if (measured_speed_mmPs < final_speed_mmPs)
            {  // accelerating
                set_speed_mmPs = measured_speed_mmPs + delta_speed_mmPs / time_steps;
                SerialMonitor.print(" + speed (mm/s): ");
                SerialMonitor.println(set_speed_mmPs);
                set_speed_mmPs = min(seg->data->speed_mmPs, set_speed_mmPs);
            }
            else
            {  // decelerating
                set_speed_mmPs = measured_speed_mmPs - delta_speed_mmPs / time_steps;
                SerialMonitor.print(" - speed (mm/s): ");
                SerialMonitor.println(set_speed_mmPs);
                set_speed_mmPs = max(0, set_speed_mmPs);
            }
        }
        break;
        
      case STATE_COMPLETED: // completed. should be using the next ladder.
        if(seg->next)
            set_speed_mmPs = seg->next->data->speed_mmPs;
        else
            set_speed_mmPs = 0;  // replace by next ladder
        break;
    }  // end switch(state)
    SerialMonitor.print(" set Speed (mm/s): ");
    SerialMonitor.println(set_speed_mmPs);
    return set_speed_mmPs;
}
/*---------------------------------------------------------------------------------------*/ 
   
void setup() 
{ 

    sensor_speed_mmPs = 0;
    SerialMonitor.begin(115200); 
    SerialMonitor.print("\n\n\n");
    SerialDrive.begin(115200); // C4 to C3 to C6
    /* A typical message is 20 ASCII characters of 20 bits each
       (2 8-bit bytes / character + start bit + stop bit)
       At 115,200 bits/s, a typical message takes 3.5 ms.
       Thus there is about a 10 ms delay (best case) in passing 
       information from C6 to C2.
    */
    SetSpeeds();
    setupWheelRev();
}
/*---------------------------------------------------------------------------------------*/ 

char * GetWord(char * major, char * str)
{
    char * CSp1;

    CSp1 = strstr( str, major);
//  SerialMonitor.print("Got: ");
//  SerialMonitor.println(CSp1);
    if (CSp1!=NULL)
    CSp1 += strlen(major);
//  SerialMonitor.print("Got: ");
//  SerialMonitor.println(CSp1);

    return CSp1;
}
/*---------------------------------------------------------------------------------------*/ 

void ProcessMessage ()
{
// Determine if message is "SENSOR {Speed xxx.xx}"    
//  SerialMonitor.print("\nString: ");
//  SerialMonitor.println(IncomingMessage);
    char * Args = GetWord ("SENSOR", IncomingMessage);
    if (Args == NULL)
        return;
    // SENSOR, so grab the new sensor_speed.
//  SerialMonitor.print("Processing: ");
//  SerialMonitor.println(Args);
    char * Number = GetWord("Speed", Args);
    
    if (Number==NULL) return;
    // change } to 0
    char* end = strchr(Number, '}');
    if (end == NULL) return;
    *end = '\0';
    float data = atof(Number);   // rev/s
//    SerialMonitor.print("data: ");
//    SerialMonitor.println(data);

    // change back to }
    *end = '}';
    // convert speed from rev/s to mm/s
    float Circum_mm = (WHEEL_DIAMETER_MM * PI);
    sensor_speed_mmPs = (long)(data * Circum_mm);
//    SerialMonitor.print("Speed (mm/s): ");
//    SerialMonitor.println(sensor_speed_mmPs);
}
/*---------------------------------------------------------------------------------------*/ 
void readSerial()
{
    int incomingByte = 0;   // for incoming serial data
    while (SerialDrive.available() > 0) 
    {
        // read the incoming byte from C4:
        incomingByte = SerialDrive.read();
        
        IncomingMessage[InIndex] = (char)(incomingByte);
        IncomingMessage[InIndex+1] = 0;
/*      SerialMonitor.print("Available: ");
        SerialMonitor.print(IncomingMessage); 
        SerialMonitor.print( " InIndex: ");
        SerialMonitor.print(InIndex);
        SerialMonitor.print( " incomingByte: ");
        SerialMonitor.print((char) incomingByte);
        SerialMonitor.print( " ");
        SerialMonitor.println( incomingByte); */
        if (IncomingMessage[InIndex] == 0 || incomingByte == '\n' || incomingByte == '\r'
         || InIndex >= BUFFER_SIZE-1)
        {
            ProcessMessage();  // see what we got
            SerialDrive.print(IncomingMessage); // pass msg on to C2
            SerialMonitor.print(IncomingMessage); // pass msg on to C2
            for (InIndex = 0; InIndex < BUFFER_SIZE; InIndex++)
                IncomingMessage[InIndex] = 0;
            InIndex = 0;
            IncomingMessage[InIndex] = 0;
  /*        SerialMonitor.print("Cleared: ");
            SerialMonitor.print(IncomingMessage); 
            SerialMonitor.print( " InIndex: ");
            SerialMonitor.print(InIndex);
            SerialMonitor.print( " incomingByte: ");
            SerialMonitor.println((char) incomingByte); */
        }
       else
       {
           ++InIndex;        
       }        
    }
}
/*---------------------------------------------------------------------------------------*/ 

void loop()
{
    static int section = 0;
    unsigned long time, endTime;
    time = millis();
    endTime = time + LOOP_TIME_MS ;
    show_speed();
    while (time < endTime)
    {
        readSerial();
        time = millis();
    }
    
    long desired_speed_mmPs = Find_Speed (&profile[section], sensor_speed_mmPs);
    SerialMonitor.println(" Found speed");    
    if (profile[section].data->state >= STATE_COMPLETED)
    {
        if(section < STEPS)
        {
            desired_speed_mmPs = Find_Speed (&profile[++section], sensor_speed_mmPs);
        }
        else
            desired_speed_mmPs = 0;  // speed profile completed
    }
    float desired_speed_revPs = ((float) desired_speed_mmPs) / (WHEEL_DIAMETER_MM * PI);
    SerialDrive.print("DRIVE {Speed ");
    SerialDrive.print(desired_speed_revPs);
    SerialDrive.println("}\0");
    SerialMonitor.print("DRIVE {Speed ");
    SerialMonitor.print(desired_speed_revPs);
    SerialMonitor.println("}\0");
}
/*=======================================================================================*/
#ifdef SINGLE_BOARD
/*=======================================================================================*/
/* Wheel Revolution Interrupt routine
   Ben Spencer 10/21/13
   Modified by Tyler Folsom 3/16/14
   
   A cyclometer gives a click once per revolution. 
   This routine computes the speed.
*/
// CLICK_IN defined: use interrupt; not defined: simulate with timer
#define CLICK_IN 1
// #define LOOP_TIME_MS 1000
#define CLICK_TIME_MS 1000

#define WHEEL_DIAMETER_MM 397
#define MEG 1000000
#define MAX_SPEED_KPH 50
#define MAX_SPEED_mmPs   ((MAX_SPEED_KPH * MEG) / 3600)
// MAX_SPEED_mmPs = 13,888 mm/s = 13.888 m/s
unsigned long MinTickTime_ms;
// ((WHEEL_DIAMETER_MM * 3142) / MAX_SPEED_mmPs)
// MinTickTime_ms = 89 ms
#define MIN_SPEED_mPh 500
// A speed of less than 0.5 KPH is zero.
unsigned long MaxTickTime_ms;
// ((WHEEL_DIAMETER_MM * 3142) / MIN_SPEED_mmPs)
// MinTickTime_ms = 9239 ms = 9 sec

float Odometer_m = 0;
long SpeedCyclometer_mmPs = 0;
// Speed in revolutions per second is independent of wheel size.
float SpeedCyclometer_revPs = 0.0;//revolutions per sec
volatile unsigned long TickTime = 0;
long WheelRev_ms = 0;
unsigned long OldTick = 0;
#define IRQ_NONE 0
#define IRQ_FIRST 1
#define IRQ_RUNNING 2
volatile int InterruptState = IRQ_NONE;
unsigned long ShowTime_ms;

/*---------------------------------------------------------------------------------------*/ 
// WheelRev is called by an interrupt.
void WheelRev()
{
    static int flip = 0;
    unsigned long tick;   
    noInterrupts();
    tick = millis();
    if (InterruptState != IRQ_RUNNING)
    // Need to process 1st two interrupts before results are meaningful.
        InterruptState++;

    if (tick - TickTime > MinTickTime_ms)
    {
        OldTick = TickTime;
        TickTime = tick;
    }
    if (flip)
        digitalWrite(13, LOW);
    else
        digitalWrite(13, HIGH);
    flip =!flip;  
    
    interrupts();
}
/*---------------------------------------------------------------------------------------*/ 
#endif  // SINGLE_BOARD
void setupWheelRev() 
{ 
#ifdef SINGLE_BOARD
    SerialOdoOut.begin(115200); // C6 to C4        
    pinMode(13, OUTPUT); //led
    digitalWrite(13, LOW);//turn LED off
    
    pinMode(2, INPUT);//pulls input HIGH
    float MinTick = WHEEL_DIAMETER_MM * PI;
//    SerialMonitor.print (" MinTick = ");
//    SerialMonitor.println (MinTick);
    MinTick *= 1000.0;
    MinTick /= MAX_SPEED_mmPs;
//    SerialMonitor.print (MinTick);
    MinTickTime_ms = MinTick;
    SerialMonitor.print (" MinTickTime_ms = ");
    SerialMonitor.println (MinTickTime_ms);

//    SerialMonitor.print (" MIN_SPEED_mPh = ");
//    SerialMonitor.print (MIN_SPEED_mPh);
    float MIN_SPEED_mmPs =  ((MIN_SPEED_mPh * 1000.0) / 3600.0);
    // MIN_SPEED_mmPs = 135 mm/s
//    SerialMonitor.print (" MIN_SPEED_mmPs = ");
//    SerialMonitor.print (MIN_SPEED_mmPs);
    float MaxTick = (WHEEL_DIAMETER_MM * PI * 1000.0) / MIN_SPEED_mmPs;
//    SerialMonitor.print (" MaxTick = ");
//    SerialMonitor.print (MaxTick);
    MaxTickTime_ms = MaxTick;
//    SerialMonitor.print (" MaxTickTime = ");
//    SerialMonitor.println (MaxTickTime_ms);
    TickTime = millis();
    // OldTick will normally be less than TickTime.
    // When it is greater, TickTime - OldTick is a large positive number,
    // indicating that we have not moved.
    // TickTime would overflow after days of continuous operation, causing a glitch of
    // a display of zero speed.  It is unlikely that we have enough battery power to ever see this.
    OldTick = TickTime;
    ShowTime_ms = TickTime;
    InterruptState = IRQ_NONE;
#ifdef CLICK_IN
    attachInterrupt (0, WheelRev, RISING);//pin 2 on Mega
#endif
    SerialMonitor.print("TickTime: ");
    SerialMonitor.print(TickTime);
    SerialMonitor.print(" OldTick: ");
    SerialMonitor.println(OldTick);
     
    SerialMonitor.println("setup complete");
#endif // SINGLE_BOARD    
}
/*---------------------------------------------------------------------------------------*/ 

void show_speed()
{
#ifdef SINGLE_BOARD
   ShowTime_ms = millis();       
   if (InterruptState == IRQ_NONE || InterruptState == IRQ_FIRST)  // no OR 1 interrupts
   {
       SpeedCyclometer_mmPs = 0;
       SpeedCyclometer_revPs = 0;
   } 

  //check if velocity has gone to zero
  else
  {
    if(ShowTime_ms - TickTime > MaxTickTime_ms)
    {  // stopped
        SerialMonitor.print("Stop. Showtime: ");
        SerialMonitor.print(ShowTime_ms);
        SerialMonitor.print(" Tick: ");
        SerialMonitor.println(TickTime);
        SpeedCyclometer_mmPs = 0;
        SpeedCyclometer_revPs = 0;
    }
    else
    {  // moving
        WheelRev_ms = max(TickTime - OldTick, ShowTime_ms - TickTime);
        if (InterruptState == IRQ_RUNNING)
        {  // have new data
      
            float Circum_mm = (WHEEL_DIAMETER_MM * PI);
            if (WheelRev_ms > 0)
            {
                SpeedCyclometer_revPs = 1000.0 / WheelRev_ms;
                SpeedCyclometer_mmPs  = Circum_mm * SpeedCyclometer_revPs;
            }
            else
            {
                SpeedCyclometer_mmPs = 0;
                SpeedCyclometer_revPs = 0;
            }
        }

      }
    }
  // C4 to C3
 // C3 to C2 via pin 18 on DB25
 // Short Pin 18 to 19 (Or have C2 repeat what it received)
 // Pin 19 on DB25 goes to SerialOdoOut input.
    // Show on monitor
    SerialMonitor.print("\nWheelRev (ms): ");
    SerialMonitor.print(WheelRev_ms);
    SerialMonitor.print(" SENSOR ");
    SerialMonitor.print("{Speed ");
    SerialMonitor.print(SpeedCyclometer_revPs);
    SerialMonitor.println("}\0");
    Odometer_m += (float)(LOOP_TIME_MS * SpeedCyclometer_mmPs) / MEG;
#endif // SINGLE_BOARD    
  // Serial 2 connects C6 to C4
    SerialOdoOut.print("SENSOR {Speed ");
    SerialOdoOut.print(SpeedCyclometer_revPs);
    SerialOdoOut.println("}\0");
}

