
#include <SPI.h>
#define BAUD_RATE 115200

enum STATUS : byte { INITIAL = 14, INDEX, LSB, MSB, INCREMENT, END };

const int RANGE_SIZE = 13;
int range[RANGE_SIZE]{};

// -------------------------------------------------------------------------------------------------
void setup()
{
    Serial.begin(BAUD_RATE);     // opens serial port, sets data rate to 9600 bps
    
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV8);

    Serial.println(F("\r\nLoading MASTER................................"));
    Serial.print(F("Booting Arduino"));
    for(int i = 0; i < 32; i++)
    {
        Serial.print(F("\\"));
        delay(250);
    }
    Serial.println(F(" READY"));
}


// -------------------------------------------------------------------------------------------------
void loop()
{
    //Read in the next
    String currentCommand = readString();

    //If we recived a message, process it
    if (currentCommand != "")
    {
        char c;

        for (int i = 0; i < currentCommand.length(); i++)
        {
            SPI.transfer(currentCommand[i]);
            delay(1);
        }

        SPI.transfer('\n');
    }
    else
    {
        byte receivedByte;

        transferAndWait(INDEX);

        receivedByte = transferAndWait(NULL);

        if (receivedByte != NULL)
        {
            Serial.print(F("["));
            Serial.print((int)receivedByte);
            Serial.print(F("]"));

            transferAndWait(LSB);

            unsigned char lsb = transferAndWait(MSB);

            unsigned char msb = transferAndWait(INCREMENT);

            short data = msb | (lsb << 8); //MSB AND LSB
            
            Serial.print(data);
            Serial.print(F("\t"));
        }
        else
        {
            Serial.println();
            transferAndWait(INCREMENT);
            delay(1000);   
        }
    }
}


// -------------------------------------------------------------------------------------------------
byte transferAndWait(const byte what)
{
    byte a = SPI.transfer(what);
    delayMicroseconds(20);
    return a;
}


// -------------------------------------------------------------------------------------------------
String readString()
{
    //Create a buffer to store the scring read into
    String buffer;

    //Read until a newline char is reached
    while(Serial.available() > 0)
    {
        //Read in the next char
        char recieved = Serial.read();

        //If the char we read is newline return our entire buffer
        if(recieved == '\n')
        {
            Serial.print("INPUT COMMAND: ");
            Serial.println(buffer);

            Serial.flush();

            return buffer;
        }

        //Otherwise add the current char to the buffer
        else
        {
            buffer += recieved;
        }
        
        //Delay a bit to let the next char enter the read buffer
        delay(1);
    }

    //If nothing was read return an empty string
    return "";
}

