/*
Elcano_Serial.cpp
Tyler Folsom   Sept 7, 2015 Modified 9/2/16 JJB

The routines writeSerial and readSerial transfer the information in a SerialData
structure over serial lines connecting Arduino microcontrollers. The contents of the messages are specified in SerialCmd.html. Note that messages are limited to 64 characters.

Data transfer has been verified by the program SerialUnitTest.ino.
When running a program from your sketchbook, create the subdirectory
libraries/Elcano_Serial, and place both Elcano_Serial.cpp and Elcano_Serial.h there.

*/
#include "string.h"
#include "math.h"
#include "Elcano_Serial.h"


/*------------------------------------------------------------------------------*/ 
/* GetWord
** Reads to see if message contains a whole word
*/
char * GetWord(char * major, char * str){
    char * CSp1;

    CSp1 = strstr( str, major);
    if (CSp1!=NULL)
    CSp1 += strlen(major);

    return CSp1;
}
/*-------------------------------------------------------------------------------*/
/* GetNumber
** Checks string to see if message contains a completed number
*/
float GetNumber(char *minor, char*Args)
{
  float data = NaN;
  if (Args == NULL) return data;
  // minor is a keyword; grab the associated value.
  char * Number = GetWord(minor, Args);
  if (Number==NULL) return data;
   // change } to 0
   char* end = strchr(Number, '}');
   if (end == NULL) return NaN;
   *end = '\0';
   data = atof(Number);
   // change back to }
   *end = '}';
   return data;
}
/*---------------------------------------------------------------------------------------*/
/* GetPos
** Pulls position data from buffer
*/
void GetPos(char *minor, char*Args, SerialData *SerialD)
{
  SerialD->posE_cm = SerialD->posN_cm = NaN;
  if (Args == NULL) return;
  // minor is a keyword; grab the associated value.
  char * Number = GetWord(minor, Args);
  if (Number==NULL) return;
   // change , to 0
   char* end = strchr(Number, ',');
   if (end == NULL) return;
   *end = '\0';
   SerialD->posE_cm = (long) atof(Number);
   // change back to ,
   *end = ',';
   Number = end+1;  // 2nd number
   // change } to 0
   SerialD->posN_cm = (long) atof(Number);
   end = strchr(Number, '}');
   if (end == NULL) return;
   *end = '\0';
   SerialD->posN_cm = atof(Number);
   // change back to ,
   *end = ',';
}
/*-------------------------------------------------------------------------------*/
/* Clear
** Sets all SerialData members to non-values
*/
void SerialData::Clear()
{
    kind = MSG_NONE;
    number = NaN;
    speed_cmPs = NaN;
    angle_deg = NaN;
    bearing_deg = NaN;
    posE_cm = NaN;
    posN_cm = NaN;
    probability = NaN;
}
/*-------------------------------------------------------------------------------*/
/* Dump
** Prints all SerialData values to Serial
*/
void Dump (char *IncomingMessage, SerialData *SerialD)
{
    Serial.println();
    Serial.println(IncomingMessage);
    Serial.print ( "Kind ");  Serial.print(SerialD->kind);
    Serial.print ( "; Num ");  Serial.print(SerialD->number);
    Serial.print ( "; Spd ");  Serial.print(SerialD->speed_cmPs);
    Serial.print ( "; Ang ");  Serial.print(SerialD->angle_deg );
    Serial.print ( "; Br ");  Serial.print(SerialD->bearing_deg );
    Serial.print ( "; pos (");  Serial.print(SerialD->posE_cm);
    Serial.print ( ", ");  Serial.print(SerialD->posN_cm);
    Serial.print ( ") Prob ");  Serial.println(SerialD->probability);
}

