#include <RH_ASK.h>
//#include <SPI.h> // Not actualy used but needed to compile

//RH_ASK driver;
RH_ASK driver(2000, 46, 6, 10, false);

void setup()
{
    Serial.begin(9600); // Debugging only
    if (!driver.init())
    {
         Serial.println("init failed");
    }
}

void loop()
{
    uint8_t buf[9];
    uint8_t buflen = sizeof(buf);
    memset(buf, 0, sizeof(buf));
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      // Message with a good checksum received, dump it.
//      Serial.print("Message: ");
//      Serial.print(sizeof(buf));
//      Serial.print(" ");
//      Serial.println((char*)buf);
      int throttle = 0;
      int turn = 0;
// Message structure (uint_8 array)
// [0] : 0 = disengaged
// [1] : 0 = autonomous
// Turn:
// [2] : x100 (most significant turn digit)
// [3] : x10 
// [4] : x1 
// Throttle:
// [5] : x1,000 (most significant throttle digit)
// [6] : x100
// [7] : x10
// [8] : x1

      if(buf[0] == '0')
      {
        Serial.print("Disengaged");
        if(buf[1] == '0')
        {
          Serial.print(" AND Autonomous");
        }
        else
        {
          Serial.print("ERROR");
        }
        Serial.println();
      }
      else
      {
        Serial.println("Engaged");
      }
      for(int j = 2; j < 5; j++)
      {
        turn += ((buf[j] - '0') * pow(10, (4-j)));
      }
      for(int j = 5; j < 9; j++)
      {
        throttle += ((buf[j] - '0') * pow(10, (8-j)));
      }
      Serial.println(turn);
      Serial.println(throttle);
      memset(buf, 0, sizeof(buf));         
    }
    else
    {
      //Serial.println("Nothing Received");
    }
}
