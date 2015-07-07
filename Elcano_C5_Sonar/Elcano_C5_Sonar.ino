/*  SONAR BOARD

    The board uses an Arduino Micro, with interrupts on digital pins 0, 1, 2, 3 and 7.
    
    The interrupt numbers are 2, 3, 1, 0 and 4 respectively. 
*/

/*  Two boards can hold 12 sonars on the points of a clock.
    The front board holds Sonar 12 (straight ahead)
    and 1, 2, and 3F to the right.
    It also holds 9F, 10, and 11 to the left.
    The rear facing board holds 3R through 9R, with 6R pointing back.
   
    The front sonar board uses the SPI interface to appear as a slave to a host computer.
    When the host asks for sonar readings, the front board sends all data.

    Two board operation is not yet implemented.   
    The front board controls the rear board over an I2C interface. It will intruct the rear 
    board to take readings when the front board is taking readings. 
    The hardware is identical for the front and rear boards.
   
    There may be no rear board present.  In this case, the front board may use the 6F socket.
   
    Sonars are grouped into three rounds so that multiple sonars can fire simultaneously and
    do not intefere with each other. Rounds are selected by the (S_DEC1, S_DEC2) signal, 
    which is used as a Gray code (only one bit changes at a time).
    The 74139 2-to-4 line decoder chip selects outputs Y0,Y1,Y2 and Y3 based on inputs a and b
    Y1, Y2 and Y3 are active low, so they are passed through the SN74LVC3G14DCTR inverter to be
    active high. 
    These signals go to the SN74LVCH8T245PWR transceiver, which enables the appropriate sonars.

    Round(AB)   Output     Sonars
    00          Y0         none
    01          Y1         12, 3, 9
    11          Y2         1, 10     // original design included 6
    10          Y3         2, 11, 6  // 6 not in original design

    A round may fire up to 3 sonars. The sonar range may be reported as either an analog value
    or as a pulse width. A pulse width will cause an interrupt.
    There are three interrupt handlers, one for each sonar fired in the round.
*/

// ###########  TO DO: communication with host over SPI  ###########################################
//  SS      Chip select from host processor
//  MOSI    This processor's serial input;  Not yet used !!!!
//  MISO    This processor's serial output
// #################################################################################################


#define cbi(port, bit) (port &= ~(1 << bit))    //Clear a bit
#define sbi(port, bit) (port |= (1 << bit))     //Set a bit

//When DEBUG_MODE is true, send data over Serial, false send data over SPI
#define DEBUG_MODE      true

//When sedning over Serial, true gives verbose output
#define VERBOSE_DEBUG   false
// -------------------------------------------------------------------------------------------------

#define PW_OR3        7     //Pulse 9, 10 or 11 ====>> (actually pulse 1, 2, or 3 - Tai B.)
#define IRQright_ID   4     //The interrupt ID relating to pin above

#define PW_OR1        3     //Pulse on 6 or 12
#define IRQcenter_ID  0     //The interrupt ID relating to pin above

//If using I2C to communicate with rear board, its SDA will need to share pin 2 with PW_OR2
#define PW_OR2        2     //Pulse on 1, 2, or 3; or I2C. (actually pulse on 9, 10, or 11 - Tai B.)
#define IRQleft_ID    1     //The interrupt ID relating to pin above
// -------------------------------------------------------------------------------------------------

//S-DECx pins good - Tai B.
#define S_DEC1        5     //Low order bit for selecting the round
#define S_DEC2        6     //Hi bit of round select

//POWER good - Tai B.
#define V_EN         13     //Apply power to sonars
#define V_TOGGLE      4     //(V_TOGGLE) HIGH for 3.4v, LOW for 5v sonar power

#define F_BSY        11     //pin signals to rear sonar board       IMPORTANT - FBSY should be pin  11 was 9
#define R_BSY         8     //pin signals from rear sonar bord      IMPORTANT - RBSY should be pin  8 was 10
// -------------------------------------------------------------------------------------------------