/*-------------------------------------------------------------------------------*/
/* ProcessMessage
** Parses buffer for meaningful SerialData object members
** Kinds:
** S = SENSOR
** D = DRIVE
** G = GOAL
** X = SEG
** Data Members:
** a = Ang 
** b = Br
** s = Speed
** p = Pos
** n = Num
** r = Prob
*/
void ProcessMessage (char *IncomingMessage, SerialData *SerialD)
{
    float data;

 // Dump (IncomingMessage, SerialD) ;   // debug
    // Determine if message is "SENSOR {Speed xxx.xx}"	
    char * Args = GetWord ("S", IncomingMessage);//S for sensor
    if (Args != NULL)
    {	
      data = GetNumber("s", Args);
	if (data != NaN) 
        {
            SerialD->speed_cmPs = (long)(data);
            SerialD->kind = MSG_SENSOR;
        }
      data = GetNumber("a", Args);
	if (data != NaN) 
        {
            SerialD->angle_deg = (long)(data);
            SerialD->kind = MSG_SENSOR;
        }
      data = GetNumber("b", Args);
	if (data != NaN) 
        {
            SerialD->bearing_deg = (long)(data);
            SerialD->kind = MSG_SENSOR;
        }
      GetPos("p", Args, SerialD);
      if (SerialD->posN_cm != NaN && SerialD->posE_cm != NaN)
            SerialD->kind = MSG_SENSOR;
   }
    // Determine if message is "DRIVE {Speed xxx.xx}"	
    Args = GetWord ("D", IncomingMessage);
    if (Args != NULL)
    {	
        data = GetNumber("s", Args);
	  if (data != NaN) 
        {
            SerialD->speed_cmPs = (long)(data);
            SerialD->kind = MSG_DRIVE;
        }
        data = GetNumber("a", Args);
	if (data != NaN) 
        {
            SerialD->angle_deg = (long)(data);
            SerialD->kind = MSG_DRIVE;
        }
    }
    Args = GetWord ("G", IncomingMessage);
    if (Args != NULL)
    {	
      data = GetNumber("n", Args);
	if (data != NaN) 
        {
            SerialD->number = (long)(data);
        }
      GetPos("p", Args, SerialD);
      data = GetNumber("b", Args);
	if (data != NaN) 
        {
            SerialD->bearing_deg = (long)(data);
        }
       data = GetNumber("r", Args);
	if (data != NaN) 
        {  // from vision: probability that cone is present in the image
            SerialD->probability = (long)(data);
        }
        if (SerialD->posN_cm != NaN && SerialD->posE_cm != NaN) // && SerialD->number > 0)
            SerialD->kind = MSG_GOAL;
   }
    Args = GetWord ("X", IncomingMessage); //X for segment
    if (Args != NULL)
    {	
        data = GetNumber("n", Args);
	if (data != NaN) 
        {
            SerialD->number = (long)(data);
        }
        data = GetNumber("s", Args);
	if (data != NaN) 
        {
            SerialD->speed_cmPs = (long)(data);
        }
        data = GetNumber("b", Args);
	if (data != NaN) 
        {
            SerialD->bearing_deg = (long)(data);
        }
      GetPos("p", Args, SerialD);
        if (SerialD->posN_cm != NaN && SerialD->posE_cm != NaN && SerialD->number != NaN)
            SerialD->kind = MSG_SEG;
   }

}
/*------------------------------------------------------------------------------*/ 
// If there is an issue with missing data the kind might not be correctly set
// for sending that data.


