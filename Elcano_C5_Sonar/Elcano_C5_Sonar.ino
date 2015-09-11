/*  SONAR BOARD

    The board uses an Arduino Micro, with interrupts on digital pins 0, 1, 2, 3 and 7.
    
    The interrupt numbers are 2, 3, 1, 0 and 4 respectively. 

    Two boards can hold 12 sonars on the points of a clock.
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
    01          Y1         9, 12, 3
    11          Y2         6, 11, 2
    10          Y3         1, 10 

    A round may fire up to 3 sonars. The sonar range may be reported as either an analog value
    or as a pulse width. A pulse width will cause an interrupt.
    There are three interrupt handlers, one for each sonar fired in the round.
*/


#define BAUD_RATE      115200   //Serial Baud Rate for debugging
#define DEBUG_MODE       true   //When DEBUG_MODE is true, send data over Serial, false send data over SPI
#define VERBOSE_DEBUG    true   //false   //When sedning over Serial, true gives verbose output
#define SONAR_POWER_5V   true   //TRUE for 5v sonar power, FALSE for 3.4v,
// -------------------------------------------------------------------------------------------------

//Pin configuration for the Arduino Micro (atmega32u4)
//If using I2C to communicate with rear board, its SDA will need to share pin 2 with PW_OR2

#define PW_OR1       3     //Pulse on the center and back of the board 6 or 12
#define PW_OR2       2     //Pulse on the left side of the board 9, 10, or 11
#define PW_OR3       7     //Pulse on the right side of the board 1, 2, or 3

#define S_DEC1       5     //Low order bit for selecting the round     //S-DECx pins good - Tai B.
#define S_DEC2       6     //Hi bit of round select

#define V_ENABLE    13     //Apply power to sonars                     //POWER good - Tai B.
#define V_TOGGLE     4     //(V_TOGGLE) HIGH for 3.4v, LOW for 5v sonar power

#define F_BSY       11     //pin signals to rear sonar board       IMPORTANT - FBSY should be pin 11 was 9
#define R_BSY        8     //pin signals from rear sonar bord      IMPORTANT - RBSY should be pin 8 was 10
// -------------------------------------------------------------------------------------------------

#define DEBUG_PIN   A0     //Light-emitting diode indicator for DEBUG_status = TRUE
// -------------------------------------------------------------------------------------------------

#define TIMEOUT_PERIOD  100 //ms: 20.5ms for calc + up to 62ms for the reading + a little buffer
#define ROUND_DELAY     100 //ms between rounds, seems to help stability

//Modify EXPECTED_SIGNALS to match which sonars are on the board.
#define EXPECTED_SIGNALS1    3
#define EXPECTED_SIGNALS2    3
#define EXPECTED_SIGNALS3    2

#define RANGE_DATA_SIZE     13
#define SAMPLE_DATA_SIZE     7
// -------------------------------------------------------------------------------------------------

//Bit Manipulation Functions
#define cbi(port, bit) (port &= ~(1 << bit))        //Clear a bit
#define sbi(port, bit) (port |= (1 << bit))         //Set a bit
#define tbi(port, bit) (port ^= (1 << bit))         //toggle a bit
#define ibh(port, bit) ((port & (1 << bit)) > 0)    //Is a bit high
#define ibl(port, bit) ((port & (1 << bit)) == 0)   //Is a bit low

#define valueToClockPosition(val) ((val >= 1 && val <= 3) ? (val - 1) : (val >= 9 && val <= 12) \
 ? (val - 5) : (val == 6) ? 3 : -1 )
// -------------------------------------------------------------------------------------------------

#include <avr/wdt.h> // Watch Dog Timer (Allows me to reboot the Arduino)

const int SCALE_FACTOR = 58;   //to calculate the distance, use the scale factor of 58us per cm

// SPI Slave Communications
enum SPI_COMMAND : byte { INITIAL = 14, INDEX, LSB, MSB, INCREMENT, END }; //SPI Command list

char receive_buffer[100];           // buffer to store input commands

volatile byte position, data_index; // position  : keeps track of buffer index; data_index: which finalRange index is send to the MASTER

volatile boolean process_command;   // tells me if there is a command that needs to be processed


//For Sonar Interrupts
volatile unsigned long timeLeft, timeRight, timeCenter;
volatile unsigned long timeStart;
volatile bool timeStartSet;

