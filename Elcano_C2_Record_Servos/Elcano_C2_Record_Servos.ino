/*
For each servo, we need the relationship between:
Control signal to servo.
Feedback value.
Servo extension.
Physical result (i.e. steering angle, brake pressure).

This sets the control signal on each servo, waits for the feedback to settle and
measures it, asks the user to record the servo extension (and physical result, if
feasible) along with the step number currently shown on the serial monitor.
To move to the next step, send any text via the serial monitor.  The entered text
will be echoed into the serial output, so suggest entering the observations -- that
way they'll all be recorded in one place.

Disconnect the brake before running this, else the brake servo may go over current
and blow a fuse!

This will move both servos a short distance on each step.  Near the extreme, it will
reverse.  It uses a conservative value for the extreme, so this should be adjusted
to actually reach the desired limits before taking data.

This runs through cycles repeatedly -- collect data for multiple runs so it can be
compared and averaged.  Values for the first few steps should be treated as suspect,
as this will be estimating the control value needed to match the initial feedback.
*/

#define MEGA

#define BASSACKWARDS
#include <IO_PCB.h>

const int LOOP_TIME_MS = 5;  // 5 milliseconds per loop pass.
unsigned long current_time_ms;
unsigned long next_loop_time_ms;
unsigned long pause_time_ms;

// @ToDo: Move anything shared among C2 variants, e.g. the _Instrument class, into a library.

// System state.
// @ToDo: Make State a class? If State becomes a class, move all state manipulation into it.
// Can make the current State, NextState, and ControlSource all part of the State class.
const int OFF = 0;           // System is not operating.
const int HOMING = 1;        // System is moving the actuators to their home positions.
const int CALIBRATING = 2;   // System is running calibration.
const int OPERATING = 3;     // Normal operation.
int State = OFF;
int NextState = OFF;         // State to enter after homing.
// Control source is ignored during calibration and homing.
const int USER_CONTROL = 0;  // System is being controlled by user.
const int AUTO_CONTROL = 1;  // System is being run by autonomous controller.
int ControlSource = USER_CONTROL;

// Calibration settings.
int CALIBRATION_STEPS_PER_SWEEP = 10;  // # of calibration steps to do from end to end of actuator's range in one direction.
int NUM_CALIBRATION_SWEEPS = 4;       // Do several sweeps.
int MAX_CALIBRATION_STEPS = NUM_CALIBRATION_SWEEPS * CALIBRATION_STEPS_PER_SWEEP;  

// Moving parts.
// @ToDo: Subclass the generic _Instrument class for Motor and LinearServo.
// In particular, feedback capture will differ.

const int Full = 255; // fully on for PWM signals -- this is full left.
const int Min = 127; // minimum PWM value that has any effect -- this is full right.
const int Off = 0;

const int FullBrake = 225; // start with a conservative value; could go as high as 255;  
const int MinimumBrake = 155; // start with a conservative value; could go as low as 127;

const int LimitLeft = 223; // start with a conservative value; could go as high as 255;
const int HalfLeft = 223; // midpoint btw straight and 255
const int Straight = 191;
const int HalfRight = 159; // midpoint btw straight and 127
const int LimitRight = 159; // start with a conservative value; could go as low as 127;

const int Motor = 0;
const int Brakes = 1;
const int Steering = 2;
const int NUM_INSTRUMENTS = 3;
const int IN_USE[] = {Brakes, Steering};
const int NUM_IN_USE = sizeof(IN_USE) / sizeof(int);

class _Instrument
{
 public:
  // Constants
  //static const int FEEDBACK_LEN = 5;