bool readSerial(HardwareSerial *SerialN, SerialData *SerialD)
{
    //SerialD->Clear();
    static char IncomingMessage[BUFFER_SIZE];
    static int InIndex = 0;  // Input side, current character of SerialDrive message
    int incomingByte = 0;   // for incoming serial data
    while (SerialN->available())
    {
		SerialN->readBytesUntil('\0', IncomingMessage, BUFFER_SIZE);
		return true;
        // read the incoming byte from C4:
        // incomingByte = SerialN->read();
        // IncomingMessage[InIndex] = (char)(incomingByte);
        // IncomingMessage[InIndex+1] = 0;
        // if (IncomingMessage[InIndex] == 0 || incomingByte == '\n' || incomingByte == '\r'
         // || InIndex >= BUFFER_SIZE-1)
        // {
            // ProcessMessage(&IncomingMessage[0], SerialD);  // see what we got
            // for (InIndex = 0; InIndex < BUFFER_SIZE; InIndex++)
                // IncomingMessage[InIndex] = 0;
            // InIndex = 0;
            // IncomingMessage[InIndex] = 0;

        // }
       // else
       // {
       //  incomingByte > 31? Serial.print(IncomingMessage[InIndex]): Serial.print(incomingByte);
           // ++InIndex;        
       // }        
    }
	return false;
}
/*-------------------------------------------------------------------------------*/ 
void writeSerial(HardwareSerial *SerialN, struct SerialData *SerialD )
{
// Caution: 64 character limit for Arduino serial. Buffer size is set in the Arduino core.
// If the message produced is > 64 characters, readSerial will ignore it.
// writeSerial will only send data that relates to the kind. ex In the Serial data struct 
//if kind is 0 the only data sent will be kind angle and direction.
    switch (SerialD->kind) 
    {
    case MSG_DRIVE:
        SerialN->print("D");
        if (SerialD->speed_cmPs != NaN)
        {
            SerialN->print(" {s ");
            SerialN->print(SerialD->speed_cmPs);
            SerialN->print("}");
        }
        if (SerialD->angle_deg != NaN)
        {
            SerialN->print(" {a ");
            SerialN->print(SerialD->angle_deg);
            SerialN->print("}");
        }
        SerialN->println("\0");
        break;
        
    case MSG_SENSOR:
        SerialN->print("S");
        if (SerialD->speed_cmPs != NaN)
        {
            SerialN->print(" {s ");
            SerialN->print(SerialD->speed_cmPs);
            SerialN->print("}");
        }
        if (SerialD->angle_deg != NaN)
        {
            SerialN->print(" {a ");
            SerialN->print(SerialD->angle_deg);
            SerialN->print("}");
        }
        if (SerialD->posE_cm != NaN && SerialD->posN_cm != NaN)
        {
            SerialN->print(" {p ");
            SerialN->print(SerialD->posE_cm);
            SerialN->print(",");
            SerialN->print(SerialD->posN_cm);
            SerialN->print("}");
        }
       if (SerialD->bearing_deg != NaN)
        {
            SerialN->print(" {b ");
            SerialN->print(SerialD->bearing_deg);
            SerialN->print("}");
        }
        SerialN->println("\0");
        break;
        
    case MSG_GOAL:
        SerialN->print("G");
        if (SerialD->number != NaN)
        {
            SerialN->print(" {n ");
            SerialN->print(SerialD->number);
            SerialN->print("}");
        }
        if (SerialD->posE_cm != NaN && SerialD->posN_cm != NaN)
        {
            SerialN->print(" {p ");
            SerialN->print(SerialD->posE_cm);
            SerialN->print(",");
            SerialN->print(SerialD->posN_cm);
            SerialN->print("}");
        }
        if (SerialD->bearing_deg != NaN)
        {
            SerialN->print(" {b ");
            SerialN->print(SerialD->bearing_deg);
            SerialN->print("}");
        }
         if (SerialD->probability != NaN)
        {
            SerialN->print(" {r ");
            SerialN->print(SerialD->probability);
            SerialN->print("}");
        }
       SerialN->println("\0");
        break;

    case MSG_SEG:
        SerialN->print("X");
        if (SerialD->number != NaN)
        {
            SerialN->print(" {n ");
            SerialN->print(SerialD->number);
            SerialN->print("}");
        }
        if (SerialD->posE_cm != NaN && SerialD->posN_cm != NaN)
        {
            SerialN->print(" {p ");
            SerialN->print(SerialD->posE_cm);
            SerialN->print(",");
            SerialN->print(SerialD->posN_cm);
            SerialN->print("}");
        }
       if (SerialD->bearing_deg != NaN)
        {
            SerialN->print(" {b ");
            SerialN->print(SerialD->bearing_deg);
            SerialN->print("}");
        }
        if (SerialD->speed_cmPs != NaN)
        {
            SerialN->print(" {s ");
            SerialN->print(SerialD->speed_cmPs);
            SerialN->print("}");
        }
        SerialN->println("\0");
        break;

    case MSG_NONE:
    default:
        break;
    }
}