#define DEBUG_PIN   A0      //Light-emitting diode indicator for DEBUG_status = TRUE

//Analog input pins from sonars           
// #define ANx  Ay // x is clock position; Ay is the analog input channel
#define AN12        A1
#define AN11        A2
#define AN10        A3
#define AN9         A4
#define AN6         A5
#define AN3         A9      /* on D9 */
#define AN2        A10      /* on D10 */
#define AN1        A11      /* on D11 */
// -------------------------------------------------------------------------------------------------

#define TIMEOUT_PERIOD  100 //ms: 20.5ms for calc + up to 62ms for the reading + a little buffer
#define ROUND_DELAY     100 //ms between rounds, seems to help stability

//Modify EXPECTED_SIGNALS to match which sonars are on the board.
#define EXPECTED_SIGNALS1   3
#define EXPECTED_SIGNALS2   3
#define EXPECTED_SIGNALS3   2
#define COMMAND_GO          1
#define RANGE_DATA_SIZE     13

#include "pins_arduino.h"

enum STATE { INITIAL, READY, ROUND1, ROUND2, ROUND3 };

STATE boardState = INITIAL;     //initializing sonar board state

volatile byte SignalsReceived;
volatile unsigned long timeLeft;
volatile unsigned long timeRight;
volatile unsigned long timeCenter;

volatile unsigned long timeStart;
// volatile unsigned long timeLeftStart;        //currently not being used
// volatile unsigned long timeRightStart;       //currently not being used
// volatile unsigned long timeCenterStart;      //currently not being used

volatile boolean timeStartSet;
// volatile boolean timeLeftStartSet;           //currently not being used
// volatile boolean timeRightStartSet;          //currently not being used
// volatile boolean timeCenterStartSet;         //currently not being used

//For SPI communications
volatile unsigned long timeWriteStart;
volatile byte valueIn;
volatile boolean processIt;

