`ElcanoSerial` Test Suite
=========================

This directory includes a series of Arduino projects which get used to test the
`ElcanoSerial` library.

To setup, do the following:

- [ ] Obtain three Arduinos
- [ ] Install the three projects onto them (leave them connected to hosts)
- [ ] Connect C1's `Serial2` output to C2's `Serial1` input
- [ ] Connect C2's `Serial2` output to C3's `Serial1` input
- [ ] Connect C3's `Serial2` output to C1's `Serial1` input
- [ ] Wire together all of the Arduino's grounds, to eliminate static

To test it, monitor C1, which has some special code for collecting time
measurements and sending data from the serial monitor. It will be your primary
interface into the testing structure.

Send a _drive_ packet to send a packet that will trigger the `passthru` feature
of the library, and will be the lower bound on how quick data can be passed
through the ring.

Send a _goal_ packet to send a packet that will trigger all three nodes in the
loop to read and write data, which should be the maximum amount of time it
takes to circle the loop.

This also leaves you with a skeleton to test any further changes to the library
made in the distant future, as it is a "complete" application that uses it.
