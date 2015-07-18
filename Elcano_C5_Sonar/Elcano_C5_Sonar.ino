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

// ###########  TO DO: communication with host over SPI  ###########################################
//  SS      Chip select from host processor
//  MOSI    This processor's serial input;  Not yet used !!!!
//  MISO    This processor's serial output
// #################################################################################################

#define DEBUG_MODE       true   //When DEBUG_MODE is true, send data over Serial, false send data over SPI
#define VERBOSE_DEBUG    true   //When sedning over Serial, true gives verbose output
#define SONAR_POWER_5V   true   //TRUE for 5v sonar power, FALSE for 3.4v,
#define BAUD_RATE      115200   //Serial Baud Rate for debugging
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
#define ROUND_DELAY      45 //ms between rounds, seems to help stability

//Modify EXPECTED_SIGNALS to match which sonars are on the board.
#define EXPECTED_SIGNALS1    3
#define EXPECTED_SIGNALS2    3
#define EXPECTED_SIGNALS3    2

#define COMMAND_GO           1

#define RANGE_DATA_SIZE     13
#define SAMPLE_DATA_SIZE     5
// -------------------------------------------------------------------------------------------------

//Bit Manipulation Functions
#define cbi(port, bit) (port &= ~(1 << bit))        //Clear a bit
#define sbi(port, bit) (port |= (1 << bit))         //Set a bit
#define tbi(port, bit) (port ^= (1 << bit))         //toggle a bit
#define ibh(port, bit) ((port & (1 << bit)) > 0)    //Is a bit high
#define ibl(port, bit) ((port & (1 << bit)) == 0)   //Is a bit low
// -------------------------------------------------------------------------------------------------

enum STATE : byte { INITIAL, READY, ROUND1, ROUND2, ROUND3 };
STATE boardState;   //initializing sonar board state

const int SCALE_FACTOR_PW = 58;   //to calculate the distance, use the scale factor of 58us per cm
const float SCALE_FACTOR_AN = ((SONAR_POWER_5V ? 5.0 : 3.3) / 1024.0);

volatile byte SignalsReceived;
volatile unsigned long timeLeft;
volatile unsigned long timeRight;
volatile unsigned long timeCenter;

volatile unsigned long timeStart;
volatile bool timeStartSet;

//For SPI communications
volatile unsigned long timeWriteStart;
volatile byte valueIn;
volatile bool processIt;

int range[RANGE_DATA_SIZE][SAMPLE_DATA_SIZE]{};
int analogRange[RANGE_DATA_SIZE]{};

int roundCount, timeoutStart, timeSinceLastRound;
byte boardCount, sampleIndex;

volatile int isr1Count, isr2Count, isr3Count;