int range[RANGE_DATA_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int analogRange[RANGE_DATA_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int roundCount, timeoutStart, timeSinceLastRound;

volatile int isr1Count, isr2Count, isr3Count;

const byte BoardCount = 1;      //just the front board
const int SCALE_FACTOR = 58;    //to calculate the distance, use the scale factor of 58us per cm
const int BAUD_RATE = 115200;


// -------------------------------------------------------------------------------------------------
void setup()
{
    //DEBUG LED test
    pinMode(DEBUG_PIN, OUTPUT);

    if (DEBUG_MODE)
    {
        digitalWrite(DEBUG_PIN, HIGH);
    }
  
    //Serial Peripheral Interface (SPI) Setup
    pinMode(MISO, OUTPUT);

    //SPI Control Register (SPCR)
    SPCR |= _BV(SPE);   //Turn on SPI in slave mode (SPI Enable)
    SPCR |= _BV(SPIE);  //Turn on interrupts for SPI (SPI Interrupt Enable) 

    processIt = false;  //False until we have a request to process
  
    // timeLeftStart = timeRightStart = timeCenterStart = 0;
    timeLeft = timeRight = timeCenter = timeStart = 0;
    roundCount = timeoutStart = timeSinceLastRound = 0;
    isr1Count = isr2Count = isr3Count = 0;

    timeLeftStartSet = false;
    timeRightStartSet = false;
    timeCenterStartSet = false;
    timeStartSet = false;

    //Baud rate configured for 115200
    //It communicates on digital pins 0 (RX) and 1 (TX) as well as with the computer via USB
    Serial.begin(BAUD_RATE);
 
    pinMode(S_DEC1, OUTPUT);        //Pin 5 (PWM)       --> SD1
    pinMode(S_DEC2, OUTPUT);        //Pin 6 (PWM/A7)    --> SD2

    //NOTE -- Pins below are default to inputs, so they don't need to be explicitly declared as inputs with pinMode()
    pinMode(PW_OR1, INPUT);         //Pin 3 (PWM/SCL)   --> PW1
    pinMode(PW_OR2, INPUT);         //Pin 2 (SDA)       --> PW2
    pinMode(PW_OR3, INPUT);         //Pin 7             --> PW3


    boardState = INITIAL;  
    BoardCount = 1;

    digitalWrite(S_DEC1, LOW);
    digitalWrite(S_DEC2, LOW);

    digitalWrite(F_BSY, HIGH); //Signal rear board that we are here; See if there is a rear board
  
    //Select 5V power(LOW), or 3.8V power(HIGH)
    digitalWrite(V_TOGGLE, LOW);    //Pin 4 (A6)        --> VTOG

    //Turn on sonars
    digitalWrite(V_EN, HIGH);
    

    //for (int i = 0; i < 1000; i++)
    //{  
    //    //See if there is a second board
    //    byte Rear = digitalRead(R_BSY);
    //    if (Rear == HIGH)
    //    {
    //        BoardCount = 2;
    //        //Need to dynamically attach/detacy PW_OR2 so that it is shared between IntRight and I2C
    //        break;
    //    }
    //}

    //Required delay above 175ms after power-up with the board for all XL-MaxSonars to be ready
    delay(200); //Per sonar datasheet, needs 175ms for boot

    attachInterrupt(IRQleft_ID, IsrLeft, CHANGE);
    attachInterrupt(IRQcenter_ID, IsrCenter, CHANGE);
    attachInterrupt(IRQright_ID, IsrRight, CHANGE);

    digitalWrite(F_BSY, LOW); //Open for business
    boardState = READY;
}


// -------------------------------------------------------------------------------------------------
//SPI interrupt routine
ISR (SPI_STC_vect) 
{
    valueIn = SPDR;     //Grab byte from SPI Data Register
    processIt = true;
}


// -------------------------------------------------------------------------------------------------
void IsrLeft()
{  
    //ISR for end of pulse on sonar 9, 10 or 11    
    noInterrupts();

    isr1Count++;

    //Checks the PWM from PW_OR3 is HIGH
    if (digitalRead(PW_OR3) == HIGH && !timeStartSet)
    {
        timeStart = micros();        
        timeStartSet = true;
    }
    else
    {
        timeLeft = micros();        
        SignalsReceived++;
    }
    interrupts();
}


// -------------------------------------------------------------------------------------------------
void IsrCenter()
{  
    //ISR for end of pulse on sonar 6 or 12
    noInterrupts();

    isr2Count++;

    if (digitalRead(PW_OR1) == HIGH && !timeStartSet)
    {
        timeStart = micros();        
        timeStartSet = true;
    }
    else
    {
        timeCenter = micros();        
        SignalsReceived++;
    }

    interrupts();
 }
 

// -------------------------------------------------------------------------------------------------
void IsrRight()
{
    //ISR for end of pulse on sonar 1, 2 or 3
    //Log time of the pulse end

    noInterrupts(); 

    isr3Count++; 

    if (digitalRead(PW_OR2) == HIGH && !timeStartSet)
    {
        timeStart = micros();        
        timeStartSet = true;
    }
    else
    {
        timeRight = micros();        
        SignalsReceived++;
    }
    interrupts();
}


// -------------------------------------------------------------------------------------------------
void loop()
{
    boardState = READY;

    digitalWrite(S_DEC1, LOW);
    digitalWrite(S_DEC2, LOW);   // superfluous
    //readInput(); //Sonar is slave waiting for command from master
    //TO DO: Send start signal to rear board.
    
//****** ROUND 1 ***************************************************************
    boardState = ROUND1;  
    SignalsReceived = 0;
    //Send a pulse on sonars 
    interrupts();    
    //digitalWrite(S_DEC2, HIGH);
    setRound(0, 1);
 
    timeoutStart = millis();
    while (SignalsReceived < EXPECTED_SIGNALS1) 
    {
        if ((millis() - timeoutStart) > TIMEOUT_PERIOD) { break; }
    }
    delay(ROUND_DELAY);

    //==== extra interrupt data holders for timing tests ========
    //    timeLeftStartSet = false;
    //    timeRightStartSet = false;
    //    timeCenterStartSet = false;
    //===========================================================

    timeStartSet = false;

    range[12] = (timeCenter - timeStart) / SCALE_FACTOR;
    range[3]  = (timeRight - timeStart) / SCALE_FACTOR;
    range[9]  = (timeLeft - timeStart) / SCALE_FACTOR;

    analogRange[12] = analogRead(AN12);
    analogRange[3] =  analogRead(AN3);
    analogRange[9] =  analogRead(AN9);

 
//****** ROUND 3 ***************************************************************
    // Gray code; only one bit changes at a time
    boardState = ROUND3;
    SignalsReceived = 0;       
    //Send a pulse on sonars 
    //digitalWrite(S_DEC1, HIGH);
    setRound(1, 1);
    
    timeoutStart = millis();
    while (SignalsReceived < EXPECTED_SIGNALS3) 
    {
        if ((millis() - timeoutStart) > TIMEOUT_PERIOD) { break; }
    }
    delay(ROUND_DELAY);

    //==== extra interrupt data holders for timing tests ========
    //    timeLeftStartSet = false;
    //    timeRightStartSet = false;
    //    timeCenterStartSet = false;
    //===========================================================
    timeStartSet = false;

    range[10] = (timeLeft - timeStart) / SCALE_FACTOR;
    range[1]  = (timeRight - timeStart) / SCALE_FACTOR;

    analogRange[10] = analogRead(AN10);
    analogRange[1] =  analogRead(AN1);
   
    
//****** ROUND 2 ***************************************************************
    boardState = ROUND2;
    SignalsReceived = 0; 
    //Send a pulse on sonars 
    //digitalWrite(S_DEC2, LOW);
    setRound(1, 0);
    
    timeoutStart = millis();
    while (SignalsReceived < EXPECTED_SIGNALS2) 
    {
        if ((millis() - timeoutStart) > TIMEOUT_PERIOD) { break; }
    }
    delay(ROUND_DELAY);

    //==== extra interrupt data holders for timing tests ========
    //    timeLeftStartSet = false;
    //    timeRightStartSet = false;
    //    timeCenterStartSet = false;
    //===========================================================

    timeStartSet = false;

    range[11] = (timeLeft - timeStart) / SCALE_FACTOR;
    range[2]  = (timeRight - timeStart) / SCALE_FACTOR;
    range[6]  = (timeCenter - timeStart) / SCALE_FACTOR;

    analogRange[11] = analogRead(AN11);
    analogRange[2] =  analogRead(AN2);
    analogRange[6] =  analogRead(AN6);


    //TO DO: Receive data from rear board
    writeOutput();
    boardState = READY;
    roundCount++;
}


// -------------------------------------------------------------------------------------------------
void setRound(int highOrderBit, int lowOrderBit)
//we should rewrite this method to manipulate the digital pin registers rather that using
//digitalWrite to toggle RND pins.  This way the pins will change simultaneously, which
//will create more reliable functionality -- tb

// //@@@@@@@@@@@  S-DECx pins good -- tb @@@@@@@@@@@@@@@@@@
// #define S_DEC1     5 //Low order bit for selecting the round
// #define S_DEC2     6 //Hi bit of round select
// //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

{
    if (highOrderBit == 0 && lowOrderBit == 0)
    {
        //Turn off all sonars
        cbi(PORTC, PORTC6);     // digitalWrite(S_DEC1, LOW);
        cbi(PORTD, PORTD7);     // digitalWrite(S_DEC2, LOW);
    }

    else if (highOrderBit == 0 && lowOrderBit == 1)
    {
        // 12, 3, 9
        cbi(PORTD, PORTD7);     // digitalWrite(S_DEC2, LOW); 
        sbi(PORTC, PORTC6);     // digitalWrite(S_DEC1, HIGH);

        delayMicroseconds(50);

        cbi(PORTC, PORTC6);     // digitalWrite(S_DEC1, LOW);
    }

    else if (highOrderBit == 1 && lowOrderBit == 0)
    {
        // 2, 11, 6
        cbi(PORTC, PORTC6);     // digitalWrite(S_DEC1, LOW);
        sbi(PORTD, PORTD7);     // digitalWrite(S_DEC2, HIGH);

        delayMicroseconds(50);

        cbi(PORTD, PORTD7);     // digitalWrite(S_DEC2, LOW);
    }

    else if (highOrderBit == 1 && lowOrderBit == 1)
    {
        // 1, 6, 10
        sbi(PORTC, PORTC6);     // digitalWrite(S_DEC1, HIGH);
        sbi(PORTD, PORTD7);     // digitalWrite(S_DEC2, HIGH);

        delayMicroseconds(50);

        cbi(PORTC, PORTC6);     // digitalWrite(S_DEC1, LOW);
        cbi(PORTD, PORTD7);     // digitalWrite(S_DEC2, LOW);
    }
}


// -------------------------------------------------------------------------------------------------
void readInput()
{ 
    byte select;
    //Wait for host to request information
    do 
    {
        select = digitalRead(SS);
    } while (select == HIGH);

    if (DEBUG_MODE) { return; }

    while (!processIt || valueIn != COMMAND_GO) { } //Stay in routine until commanded to start on MOSI
    processIt = false;
}


// -------------------------------------------------------------------------------------------------
void writeOutput()
{
    if (DEBUG_MODE) 
    {
        if (VERBOSE_DEBUG) 
        {
            Serial.print("Round: ");
            Serial.print(roundCount);
            Serial.print(" micros: ");
            Serial.print(micros());
            Serial.print(", micros since last: ");
            Serial.print(micros() - timeSinceLastRound);
            timeSinceLastRound = micros();
            Serial.print(", SigReceived: ");
            Serial.print(SignalsReceived);
            Serial.print(", isr1: ");
            Serial.print(isr1Count);
            Serial.print(", isr2: ");
            Serial.print(isr2Count);
            Serial.print(", isr3: ");
            Serial.println(isr3Count);
            Serial.print(" Vals: ");
        }

        for (int index = 1; index < RANGE_DATA_SIZE; ++index)
        {
            //Prints out only the sonars for one board (9, 10, 11, 12, 1, 2, 3)
            if (BoardCount == 1 && (index == 4 || index == 5 || index == 7 || index == 8)) { continue; }

            if (index < 10) { Serial.print("[ "); }
            else { Serial.print('['); }
                
            Serial.print(index);
            Serial.print("] ");
            Serial.println(range[index]);
            // Serial.println(analogRange[index]);
        }
        Serial.println();
    }
    else 
    {
        //Here is where data will be sent over SPI. Still needs work
        int dataPos = 0;
        processIt = false;
        while (dataPos < RANGE_DATA_SIZE) 
        {
            timeWriteStart = millis();

            while (!processIt) 
            {
                if (millis() - timeWriteStart > TIMEOUT_PERIOD) { break; }
            }

            writeIntData(range[dataPos++]);
            delay(20);
        }

        //NOTE -- Why do I need to set dataPos back to 0 if it will be deleted from the stack later on?
        dataPos = 0;
    }
}


// -------------------------------------------------------------------------------------------------
// Send two bytes over SPI in order to represent an int
void writeIntData(int data)
{
    byte highOrderByte = (byte) (data >> 8);
    byte lowOrderByte = (byte) data;
  
    // Send first byte
    SPDR = highOrderByte;
    processIt = false;
  
    // Wait for SPI interrupt before sedning second byte
    while(!processIt) 
    {
        if (millis() - timeWriteStart > TIMEOUT_PERIOD) { break; }
    }

    //Send second byte
    SPDR = lowOrderByte;
    processIt = false;
}
