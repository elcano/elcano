#ifndef RC_RF69_H
#define RC_RF69_H

#define UART_BAUDRATE 115200
#define RF69_FREQ_MHZ 915.0

// The encryption key has to be the same for transmitter and receiver
uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                  0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
                };

typedef struct {
  unsigned int throttle;
  unsigned int turn;
  unsigned char ebrake;
  unsigned char autonomous;
  int rssi; // "The current RSSI value in units of 0.5dB", negative
} dataFromTransmitter;

typedef struct {
  int rssi; // "The current RSSI value in units of 0.5dB", negative
} dataFromReceiver;

// transmitter input pins
#define TURN_PIN      A3 // joystick 2 L/R 
#define THROTTLE_PIN  A2 // joystick 1 U/D
#define INTERRUPT_PIN 2
#define AUTO_PIN      5
#define EBRAKE_PIN    4
#define REVERSE_PIN   20

// LED pins on the transmitter
#define TX_LED_LINK       6

// other outputs
#define SS_PIN            10
/* pins 11 - 13 are reserved for SPI */

#endif