  // Instance data
  char* Name;          // A string for debugging and calibration messages.
  int DesiredPosition;
  int OldDesiredPosition;  // @ToDo: Need this? Intent was to ignore little twitches in the control.
  int MinPosition;
  int MaxPosition;
  int HomePosition;   // Home position for the actuator, e.g. for steering, straight ahead.
  int Feedback;       // Most recent feedback value. @ToDo: Store smoothed feedback value here.
  int OldFeedback;    // Previous (smoothed) feedback value.
  //int FeedbackHistory[FEEDBACK_LEN];  // History of raw feedback.
  //int FeedbackIndex;                  // Index of latest feedback reading.
  //int FeedbackDelta[FEEDBACK_LEN];    // History of change in feedback: it's awkward to just subtract FeedbackHistory values due to use of circular buffers.
  int EnablePin;
  int SignalPin;
  int FeedbackPin;
  int CurrentPin;
  int CloseEnough;    // Difference between two position values that is small enough to be regarded as equal. Measured in input levels (counts).
  int StuckCount;     // Max number of time steps for getting to position before being declared stuck.
  int NoMoveCount;    // # consecutive steps with feedback change < CloseEnough but distance from DesiredPosition >= CloseEnough
  boolean Stuck;      // Set if we've been stuck for at least StuckCount consecutive steps.
  // @ToDo: Step size depends on the loop rate. Instead, specify max slew rate --
  // step size per a specified time. Then, given a specified loop frequency, compute
  // the step size per loop pass.
  int MaxSlewRate;    // How much the actuator can change position per second, measured in same units as internal position and feedback, i.e. 0-1023.
  int StepSize;       // How much the actuator can change position per loop pass. Computed from MaxSlewRate and loop time.
  int CalibrationStepSize;  // Size of a calibration step, saved here to avoid recomputing it. @ToDo: Give this a generic name and use it as a scratch variable?
  // One count (minimum input level change) from analog input is about 5mV from ACS758lcb050U.
  // The ACS758 has sensitivity of 60 mA/V.
  int QuiescentCurrent;  // Nominally 120 counts
  int CurrentDraw;    // In counts, with 1 Amp = 12 counts
  int CurrentLimit;   // In counts
  // Parameters for linear estimation of the control value corresponding to the
  // observed feedback. These are expected to apply when the servo is not
  // currently moving, and only after waiting for feedback to settle. This will
  // be used mainly on startup to avoid moving the servos abruptly.
  float FeedbackSlope;
  float FeedbackIntercept;

  // Update the feedback value. @ToDo: Apply smoothing.
  void update_feedback()
  {
    Serial.print("In I.update_feedback ");
    Serial.print(Name);
    Serial.print(": Feedback = ");
    OldFeedback = Feedback;
    Feedback = analogRead(FeedbackPin);  // 0 to 1023
    Serial.println(Feedback);
  }

  // Initialize the feedback values. @ToDo: This needs to init the smoothing array.
  void initialize_feedback()
  {
    Serial.print("In I.initialize_feedback ");
    Serial.println(Name);
    update_feedback();       // OldFeedback won't have real data after the first call,
    OldFeedback = Feedback;  // so copy in the current feedback.
  }

  // Update the control value, DesiredPosition. External controllers, i.e. user or autonomous
  // operation, should set check_if_operating to true. This test for normal operation allows
  // update_control to be called asynchronously, e.g. on interrupt from an external autonomous
  // controller, without knowing whether homing or calibration is running. If the supplied
  // position is a change to the previous position (i.e. should be added to it), set is_delta true.
  // Defaults are appropriate for normal operation.
  void update_control(int new_desired_position,
                      boolean check_if_operating = true,
                      boolean is_delta = false)
  {
    Serial.print("In I.update_control ");
    Serial.print(Name);
    Serial.print(": new_desired_position = ");
    Serial.print(new_desired_position);
    Serial.print(" check_if_operating = ");
    Serial.print(check_if_operating);
    Serial.print(" is_delta = ");
    Serial.println(is_delta);
    if (!check_if_operating || State == OPERATING)
    {
      OldDesiredPosition = DesiredPosition;
      if (is_delta)
      {
        DesiredPosition += new_desired_position;
      }
      else
      {
        DesiredPosition = new_desired_position;
      }
      // Sanity check the new value. Note if something is checking for the position to reach the
      // requested value, the requested value had better be within CloseEnough of the allowed
      // range.
      if (DesiredPosition > MaxPosition)
      {
        DesiredPosition = MaxPosition;
      }
      else if (DesiredPosition < MinPosition)
      {
        DesiredPosition = MinPosition;
      }
    }
  }

  // Set the device's position control.
  // @ToDo: This will differ per device. Override in subclasses, especially for the motor.
  void apply_control(int this_move)
  {
    Serial.print("In I.apply_control ");
    Serial.print(Name);
    Serial.print(": pwm_position = ");
    // PWM values below 127 are ineffective for the servos we're using.
    int pwm_position = 127 + this_move/8;
    analogWrite(SignalPin, pwm_position);  // 127 to 255
    Serial.println(pwm_position);
  }

