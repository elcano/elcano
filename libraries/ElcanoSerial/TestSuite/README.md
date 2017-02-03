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

From the Arduino IDE's serial monitor, send a message around, using that one's
capture if you want to test an entire loop, and another's if you want to test
the connection between one node and the next.