// -------------------------------------------------------------------------------------------------
void setup()
{
    if (DEBUG_MODE)
    {
        //DEBUG LED test
        pinMode(DEBUG_PIN, OUTPUT);

        //Baud rate configured for 115200
        //It communicates on digital pins 0 (RX) and 1 (TX) as well as with the computer via USB
        Serial.begin(BAUD_RATE);
        digitalWrite(DEBUG_PIN, HIGH);
    }
    else
    {
        //Serial Peripheral Interface (SPI) Setup
        pinMode(MISO, OUTPUT);

        //SPI Control Register (SPCR)
        SPCR |= _BV(SPE);   //Turn on SPI in slave mode (SPI Enable)
        SPCR |= _BV(SPIE);  //Turn on interrupts for SPI (SPI Interrupt Enable) 

        processIt = false;  //False until we have a request to process
    }  

    timeLeft = timeRight = timeCenter = timeStart = 0;
    roundCount = timeoutStart = timeSinceLastRound = 0;
    isr1Count = isr2Count = isr3Count = 0;
    timeStartSet = false;
 
    pinMode(S_DEC1, OUTPUT);        //Pin 5 (PWM)       --> SD1
    pinMode(S_DEC2, OUTPUT);        //Pin 6 (PWM/A7)    --> SD2

    //NOTE -- Pins below are default to inputs, so they don't need to be explicitly declared as inputs with pinMode()
    pinMode(PW_OR1, INPUT);         //Pin 3 (PWM/SCL)   --> PW1
    pinMode(PW_OR2, INPUT);         //Pin 2 (SDA)       --> PW2
    pinMode(PW_OR3, INPUT);         //Pin 7             --> PW3

    boardState = INITIAL; 
    SignalsReceived = 0; 
    boardCount = 1;

    setRound(LOW, LOW);

    digitalWrite(F_BSY, HIGH); //Signal rear board that we are here; See if there is a rear board
    digitalWrite(V_TOGGLE, (SONAR_POWER_5V ? LOW:HIGH)); //Select 5V power(LOW), or 3.8V power(HIGH)

    //Turn on sonars
    digitalWrite(V_ENABLE, HIGH);
    

    //for (int i = 0; i < 1000; i++)
    //{  
    //    //See if there is a second board
    //    byte Rear = digitalRead(R_BSY);
    //    if (Rear == HIGH)
    //    {
    //        boardCount = 2;
    //        //Need to dynamically attach/detacy PW_OR2 so that it is shared between IntRight and I2C
    //        break;
    //    }
    //}

    //Required delay above 175ms after power-up with the board for all XL-MaxSonars to be ready
    delay(200); //Per sonar datasheet, needs 175ms for boot

    attachInterrupt( digitalPinToInterrupt(PW_OR2), ISRLeft, CHANGE );
    attachInterrupt( digitalPinToInterrupt(PW_OR1), ISRCenter, CHANGE );
    attachInterrupt( digitalPinToInterrupt(PW_OR3), ISRRight, CHANGE );

    digitalWrite(F_BSY, LOW); //Open for business
    boardState = READY;

    // digitalWrite(R_BSY, LOW);        //TESTING REMOVE ME LATER // REMOVE ME // PICOSCOPE TESTING
}


// -------------------------------------------------------------------------------------------------
//SPI interrupt routine
ISR (SPI_STC_vect) 
{
    valueIn = SPDR;     //Grab byte from SPI Data Register
    processIt = true;
}