  // Check if posiiton is sufficiently close to desired position.
  int close_enough()
  {
    Serial.print("In I.close_enough ");
    Serial.print(Name);
    Serial.print(": DesiredPosition = ");
    Serial.print(DesiredPosition);
    Serial.print(" Feedback = ");
    Serial.print(Feedback);
    Serial.print(" result = ");
    boolean result = abs(DesiredPosition - Feedback) < CloseEnough;
    Serial.println(result);
    return result;
    //return abs(DesiredPosition - Feedback) < CloseEnough;
  }

  // Compute and apply one step toward the desired position.
  // (Note the Arduino IDE highlights the word move as though it is a reserved word, so don't
  // use that as the method name.)
  void move_position()
  {
    Serial.print("In I.move_position ");
    Serial.print(Name);
    Serial.print(": DesiredPosition = ");
    Serial.print(DesiredPosition);
    Serial.print(" Feedback = ");
    Serial.print(Feedback);
    Serial.print(" OldFeedback = ");
    Serial.println(OldFeedback);
    int this_move;
    // Already close enough to the desired position?
    if (close_enough())
    {
      Serial.println("Is close enough, so not moving.");
      return;
    }
    // No, keep moving.
    // Did we move on the last step? Note the first "did not move" after a move is
    // commanded will (likely) be a false positive.
    //if (abs(DesiredPosition - OldDesiredPosition) < CloseEnough) // !!! wrong -- should be feedback and old feedback, no?
    if (abs(Feedback - OldFeedback) <= CloseEnough)
    {
      // Did not move much.
      Serial.println("Didn't move much.");
      NoMoveCount++;
      if (NoMoveCount >= StuckCount)
      {
        Stuck = true;
      }
    }
    else
    {
      // Whew, not stuck this step.
      Serial.println("Did move some.");
      NoMoveCount = 0;
      Stuck = false;
    }
    Serial.print("NoMoveCount = ");
    Serial.println(NoMoveCount);
    // Compute one allowed step toward the desired position at the maximum slew rate.
    // Note Arduino max() and min() docs warn not to use any arithmetic inside.
    // One presumes they are macros. Thus this_move is used to hold intermediate
    // steps in the calculations.
    if (DesiredPosition > Feedback)
    {
      Serial.print("DesiredPosition > Feedback: Feedback + StepSize = ");
      //this_move = min(min(DesiredPosition, Feedback + StepSize), MaxPosition);
      this_move = Feedback + StepSize;
      Serial.print(this_move);
      this_move = min(DesiredPosition, this_move);
      Serial.print("min(DesiredPosition, this_move)");
      Serial.print(this_move);
      this_move = min(this_move, MaxPosition);
      Serial.print("this_move = min(this_move, MaxPosition) = ");
      Serial.println(this_move);
    }
    else
    {
      Serial.print("DesiredPosition <= Feedback: Feedback - StepSize = ");
      //this_move = max(MinPosition, max(DesiredPosition,  Feedback - StepSize));
      this_move = Feedback - StepSize;
      Serial.print(this_move);
      this_move = max(DesiredPosition,  this_move);
      Serial.print("max(DesiredPosition,  this_move)");
      Serial.print(this_move);
      this_move = max(MinPosition, this_move);
      Serial.print("this_move = max(MinPosition, this_move) = ");
      Serial.println(this_move);
    }
    apply_control(this_move);
    return;
  }

  // Set home position.
  void set_homing()
  {
    Serial.print("In I.set_homing ");
    Serial.print(Name);
    Serial.print(": HomePosition = ");
    Serial.println(HomePosition);
    DesiredPosition = HomePosition;
  }

  // Set min position -- used for calibration.
  void set_min()
  {
    Serial.print("In I.set_min ");
    Serial.print(Name);
    Serial.print(": MinPosition = ");
    Serial.println(MinPosition);
    DesiredPosition = MinPosition;
  }

  // Estimate control (DesiredPosition) value that would have produced a given feedback value.
  // This is from a standing start with the servos not moving -- so there is no way to account
  // for hysteresis as that depends on, e.g., which direction the servos were moving when the
  // system was shut down, whether the trike has been picked up and moved since then, whether
  // someone is sitting on it... The point is only to get a control value set on the control
  // pins before they're set as outputs.
  int estimate_control_from_feedback()
  {
    Serial.print("In I.estimate_control_from_feedback ");
    Serial.print(Name);
    Serial.print(": control = ");
    float control_est = FeedbackSlope * Feedback + FeedbackIntercept;
    // Round to nearest integer.
    int control = (int)(control_est + 0.5);
    Serial.println(control);
    return control;
  }