volatile int isr1Count, isr2Count, isr3Count;
volatile byte SignalsReceived;


//For Sonar Data and Sample Storage
enum SONAR : byte { S01, S02, S03, S06, S09, S10, S11, S12 }; //Sonar list

int sampleRange[RANGE_DATA_SIZE][SAMPLE_DATA_SIZE]{};   //Stores sample readings from each sonar
int range[RANGE_DATA_SIZE] = { INITIAL };               //Stores the final results of each sonar radar

int roundCount, timeoutStart, timeSinceLastRound;

byte sampleIndex;   //Keeps track of which sample is being stored for the sampleRange array
int controlSwitch;  //Keeps track of each radar in binary form if enabled/disabled to store data

// -------------------------------------------------------------------------------------------------
void setup()
{
    //Baud rate configured for 115200
    //It communicates on digital pins 0 (RX) and 1 (TX) as well as with the computer via USB
    Serial.begin(BAUD_RATE);

    if (DEBUG_MODE)
    {
        //DEBUG LED test
        pinMode(DEBUG_PIN, OUTPUT);
        digitalWrite(DEBUG_PIN, HIGH);
    }
    else
    {
        //Serial Peripheral Interface (SPI) Setup
        pinMode(MISO, OUTPUT); // have to send on master in, *slave out*

        //SPI Control Register (SPCR)
        SPCR |= _BV(SPE);   //Turn on SPI in slave mode (SPI Enable)
        SPCR |= _BV(SPIE);  //Turn on interrupts for SPI (SPI Interrupt Enable) 

        // get ready for an interrupt 
        position = 0;               // buffer empty
        process_command = false;    //False until we have a request to process
        data_index = 1;             //start the array index at 1
    }  
 
    pinMode(S_DEC1, OUTPUT);        //Pin 5 (PWM)       --> SD1
    pinMode(S_DEC2, OUTPUT);        //Pin 6 (PWM/A7)    --> SD2

    timeLeft = timeRight = timeCenter = timeStart = 0;
    roundCount = timeoutStart = timeSinceLastRound = 0;
    isr1Count = isr2Count = isr3Count = 0;
    timeStartSet = false;
    SignalsReceived = 0; 

    controlSwitch = 0xFF; //Enable all the sonars ready to store data
    clearRangeData(); //initialize range array

    setRound(LOW, LOW);

    digitalWrite(F_BSY, HIGH); //Signal rear board that we are here; See if there is a rear board
    digitalWrite(V_TOGGLE, (SONAR_POWER_5V ? LOW:HIGH)); //Select 5V power(LOW), or 3.8V power(HIGH)

    //Turn on sonars
    digitalWrite(V_ENABLE, HIGH);
    
    //Required delay above 175ms after power-up with the board for all XL-MaxSonars to be ready
    delay(200); //Per sonar datasheet, needs 175ms for boot

    attachInterrupt( digitalPinToInterrupt(PW_OR2), ISRLeft, CHANGE );
    attachInterrupt( digitalPinToInterrupt(PW_OR1), ISRCenter, CHANGE );
    attachInterrupt( digitalPinToInterrupt(PW_OR3), ISRRight, CHANGE );

    digitalWrite(F_BSY, LOW); //Open for business
}


