/*
Test board connectivity by driving one end of each board trace in a pattern,
and checking for the same value on the other end.

We'll use one or more Arduino Megas as test controllers.  In each, we will...

Identify one connector on each PC board trace as an "input" to the system under
test (SUT).  Identify the remaining connectors on that same trace as outputs for
that trace.

Setup:

Connect one Arduino output to each of the SUT "inputs".  Connect one Arduino input to
each of the SUT "outputs".  These connections can be made by way of the breadboard.

Set all the Arduino outputs to tri-state, output.
Set all the Arduino inputs to pull-up, input.

Test procedure:

Set one Arduino output low.  (Others remain tri-stated).
Check that the Arduino inputs on the same trace are low and the rest high.

Go on to the next output.

Be very careful that only one pin is driven low at a time, and that the rest are
tri-stated.  Otherwise -- if we were to set the other traces to high, and there
was a solder bridge, it would short the driving pin to power.
*/

// Represent one connection on the PC board and its corresponding Arduino
// pin as the Arduino pin number and the name of the connection on the
// PC board.  Names of connections have the format:
// <Component name>_<Pin name>
// (Component == one item with pin names, e.g. a header or chip.)
// For now, we are using names from the spreadsheet, but we are unable to
// locate these on the schematic.
class Connection {
  public:
  char* name;   // Connection name formatted as described above.
  uint8_t pin;  // Pin on the Arduino conducting the test.
  Connection(): name(0), pin(0) {}
  Connection(char* n, uint8_t p) {
    name = n;
    pin = p;
  }
};

// A trace on the PC board may have more than two connection points.  We want
// to test all of them, so will drive one connection and check the signal at
// the others.  Represent a trace as a struct with one Arduino pin as output
// and a list of Arduino pins as inputs.
class Trace {
  public:
  char* trace_name;                 // Trace name on the spreadsheet / schematic.
  Connection* pin_out;        // Connection that will drive the trace.
  Connection** pin_in_list;  // The other connections on the trace.
  Trace(char* n, Connection* po, Connection** pl) {
    trace_name = n;
    pin_out = po;
    pin_in_list = pl;
  }
};

// Declare traces
Connection C6_A04("C6_A04", A0);
Connection C4_R09("C4_R09", 13);
Connection C3_R09("C3_R09", 14);
Connection* c1[] = {&C4_R09, &C3_R09};
Trace R_RNG("R_RNG", &C6_A04, c1);
Connection C6_A05("C6_A05", A1);
Connection X1_7("X1_7", 15);
Connection* c2[] = {&X1_7};
Trace CRS_BRK("CRS_BRK", &C6_A05, c2);
Connection C6_A06("C6_A06", A2);
Connection X1_6("X1_6", 16);
Connection* c3[] = {&X1_6};
Trace CRS_STR("CRS_STR", &C6_A06, c3);

// List of traces.
Trace traces[] = {
  R_RNG,
  CRS_BRK,
  CRS_STR
};

int num_traces = sizeof(traces) / sizeof(struct Trace);

void set_pins(Connection** clist, int value) {
  int num_pins = sizeof(clist) / sizeof(Connection*);
  for (int i=0; i < num_pins; i++) {
    Connection* c = clist[i];
    pinMode(c->pin, INPUT_PULLUP);
  }
}

// Initialize the pins.
void setup() {
  for (int i=0; i < num_traces; i++) {
    Trace trace = traces[i];
    pinMode((trace.pin_out)->pin, INPUT);  // INPUT is tri-stated
    Connection** other_ends = trace.pin_in_list;
    set_pins(other_ends, INPUT_PULLUP);
    /*
    int num_ends = sizeof(other_ends) / sizeof(Connection*);
    for (int i=0; i < num_ends; i++) {
      Connection* other_end = other_ends[i];
      pinMode(other_end->pin, INPUT_PULLUP);
    }
    */
  }
}

void loop() {
  for (int i=0; i < num_traces; i++) {
    struct Trace trace = traces[i];
    uint8_t pin = (trace.pin_out)->pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
    delay(1);
    // Check that the current trace ends are low.
    
    // Check that the other trace ends are high.
    
    // Put the test connections back as they were.
    pinMode((trace.pin_out)->pin, INPUT);
  }
  
  // Could wait here for serial input -- we don't need to loop.
}