  // Initialize the control values to match the actual position of the actuators.
  // @ToDo: Do we ever call estimate_control_from_feedback apart from setting the controls?
  // If not, combine them.
  void initialize_control_from_feedback()
  {
    Serial.print("In I.initialize_control_from_feedback ");
    Serial.print(Name);
    Serial.print(": DesiredPosition = ");
    DesiredPosition = OldDesiredPosition = estimate_control_from_feedback();
    Serial.println(DesiredPosition);
    apply_control(DesiredPosition);
  }
} Instrument[NUM_INSTRUMENTS];

// @ToDo: Motor needs specialized versions of methods that compute and apply control values,
// update feedback, and handle step size (ramping up is constrained, but down is not, so
// StepSize is only used while ramping up).
// class _Motor : public _Instrument { };

// Note with subclasses, can't instantiate all the devices as above -- that produces only base
// classes. Simplest is to have an array of pointers to the instances, e.g.:
// _Instrument* Instrument[] = {
//   new _Instrument(...),
//   new _Instrument(...),
//   new _Motor(...) };
// Provide constructors so can avoid indexing into the array by named indices.
// If the classes are designed correctly, we should not need to know which is which outside
// of the classes, so the named indices would not be needed.

boolean all_close_enough()
{
  Serial.print("In all_close_enough: result = ");
  for (int i = 0; i < NUM_IN_USE; i++)
  {
    //if (!Instrument[IN_USE[i]].close_enough()) return false;
    if (!Instrument[IN_USE[i]].close_enough())
    {
      Serial.println("false");
      return false;
    }
  }
  Serial.println("true");
  return true;
}

// @ToDo:
// Represents the control source, either user or autonomous. Subclass for various forms
// of user control devices, or for autonomous.
/*
class _Control
{
 public:
  // ...
};
_Control* Control = new _Control();
*/

// Move all actuators to home positions. This just sets the state so that external control
// signals are ignored, then sets the desired position for each actuator to its home
// position. set_homing() would typically be called from setup or via an interrupt when
// about to shut down. The next_state should be OPERATING if home() is called during
// setup, or OFF if set_homing() is called when shutting down.
void set_homing(int next_state)
{
  Serial.print("In set_homing: next_state = ");
  Serial.println(next_state);
  NextState = next_state;
  State = HOMING;
  for (int i = 0; i < NUM_IN_USE; i++)
  {
    Instrument[IN_USE[i]].set_homing();
  }
}

// If homing, check if we've gotten there, and 
void check_homing()
{
  Serial.println("In check_homing:");
  if (all_close_enough())
  {
    Serial.print("Close enough, setting State = ");
    Serial.println(NextState);
    // All actuators are home -- go on to the real state.
    State = NextState;
  }
}

// Functions for calibration

int calibration_step = 1;   // Number the calibration steps.
int calibration_pass = 1;
int ramping_direction = 1;  // Move all the actuators in the same "direction": +1 up, -1 down.

// Set all to the minimum end of their position range, to prepare for calibration.
// This borrows the homing mechanism, but with different target positions.
void set_min()
{
  Serial.println("In set_min:");
  NextState = CALIBRATING;
  State = HOMING;
  for (int i = 0; i < NUM_IN_USE; i++)
  {
    Instrument[IN_USE[i]].set_min();
  }
}