// -------------------------------------------------------------------------------------------------
void loop()
{
    if (process_command)
    {
        processCommand();
        process_command = false;
    }

    setRound(LOW, LOW); //Send a pulse on sonars, superfluous
    
    interrupts(); // Not sure if we need this here at all

    //Gets the current sample index to be used for the range array
    sampleIndex = (roundCount % SAMPLE_DATA_SIZE);

//*****  ROUND 1  **************************************************************

    SignalsReceived = 0;

    setRound(LOW, HIGH); //Send a pulse on sonars
    delayPeriod(EXPECTED_SIGNALS1);

    // if (bitRead(controlSwitch, S12))
    // {
        sampleRange[12][sampleIndex] = (timeCenter - timeStart) / SCALE_FACTOR;
    // }  
    // if (bitRead(controlSwitch, S03))
    // {
        sampleRange[3][sampleIndex] = (timeRight - timeStart) / SCALE_FACTOR;
    // }
    // if (bitRead(controlSwitch, S09))
    // {
        sampleRange[9][sampleIndex] = (timeLeft - timeStart) / SCALE_FACTOR;
    // }
 
//*****  ROUND 3  **************************************************************

    SignalsReceived = 0;

    setRound(HIGH, HIGH); //Send a pulse on sonars
    delayPeriod(EXPECTED_SIGNALS3);

    // if (bitRead(controlSwitch, S10))
    // {
        sampleRange[10][sampleIndex] = (timeLeft - timeStart) / SCALE_FACTOR;
    // }
    // if (bitRead(controlSwitch, S01))
    // {
        sampleRange[1][sampleIndex] = (timeRight - timeStart) / SCALE_FACTOR;
    // }
    
//****** ROUND 2 ***************************************************************

    SignalsReceived = 0;

    setRound(HIGH, LOW); //Send a pulse on sonars
    delayPeriod(EXPECTED_SIGNALS2);

    // if (bitRead(controlSwitch, S11))
    // {
        sampleRange[11][sampleIndex] = (timeLeft - timeStart) / SCALE_FACTOR;
    // }
    // if (bitRead(controlSwitch, S02))
    // {
        sampleRange[2][sampleIndex] = (timeRight - timeStart) / SCALE_FACTOR;
    // }
    // if (bitRead(controlSwitch, S06))
    // {
        sampleRange[6][sampleIndex] = (timeCenter - timeStart) / SCALE_FACTOR;
    // }

//******************************************************************************

    //TO DO: Receive data from rear board
    if (sampleIndex == (SAMPLE_DATA_SIZE - 1))
    {
        writeOutput();
    }
    roundCount++;
}


// -------------------------------------------------------------------------------------------------
//we should rewrite this method to manipulate the digital pin registers rather that using
//digitalWrite to toggle RND pins.  This way the pins will change simultaneously, which
//will create more reliable functionality -- tb

//If Pin 4 is left open or held high (20uS or greater), the sensor will take a range reading
//Bring high 20uS or more for range reading.

void setRound(int highOrderBit, int lowOrderBit)
{
    //Turn off all sonars
    if (highOrderBit == LOW && lowOrderBit == LOW)
    {
        cbi(PORTC, PC6);     // digitalWrite(S_DEC1, LOW);
        cbi(PORTD, PD7);     // digitalWrite(S_DEC2, LOW);
    }

    //SD1 = HIGH, SD2 = LOW. Clock positions 9, 12, and 3
    else if (highOrderBit == LOW && lowOrderBit == HIGH)
    {
        cbi(PORTD, PD7);     // digitalWrite(S_DEC2, LOW); 
        sbi(PORTC, PC6);     // digitalWrite(S_DEC1, HIGH);

        delayMicroseconds(50);

        cbi(PORTC, PC6);     // digitalWrite(S_DEC1, LOW);
    }

    //SD1 = LOW, SD2 = HIGH. Clock positions 6, 11, and 2
    else if (highOrderBit == HIGH && lowOrderBit == LOW)
    {
        cbi(PORTC, PC6);     // digitalWrite(S_DEC1, LOW);
        sbi(PORTD, PD7);     // digitalWrite(S_DEC2, HIGH);

        delayMicroseconds(50);

        cbi(PORTD, PD7);     // digitalWrite(S_DEC2, LOW);
    }

    //SD1 = HIGH, SD2 = HIGH. Clock positions 10, and 1
    else if (highOrderBit == HIGH && lowOrderBit == HIGH)
    {
        sbi(PORTC, PC6);     // digitalWrite(S_DEC1, HIGH);
        sbi(PORTD, PD7);     // digitalWrite(S_DEC2, HIGH);

        delayMicroseconds(50);

        cbi(PORTC, PC6);     // digitalWrite(S_DEC1, LOW);
        cbi(PORTD, PD7);     // digitalWrite(S_DEC2, LOW);
    }
}


// -------------------------------------------------------------------------------------------------
void delayPeriod(byte expectedSingal)
{
    timeoutStart = millis();
    while (SignalsReceived < expectedSingal) 
    {
        if ((millis() - timeoutStart) > TIMEOUT_PERIOD) { break; }
    }
    
    delay(ROUND_DELAY);

    timeStartSet = false;
}
