#include <RH_RF69.h> // External; documented at http://www.airspayce.com/mikem/arduino/RadioHead/classRH__RF69.html
#include <SPI.h>
#include "RadioControl_rf69.h"

#define TRANSMITTER 0 // set false to compile receiver code
#define DEBUG 1       /*prints debugging info to serialUSB, can impact loop time
                        WARNING: when true (!0), you must connect USB to allow hardware reset, otherwise
                        SAMD21 Arduino will do nothing*/

RH_RF69 driver(SS_PIN, INTERRUPT_PIN);

dataFromTransmitter txData; // sent from transmitter
unsigned char lenTx = sizeof(txData);
dataFromReceiver rxData; // sent from receiver
unsigned char lenRx = sizeof(rxData);

const unsigned long BACKOFF = 20; // backoff controls maximum waiting time for a reply and ultimately maximum packets / second
unsigned long timeSent = 0UL;
unsigned long timeRecv = 0UL;

void setup()
{
  // configure pins
  pinMode(TURN_PIN, INPUT);
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(AUTO_PIN, INPUT_PULLUP); /* internal pullup keeps values from floating */
  pinMode(EBRAKE_PIN, INPUT_PULLUP);
  pinMode(TX_LED_LINK, OUTPUT);
  analogReadResolution(12);
  // SerialUSB connects to serial monitor or tty
  SerialUSB.begin(UART_BAUDRATE);
  if (DEBUG)
    while (!SerialUSB) {
      ;
    }

  if (!driver.init()) {
    if (DEBUG)
      SerialUSB.println("init failed");
  }
  else {
    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
    driver.setFrequency(RF69_FREQ_MHZ);
    driver.setTxPower(20, true);
    driver.setEncryptionKey(key);
  }
  if (DEBUG) {
    SerialUSB.println("Setup Complete: ");
    if  (TRANSMITTER) {
      SerialUSB.println("transmitter mode");
    }
    else {
      SerialUSB.println("receiver mode");
    }
  }
}

void loop() {
#if TRANSMITTER
  txloop();
#else
  rxloop();
#endif
}

void readInputs() {
  txData.turn = analogRead(TURN_PIN);
  txData.throttle = analogRead(THROTTLE_PIN);
  txData.autonomous = digitalRead(AUTO_PIN);
  txData.ebrake = digitalRead(EBRAKE_PIN);
}

static bool received = false;
void setLights() {
  digitalWrite(TX_LED_LINK, received);
}

void txloop()
{
  readInputs();
  // Send a message to receiver unit
  driver.send((unsigned char*)(&txData), lenTx);
  driver.waitPacketSent();
  timeSent = millis();
  // Now wait for a reply
  if (driver.waitAvailableTimeout(BACKOFF)) {
    // we have a reply
    if (driver.recv((unsigned char*)(&rxData), &lenRx)) {
      // payload is the right size
      received = true;
      txData.rssi = driver.rssiRead();
      setLights();
      timeRecv = millis();
      if (DEBUG)
        SerialUSB.println(String(rxData.rssi / 2.0));

      unsigned long interval = timeRecv - timeSent;
      unsigned long msDelay = BACKOFF - interval;
      if (msDelay < BACKOFF) {
        delay(msDelay);
      }
      else {
        delay(BACKOFF);
      }
    }

    else {
      // payload is not the right size
      received = false;
      setLights();
      if (DEBUG)
        SerialUSB.println("!");
    }
  }
  else {
    // we did not get a reply
    received = false;
    setLights();
    if (DEBUG)
      SerialUSB.println("?");
  }
}

void rxloop() {
  if (driver.available()) {
    // a packet is availible
    if (driver.recv((unsigned char*)(&txData), &lenTx)) {
      // payload is the right size
      if (timeRecv > 0)
        timeSent = timeRecv;
      timeRecv = millis();
      rxData.rssi = driver.rssiRead();
      driver.send((unsigned char*)(&rxData), lenRx); // packet sent to transmitter
      driver.waitPacketSent();
      if (DEBUG) {
        printTx(false);
        unsigned long duration = timeRecv - timeSent;
        SerialUSB.println(","
                          + String(duration));
      }
    }
    else {
      // payload was the wrong size
      if (DEBUG)
        SerialUSB.println("!");
    }
  }
}

void printTx(bool newline) {
  // output formatted as CSV for ease of review/logging
  String packet = String(rxData.rssi / 2.0)
                  + ","
                  + String(txData.throttle)
                  + ","
                  + String(txData.turn)
                  + ",";

  if (txData.ebrake) {
    packet += "E";
  }
  packet += ",";

  if (txData.autonomous) {
    packet += "A";
  }
  packet += ",";

  if (newline) {
    SerialUSB.println(packet);
  }
  else {
    SerialUSB.print(packet);
  }
}