// Check if we're at the end of a calibration move and ready to log info.
// Prompt the user for the measured position value by printing out the calibration data
// for this step. Wait til they type a response (ending with enter) on the serial line,
// to indicate they're ready to go to the next control value. Echo it back up the serial
// line so it gets logged.
void check_calibrating()
{
  Serial.println("In check_calibrating:");
  // Have the actuators reached their next measurement point?
  if (!all_close_enough()) return;

  // Write calibration info.
  Serial.print("Pass ");
  Serial.print(calibration_pass);
  Serial.print(", Step ");
  Serial.print(calibration_step);
  for (int i = 0; i < NUM_IN_USE; i++)
  {
    Serial.print(", ");
    Serial.print(Instrument[IN_USE[i]].Name);
    Serial.print(" ");
    Serial.print(Instrument[IN_USE[i]].Feedback);
  }
  
  // Wait for the user to say ok to move. Busy-wait until a char appears.
  Serial.println("Enter measurements:");
  while (Serial.available() < 0) delay(100);
  Serial.println("Got user input.");

  // Read all the queued-up chars into a string. Note not all chars in the line may get sent
  // in one packet, so wait for the newline.
  String line = "";
  while (true)
  {
    char c = (char)Serial.read();
    if (c > 0)
    {
      if (c == '\n') break;  // Stop when we get the end of the line.
      line += c;
    }
    else
    {
      while (Serial.available() < 0) {}
    }
  }
  // Echo user's text into the log.
  Serial.println(line);

  // Advance to next step.
  Serial.println("Advancing to next step.");
  if (++calibration_step > CALIBRATION_STEPS_PER_SWEEP)
  {
    // Done with a sweep.
    Serial.println("Done with a sweep.");
    if (++calibration_pass > NUM_CALIBRATION_SWEEPS)
    {
      // Quit after specified number of sweeps.
      Serial.println("Done.");
      set_homing(OFF);
      return;
    }
    // At end of each sweep, reverse.
    ramping_direction = -ramping_direction;
    calibration_step = 1;
  }

  // Compute next positions.
  for (int i = 0; i < NUM_IN_USE; i++)
  {
    Serial.print("Next position for ");
    Serial.print(Instrument[IN_USE[i]].Name);
    Serial.print(" = ");
    int next = Instrument[IN_USE[i]].CalibrationStepSize * ramping_direction;
    Serial.println(next);
    //Instrument[IN_USE[i]].update_control(Instrument[IN_USE[i]].CalibrationStepSize * ramping_direction, false, true);
    Instrument[IN_USE[i]].update_control(next, false, true);
  }
}

