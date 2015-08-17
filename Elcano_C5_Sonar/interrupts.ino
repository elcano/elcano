// -------------------------------------------------------------------------------------------------
// SPI interrupt routine
ISR (SPI_STC_vect)
{
    //If either 
    if (SPDR > INITIAL && SPDR < END)
    {
        switch (SPDR)
        {
            //Transfers the current range index to the SPI Data Register
            case INDEX:
                SPDR = data_index;
                break;

            //Transfers the ranges least significant bit into the SPI Data Register 
            case LSB:
                SPDR = (range[data_index] >> 8);
                break;

            //Transfers the ranges most significant bit into the SPI Data Register 
            case MSB:
                SPDR = (range[data_index] & 0xFF);
                break;

            //Increment the array index to the next available 
            case INCREMENT:

                data_index++;

                while (range[data_index] == 0)
                {
                    data_index = (data_index + 1) % RANGE_DATA_SIZE;
                }
                break;
        }
    }
    else if (SPDR != NULL)
    {
        byte c = SPDR;  // grab byte from SPI Data Register

        if (c == '\n')
        {
            receive_buffer[position] = 0;
            process_command = true;
        }
        else
        {
            receive_buffer[position++] = c;
        }
    }
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
        timeStartSet = true;      
        timeStart = micros();
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