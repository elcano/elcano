
// -------------------------------------------------------------------------------------------------
void writeOutput()
{
    if (DEBUG_MODE) 
    { 
        // if (VERBOSE_DEBUG)
        // {
        //     Serial.print(F("Round: "));     Serial.print(roundCount);
        //     Serial.print(F(" micros: "));   Serial.print(micros());
           
        //     Serial.print(F(", micros since last: "));
        //     Serial.print(micros() - timeSinceLastRound);
        //     timeSinceLastRound = micros();
            
        //     Serial.print(F(", SigReceived: ")); Serial.print(SignalsReceived);
        //     Serial.print(F(", isr1: "));        Serial.print(isr1Count);
        //     Serial.print(F(", isr2: "));        Serial.print(isr2Count);
        //     Serial.print(F(", isr3: "));        Serial.println(isr3Count);
        // }

        for (int index = 1; index < RANGE_DATA_SIZE; index++)
        {
            if (VERBOSE_DEBUG)
            {
                //Prints out only the sonars for one board (9, 10, 11, 12, 1, 2, 3)
                if (index == 4 || index == 5 || index == 7 || index == 8) { continue; }
                // if (bitRead(controlSwitch, valueToClockPosition(index))) { continue; }

                if (index < 10) { Serial.print(F("[ ")); }
                else { Serial.print(F("[")); }

                Serial.print(index);
                Serial.print(F("]\t"));

                for (int position = 0; position < SAMPLE_DATA_SIZE; position++)
                {
                    Serial.print(sampleRange[index][position]);
                    Serial.print(F("\t"));
                }
                Serial.print(F("\t"));
            }
           
            quicksort(sampleRange, index, 0, SAMPLE_DATA_SIZE - 1); //Quicksort

            Serial.print(findMode(sampleRange, index, SAMPLE_DATA_SIZE));
            
            if (VERBOSE_DEBUG) { Serial.println(); }
            else { Serial.print(F(" ")); }
            
        }
        Serial.println();
    }

    else
    {
        for (int index = 1; index < RANGE_DATA_SIZE; index++)
        {
            quicksort(sampleRange, index, 0, SAMPLE_DATA_SIZE - 1); //Quicksort

            range[index] = findMode(sampleRange, index, SAMPLE_DATA_SIZE);
        }

        showBinaryControlSwitch();
    }
}


// -------------------------------------------------------------------------------------------------
void processCommand()
{
    String currentCommand;
    for (int i = 0; i < position; i++)
    {
        currentCommand += receive_buffer[i];
    }
    position = 0;

    //COMMAND: Enable all the sonars on the sonar board
    if (currentCommand == "enable all")
    {
        Serial.println("ENABLE ALL");

        //Enable all sonars ON
        controlSwitch = 0xFF;
    }

    //COMMAND: Rebooting the Arduino
    else if (currentCommand == "reboot")
    {
        Serial.print(F("Rebooting Arduino"));
        for (int i = 0; i < 8; i++)
        {
            Serial.print(F("."));
            delay(250);
        }
        Serial.println(F("GOODBYE"));

        software_Reboot();
    }

    //Get the length of the command header
    int headerLength = currentCommand.indexOf(":");

    //Parse out the command header
    String commandHeader = currentCommand.substring(0, headerLength);

    //Parse out the command value
    String commandValue = currentCommand.substring(headerLength + 1);

    //COMMAND:Narrow the sonar focus on the sonar board
    if (commandHeader == "narrow focus")
    {
        if (commandValue == "left")
        {
            Serial.println("LEFT");
            controlSwitch = 0x70; //Enable sonars 9, 10, 11 on

        }
        else if (commandValue == "center")
        {
            Serial.println("CENTER");
            controlSwitch = 0xC1; //Enable sonars 11, 12, 1 on

        }
        else if (commandValue == "right")
        {
            Serial.println("RIGHT");
            controlSwitch = 0x07; //Enable sonars 1, 2, 3 on

        }

        //Clear array data before after changing any configuration
        clearRangeData();
    }

    //COMMAND:Single a sonar to read on the sonar board
    else if (commandHeader == "single mode")
    {
        int sonar = commandValue.toInt();

        if (valueToClockPosition(sonar) >= 0)
        {
            Serial.print("SINGLE MODE [");
            Serial.print(sonar);
            Serial.println("]");

            controlSwitch = 0x00;
            sbi(controlSwitch, sonar);
            clearRangeData();
        }
    }

    //COMMAND:Toggle a sonar ON or OFF
    else if (commandHeader == "toggle radar")
    {
        int sonar = commandValue.toInt();

        if (valueToClockPosition(sonar) >= 0)
        {
            Serial.print("TOGGLE RADAR[");
            Serial.print(sonar);
            Serial.println("]");

            tbi(controlSwitch, sonar);
            clearRangeData();
        }
    }
}


// -------------------------------------------------------------------------------------------------
void showBinaryControlSwitch()
{
    for (int i = 7; i >= 0; i--)
    {
        Serial.print(((controlSwitch & 1 << i) > 0));
    }
    Serial.println();
}


// -------------------------------------------------------------------------------------------------
void clearRangeData()
{
    for (int index = 1; index < RANGE_DATA_SIZE; index++)
    {
        range[index] = 0;
    }    
}


// -------------------------------------------------------------------------------------------------
void software_Reboot()
{
    //Watch Dog Timer (Reboot in 15ms)
    wdt_enable(WDTO_15MS);
    while (true);
}