/*---------------------------------------------------------------------------------------*/ 
void setup()  
{
  Serial.begin(9600);

  Serial.println("In setup:");

  // @ToDo: Set these in constructor calls up above.
  Instrument[Motor].Name = "Motor";
  Instrument[Motor].EnablePin = EnableThrottle;
  Instrument[Motor].SignalPin = Throttle;
  //Instrument[Motor].FeedbackPin = FeedbackMotor;
  Instrument[Motor].CurrentPin = Current36V;
  Instrument[Motor].MaxSlewRate = 4000;
  Instrument[Motor].StepSize = Instrument[Motor].MaxSlewRate * LOOP_TIME_MS / 1000;  // = 20
  Instrument[Motor].CloseEnough = 1000;  // no feedback
  Instrument[Motor].StuckCount = 3;
  Instrument[Motor].NoMoveCount = 0;
  Instrument[Motor].Stuck = false;
  Instrument[Motor].MinPosition = 85;  // anolog read range is 0 to 1023
  Instrument[Motor].MaxPosition = 800;
  Instrument[Motor].FeedbackSlope = 0.0;  // placeholders
  Instrument[Motor].FeedbackIntercept = 0.0;

  Instrument[Steering].Name = "Steer";
  Instrument[Steering].EnablePin = EnableSteer;
  Instrument[Steering].SignalPin = Steer;
  Instrument[Steering].FeedbackPin = SteerFB;
  Instrument[Steering].CurrentPin = CurrentSteer;
  Instrument[Steering].MaxSlewRate = 4000;  // 2"/sec servo max slew rate.
  Instrument[Steering].StepSize = Instrument[Motor].MaxSlewRate * LOOP_TIME_MS / 1000;  // = 20
  Serial.print("Steer StepSize = ");
  Serial.println(Instrument[Steering].StepSize);
  Instrument[Steering].CloseEnough = 5;
  Instrument[Steering].StuckCount = 5;
  Instrument[Steering].NoMoveCount = 0;
  Instrument[Steering].Stuck = false;
  Instrument[Steering].MinPosition = 85;  // anolog read range is 0 to 1023
  Instrument[Steering].MaxPosition = 900;
  // These are based on a small number of values recorded in comments in other C2 scripts,
  // fitted to a line:
  // control = 0.13922 * feedback + 118.42
  // @ToDo: Replace by fit to larger set of values.
  Instrument[Steering].FeedbackSlope = 0.13922;
  Instrument[Steering].FeedbackIntercept = 118.42;

  Instrument[Brakes].Name = "Brake";
  Instrument[Brakes].EnablePin = EnableBrake;
  Instrument[Brakes].SignalPin = DiskBrake;
  Instrument[Brakes].FeedbackPin = BrakeFB;
  Instrument[Brakes].CurrentPin = CurrentBrake;
  Instrument[Brakes].MaxSlewRate = 4000;  // 2"/sec servo max slew rate.
  Instrument[Brakes].StepSize = Instrument[Motor].MaxSlewRate * LOOP_TIME_MS / 1000;  // = 20
  Serial.print("Brake StepSize = ");
  Serial.println(Instrument[Brakes].StepSize);
  Instrument[Brakes].CloseEnough = 5;
  Instrument[Brakes].StuckCount = 5;
  Instrument[Brakes].NoMoveCount = 0;
  Instrument[Brakes].Stuck = false;
  Instrument[Brakes].MinPosition = 85;  // anolog read range is 0 to 1023
  Instrument[Brakes].MaxPosition = 800;
  Instrument[Brakes].FeedbackSlope = 0.13922;  // @ToDo: these are for steering servo
  Instrument[Brakes].FeedbackIntercept = 118.42;

  // Wait a short while to let the feedback settle as power comes up.
  //delay(5000);
  delay(100);

  // Read the feedback values first before applying output. Estimate the control value that
  // should produce the observed feedback, and use that as the starting control value so the
  // servos do not move too quickly.
  Serial.println("Reading initial feedback.");
  for (int i = 0; i < NUM_IN_USE; i++)
  {
    Instrument[IN_USE[i]].initialize_feedback();
    Instrument[IN_USE[i]].initialize_control_from_feedback();
  }

  Serial.println("Setting control pins to output.");
  pinMode(Instrument[Steering].SignalPin, OUTPUT);
  pinMode(Instrument[Brakes].SignalPin, OUTPUT);
  
  // Compute calibration step size from allowed ranges.
  for (int i = 0; i < NUM_IN_USE; i++)
  {
    Serial.print("Calibration step size for ");
    Serial.print(Instrument[IN_USE[i]].Name);
    Serial.print(" = ");
    Instrument[IN_USE[i]].CalibrationStepSize =
      abs(Instrument[IN_USE[i]].MaxPosition - Instrument[IN_USE[i]].MinPosition) / CALIBRATION_STEPS_PER_SWEEP;
    Serial.println(Instrument[IN_USE[i]].CalibrationStepSize);
  }

  // Set up to home the actuators, then run calibration.
  set_homing(CALIBRATING);
  
  // As close to the end of setup as possible, set the first loop start time.
  next_loop_time_ms = millis() + LOOP_TIME_MS;
}

/*---------------------------------------------------------------------------------------*/
void loop() 
{
  Serial.println("In loop:");
  // Check if we need to pause until time to run this loop pass.
  // Note to self: times are unsigned -- beware subtraction.
  current_time_ms = millis();
  if (current_time_ms < next_loop_time_ms)
  {
    // Not at next loop time yet -- pause.
    pause_time_ms = next_loop_time_ms - current_time_ms;
    Serial.print("pause_time_ms = ");
    Serial.println(pause_time_ms);
    delay(pause_time_ms);
  }
  else
  {
    Serial.println("No need to pause.");
  }
  next_loop_time_ms += LOOP_TIME_MS;

  // Do State update checks.
  // @ToDo: Refactor to use a State class. If we have a subclass for each specific state with
  // appropriate specialized methods, we won't need tests for which state we're in. E.g.
  // could replace check_homing() and check_calibrating() by a check_state().
  
  // If we're homing, are we there yet?
  if (State == HOMING)
  {
    check_homing();
  }

  // If we're calibrating...
  if (State == CALIBRATING)
  {
    check_calibrating();
  }

  // Did source of control change?
  // @ToDo: Add switch between auto and user control.
  
  // Collect inputs: User control values, feedback, currents, other sensor data,...
  // Update actuator feedback values. Do this all together so as to capture readings
  // as close together in time as possible.
  for (int i = 0; i < NUM_IN_USE; i++)
  {
    Instrument[IN_USE[i]].update_feedback();
  }

  // Compute and apply controls.
  for (int i = 0; i < NUM_IN_USE; i++)
  {
    Instrument[IN_USE[i]].move_position();
  }
  
  // Write any debug info to serial.
}

