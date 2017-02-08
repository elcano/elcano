
volatile long elapsed[7];
volatile long temp[7];
volatile bool done[7];

volatile byte oldPINK;

void setup () {
    Serial . begin (9600);
    // Configure Input/Output Pins on port K
    DDRK = 0b00001111;
    // Set pull up resistors on port K input pins, and set outputs low
    PORTK = B11111111;
    // Configure interrupts
    // PCICR - Pin Change Interrupt Control Register, 15.2.5,
    // Enable PCIE2 to enable PCINT23:16
    PCICR = 0b00000100;
    // PCMSK2 - Pin Change Mask Register 2, 15.2.7, Enable each pin PCINT23:16
    PCMSK2 = 0b11110000;
    // enables interrupts
    sei();
}
// Loop to continuously poll each row individually
void loop () {
    
    if(done[1])
    { 
//      Serial.println(elapsed[1]);
    }
    PORTK = B11111110;  // to do: remove writes to PORTK
    delay (1);
    PORTK = B11111101;
    delay (1);
    PORTK = B11111011;
    delay (1);
    PORTK = B11110111;
    delay (1);
    // to do: if a key has been pressed, display it.
}
// Interrupt function, detects pin change on PCINT23:20
ISR(PCINT2_vect) {
//    Serial.println(PINK, BIN);
    byte lowNibble , highNibble;
    lowNibble = PINK >> 4;
    highNibble = PINK & 0xF;
    byte high;
    byte low;
    bool valid = 
      (highNibble == B1101 || 
       highNibble == B1011 || 
       highNibble == B0111)
       &&
       (lowNibble == B1110 ||
       lowNibble == B1101  || 
       lowNibble == B1011  || 
       lowNibble == B0111);
    valid = true;
    noInterrupts();
    if (valid)
    { 
      Serial.println(getBit(PINK, 1));
      if(getBit(oldPINK, 1) == 0 && getBit(PINK, 1) == 1) Serial.println("RISING");
      if(getBit(oldPINK, 1) == 1 && getBit(PINK, 1) == 0) Serial.println("FALLING");
      oldPINK = PINK;
    }
    interrupts();

}
bool getBit(byte b, int pos)
{
  return (b >> pos) & 0x1;
}