// -------------------------------------------------------------------------------------------------
void ISRLeft()
{  
    //ISR for end of pulse on sonar 9, 10 or 11    
    noInterrupts();

    isr1Count++;

    //if (digitalRead(PW_OR3) == HIGH && timeStartSet == false)
    if (ibh(PINE, PE6) && !timeStartSet)
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
void ISRCenter()
{  
    //ISR for end of pulse on sonar 6 or 12
    noInterrupts();

    isr2Count++;

    //if (digitalRead(PW_OR1) == HIGH && timeStartSet == false)
    if (ibh(PIND, PD2) && !timeStartSet)
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
void ISRRight()
{
    //ISR for end of pulse on sonar 1, 2 or 3
    noInterrupts(); 

    isr3Count++; 

    //if (digitalRead(PW_OR2) == HIGH && timeStartSet == false)
    if (ibh(PIND, PD1) && !timeStartSet)
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
    // sbi(PORTB, PB7);    // D0 HIGH (FBSY) // REMOVE ME // PICOSCOPE TESTING

    boardState = READY;

    setRound(LOW, LOW); //Send a pulse on sonars, superfluous

    //readInput(); //Sonar is slave waiting for command from master
    //TO DO: Send start signal to rear board.
    
    interrupts(); // Not sure if we need this here at all

    //Gets the current sample index to be used for the range array
    sampleIndex = (roundCount % SAMPLE_DATA_SIZE);

//*****  ROUND 1  **************************************************************
    // sbi(PORTB, PB4);         // D1 HIGH (RBSY) // REMOVE ME // PICOSCOPE TESTING

    boardState = ROUND1;  
    SignalsReceived = 0;

    setRound(LOW, HIGH); //Send a pulse on sonars
    delayPeriod(EXPECTED_SIGNALS1);

    range[12][sampleIndex] = (timeCenter - timeStart) / SCALE_FACTOR_PW;
    range[3] [sampleIndex] = (timeRight - timeStart) / SCALE_FACTOR_PW;
    range[9] [sampleIndex] = (timeLeft - timeStart) / SCALE_FACTOR_PW;

    // analogRange[12] = analogRead(AN12);
    // analogRange[3] =  analogRead(AN3);
    // analogRange[9] =  analogRead(AN9);

    // cbi(PORTB, PB4);         // D1 LOW (RBSY) // REMOVE ME // PICOSCOPE TESTING

 
//*****  ROUND 3  **************************************************************
    // sbi(PORTF, PF7);         // D2 HIGH (DEBUG_PIN) // REMOVE ME // PICOSCOPE TESTING

    boardState = ROUND3;
    SignalsReceived = 0;

    setRound(HIGH, HIGH); //Send a pulse on sonars
    delayPeriod(EXPECTED_SIGNALS3);

    range[10][sampleIndex] = (timeLeft - timeStart) / SCALE_FACTOR_PW;
    range[1] [sampleIndex] = (timeRight - timeStart) / SCALE_FACTOR_PW;

    // analogRange[10] = analogRead(AN10);
    // analogRange[1] =  analogRead(AN1);

    // cbi(PORTF, PF7);         // D2 LOW (DEBUG_PIN) // REMOVE ME // PICOSCOPE TESTING

    
//****** ROUND 2 ***************************************************************
    // sbi(PORTB, PB0);         // D3 HIGH (RX) // REMOVE ME // PICOSCOPE TESTING

    boardState = ROUND2;
    SignalsReceived = 0;

    setRound(HIGH, LOW); //Send a pulse on sonars
    delayPeriod(EXPECTED_SIGNALS2);

    range[11][sampleIndex] = (timeLeft - timeStart) / SCALE_FACTOR_PW;
    range[2] [sampleIndex] = (timeRight - timeStart) / SCALE_FACTOR_PW;
    range[6] [sampleIndex] = (timeCenter - timeStart) / SCALE_FACTOR_PW;

    // analogRange[11] = analogRead(AN11);
    // analogRange[2] =  analogRead(AN2);
    // analogRange[6] =  analogRead(AN6);

    // cbi(PORTB, PB0);         // D3 LOW (RX) // REMOVE ME // PICOSCOPE TESTING

//******************************************************************************

    //TO DO: Receive data from rear board
    if (sampleIndex == (SAMPLE_DATA_SIZE - 1))
    {
        writeOutput();
        // cbi(PORTB, PB7);     // D0 LOW (FBSY) // REMOVE ME // PICOSCOPE TESTING
        // delay(500); // REMOVE ME // PICOSCOPE TESTING
    }
    // cbi(PORTB, PB7);         // D0 LOW (FBSY) // REMOVE ME // PICOSCOPE TESTING
    // delayMicroseconds(50);   // REMOVE ME // PICOSCOPE TESTING

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
        for (int index = 1; index < RANGE_DATA_SIZE; index++)
        {
            if (VERBOSE_DEBUG)
            {
                //Prints out only the sonars for one board (9, 10, 11, 12, 1, 2, 3)
                if (index == 4 || index == 5 || index == 7 || index == 8) { continue; }

                Serial.print(F("Round: "));     Serial.print(roundCount);
                Serial.print(F(" micros: "));   Serial.print(micros());
               
                Serial.print(F(", micros since last: "));
                Serial.print(micros() - timeSinceLastRound);
                timeSinceLastRound = micros();
                
                Serial.print(F(", SigReceived: ")); Serial.print(SignalsReceived);
                Serial.print(F(", isr1: "));        Serial.print(isr1Count);
                Serial.print(F(", isr2: "));        Serial.print(isr2Count);
                Serial.print(F(", isr3: "));        Serial.println(isr3Count);

                if (index < 10) { Serial.print(F("[ ")); }
                else { Serial.print(F("[")); }

                Serial.print(index);
                Serial.print(F("]\t"));

                for (int position = 0; position < SAMPLE_DATA_SIZE; position++)
                {
                    Serial.print(range[index][position]);
                    Serial.print(F("\t"));
                }
                Serial.print(F("\t"));
            }
           
            // sortArray(range, index, SAMPLE_DATA_SIZE);   //Bubble Sort
            quicksort(range, index, 0, SAMPLE_DATA_SIZE);   //Quicksort

            Serial.print(findMode(range, index, SAMPLE_DATA_SIZE));
            
            if (VERBOSE_DEBUG) { Serial.println(); }
            else { Serial.print(F(" ")); }
            
            // Serial.println(analogRange[index]);
        }
        Serial.println();
    }

// =============================================================================
// TODO -- Below is where data will be sent over SPI.
// =============================================================================
    else 
    {
        int dataPos = 0;
        processIt = false;
        while (dataPos < RANGE_DATA_SIZE) 
        {
            timeWriteStart = millis();

            while (!processIt) 
            {
                if (millis() - timeWriteStart > TIMEOUT_PERIOD) { break; }
            }

            writeIntData(range[dataPos++][0]);
            delay(20);
        }
        dataPos = 0;
    }
// =============================================================================
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


// -------------------------------------------------------------------------------------------------
//Sorting function (Array, clock postion, numer of samples)
void sortArray(int array[][SAMPLE_DATA_SIZE], int clockPos, int size)
{
    for (int index = 1; index < size; ++index)
    {
        int next = array[clockPos][index];
        int current;
    
        for (current = (index - 1); (current >= 0) && (next < array[clockPos][current]); current--)
        {
            array[clockPos][current + 1] = array[clockPos][current];
        }
        array[clockPos][current + 1] = next;
    }
}


// -------------------------------------------------------------------------------------------------
//Sorting function (Array, clock postion, 0, numer of samples)

//Pass by Reference
// void quicksort(int (&array)[RANGE_DATA_SIZE][SAMPLE_DATA_SIZE], int clockPos, int start, int end)

void quicksort(int array[][SAMPLE_DATA_SIZE], int clockPos, int start, int end)
{
    int up = start, down = end;
    int pivot = array[clockPos][(start + end) / 2];
    
    while (up <= down)  //Loop until they cross
    {
        
        while (array[clockPos][up] < pivot) { up++; }     // Walks the up right
        while (array[clockPos][down] > pivot) { down--; } // Walks the down left
        
        if (up <= down)
        {
            //Swap
            int temp = array[clockPos][up];
            array[clockPos][up] = array[clockPos][down];
            array[clockPos][down] = temp;
            
            up++;
            down--;
        }
    }
    if (start < down) { quicksort(array, clockPos, start, down); }
    if (up < end) { quicksort(array, clockPos, up, end); }
}


// -------------------------------------------------------------------------------------------------
//Mode function, returning the mode or median.
// - Finds the MODE in the given data set
// - If there isn't a MODE, the MEDIAN is returned
// - if there are two or more MODES (bimodal), the MEDIAN is returned
// - Negative values are not included when finding the MODE or MEDIAN
// - Known issue if all samples are negative but one, returns negative
int findMode(int array[][SAMPLE_DATA_SIZE], int clockPos, int arraySize)
{
    int modeValue = 0;    //Mode Value

    bool bimodal = false; //Bimodal Distribution

    int index = 0;        //Sample Index
    int shiftSize = 0;    //Shift Array Size
    
    int count = 0;
    int prevCount = 0;
    
    int maxCount = 0;
    int prevMaxCount = 0;
    
    //checks if you reached the end of the array
    while (index < (arraySize - 1))
    {
        if (array[clockPos][index] < 0)
        {
            shiftSize++;
            index++;
            continue;
        }
        
        prevCount = count;  //copy the previous count value
        count = 0;          //reset the count value

        //counts the number of duplications values
        while (array[clockPos][index] == array[clockPos][index + 1])
        {
            count++;
            index++;
        }
        
        if (count > prevCount && count >= maxCount) //Found the MODE. Higher than max and prev
        {
            modeValue = array[clockPos][index]; //set the new MODE
            prevMaxCount = maxCount;
            maxCount = count;
            bimodal = false;
        }
        
        //Moves to the next sample if no MODE was found
        if (count == 0) { index++; }
        //If the sample dataset has 2 or more MODES.
        else if (count == prevMaxCount && maxCount == prevMaxCount)
        {
            bimodal = true;
        }
        
        //Return the MEDIAN if there is no MODE or are more than two MODES.
        if (modeValue == 0 || bimodal)
        {
            modeValue = array[clockPos][((arraySize + shiftSize ) / 2)];
        }
    }
    return modeValue;
}
