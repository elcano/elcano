Elcano Serial
=============

This document should serve as a complete guide to working with the `ElcanoSerial` library, the backbone of interdevice communication in the Elcano project.

1. [Abstract](#abstract)
2. [Data Format](#data-format)
3. [More on Message Types](#more-on-message-types)
4. [Usage](#usage)
5. [Limitations](#limitations)

Abstract
--------

The library is meant to send a packet of data from one Arduino in the system to the next over serial connections.

When a connection is normally made, the Arduino must block until it has finished reading a continuous block of text. However, as these are nearly-real time embedded systems, we cannot do this, and it must read the data asyncronously.

The way that we chose to do this was with a state machine that can update one character at a time without loosing track of where it is in the processing stage. As a result, the data is updated in such a way that no blocking is required between reads, and if a batch of data is not fully read at the right time, the device can come back and keep going later.

The `ParseState` data structure contains a pointer to the resulting data (where it places information it has read from the input), and a flag that tells the state machine what part of the structure it is currently at.

The Arduinos are arranged into a ring, so that all data will eventually reach its target, as Arduinos pass data they don't need back into the ring.

Data Format
-----------

A PEG for the data that gets passed around is as follows:

    ('D' / 'S' / 'G' / 'X') ('{' ('n' / 's' / 'a' / 'b' / 'r') '-'? [0-9]+ '}')+
    \--------header-------/ \----------------------body------------------------/
                                 \---------attribute---------/ \--value--/

The varios components of the string have been labeled for your convenience.

In the header, we see the four types of messages that can be passed:

- **D** for _drive_
- **S** for _sensor_
- **G** for _goal_
- **X** for _segment_

In the attribute, we see the 5 types of numbers that can be passed per message:

- **n** for _number_
- **s** for _sensor_
- **a** for _angle_
- **b** for _bearing_
- **r** for _probability_

There is actually a _6th_ attribute that can be passed, **p** for _position_, and it sends a _pair_ of numbers. Replace the body section of the PEG with this:

    '{' 'p' '-'? [0-9]+ ',' '-'? [0-9]+ '}'

Any spaces sent are ignored by the parser. Given all of the above, a complete PEG would look like the following:

```
Message = ('D' / 'S' / 'G' / 'X') ('{' (
    ('n' / 's' / 'a' / 'b' / 'r') '-'? [0-9]+
  / 'p' '-'? [0-9]+ ',' '-'? [0-9]+
) '}')+
```

This may be of use to you if you need to re-implement this in the future.

More on Message Types
---------------------

There are 4 types of messages that may be circulated through the system, as mentioned above. Each of them has a specific purpose, and only uses a portion of the available data options.

A *drive* message is used to send data to C2, and requires the usage of two flags:
- `speed_cmPs`
- `angle_deg`

A *sensor* command is used to send data to C3 and C6, and requires the usage of 4 flags:
- `speed_cmPs`
- `angle_deg`
- `posE_cm` and `posN_cm`
- `bearing_deg`

A *goal* command is used to communicate the location of an object to C4, and requires the usage of 3 flags, with an optional 4th:
- Number
- Position
- Bearing
- Probability (optional)

A *segment* command is used to communicate part of the navigation path to C3, and requires the usage of 4 flags:
- Number
- Position
- Bearing
- Speed

This is internally verified to ensure that the types have the correct flags. If they do not, `ParseState::update()` returns `ParseStateError::inval_comb`, which you can check for in your `loop()`.

Usage
-----

Here is some example code, which you can use as a skeleton for writing your own stuff.

```c++
elcano::ParseState ps;
elcano::SerialData dt;

void setup() {
  Serial1.begin(elcano::baudrate);
  Serial2.begin(elcano::baudrate);

  ps.dt     = &dt;
  ps.input  = &Serial1;
  ps.output = &Serial2;
  ps.capture = elcano::MsgType::DESIRED_TYPE;
  dt.clear();

  // Any other initialization code goes here
}

void loop1() {
  // Update code here that depends on having recieved a data set
}

void loop2() {
  // Update code here that does not depend on having received a data set
}

void loop() {
  elcano::ParseStateError r = ps.update();
  if (r == elcano::ParseStateError::success) {
    loop1();
  }

  loop2();
}
```

Replace `DESIRED_TYPE` with `drive`, `sensor`, `goal`, or `seg` depending on your component. The specific serial port used by your component may vary, so you may have to change `Serial1` and `Serial2` around.

The error handling code in the skeleton is enough to get you started, but may be a little bit lacking if you want more fine grained control, especially if you are debugging. That can be accomplished with a switch statement in the loop function:

```c++
void loop() {
  elcano::ParseStateError r = ps.update();
  switch (r) {
  case elcano::ParseStateError::success:
    loop1();
    break;
  <other cases go here>
  }

  loop2();
}
```

Limitations
-----------

The Arduino has a hard limit of 32 characters which it can store at a time from the serial port. It can also only write to the serial far faster than it can read and parse. This combined leads to a situation where if you write too quickly, you will overwhelm the reader and get garabage results. This can be countered in 2 ways:

1. Put a wait between writes. Hold on, I hear you say! Isn't the point of the asyncronous system that no blocking is required? The difference between this and naive blocking serial usage is that this blocks on _write_, while the other blocks on _read_, and this is purely due to the memory constrainsts on the Arduinos.

2. Only write to the message loop after you have just recieved from the message loop. Based on our usage example above this, you would only write in `loop1` and not in `loop2`. This ensures that nothing gets written until something is recieved, which forces all delay necessary to prevent a problem.

If at some point in the future, the hardware gets updated to something without the hard Arduino requirements, then feel free to experiment with removing these limitations.
