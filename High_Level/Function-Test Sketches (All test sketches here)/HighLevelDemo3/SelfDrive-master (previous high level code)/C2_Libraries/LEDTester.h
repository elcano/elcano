#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            4  //23 25 26 27 28 31 4 34
#define NUMPIXELS      29 //10 + 10 + 9
#define WIDTH          11
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int xPos;
int yPos;
int CursorPosition;

void clearLights() {
    for (int i = 0; i < NUMPIXELS; i ++) {
        pixels.setPixelColor(i, 0, 0, 0);
        pixels.show();
    }
}

void setAngles (long pos) {
    int brightness = 30;
    // int offset = 4;
    //assumes 9 LED strips (0 to 8 where 4 is center)
    if (pos >= 0 && pos < 4){
        for (int i = pos; i <= 4; i++) {
            pixels.setPixelColor(i, brightness, brightness, brightness);
            pixels.show();
            // dim the lights for the ones closer to center
            brightness /= 1.5;
        }
        //set other side off
        for (int i = 5; i <= 8; i++) {
            pixels.setPixelColor(i, 0, 0, 0);
            pixels.show();
        }
    }
    else if (pos == 4) {
        pixels.setPixelColor(pos, brightness, brightness, brightness);
        pixels.show();
        for (int i = 0; i <= 8; i++) {
            if(i != 4) {
                pixels.setPixelColor(i, 0, 0, 0);
                pixels.show();
            }
        }
    } else { //to the right pos > 4
        if (pos > 8) {
            pos = 8;
        }
        for (int i = pos; i >= 4; i--) {
            pixels.setPixelColor(i,brightness, brightness, brightness);
            pixels.show();
            // dim the lights for the ones closer to center
            brightness /= 1.5;
        }
        //set other side off
        for (int i = 3; i >= 0; i--) {
            pixels.setPixelColor(i, 0, 0, 0);
            pixels.show();
        }
    }
}
void setLeftSpeed (long sped) {
    int offset = 18; //TODO change this
    //set lights on
    
    if(sped > 10) {
        sped = 10;
    }
    for (int i = 0; i < sped; i++) {
        pixels.setPixelColor(offset - i, 0, 10, 0);
        pixels.show();
    }
    //set remaining lights off
    for (int i = sped; i < 10; i++) {
        pixels.setPixelColor(offset - i, 0, 0, 0);
        pixels.show();
    }
}

void setRightSpeed (long sped) {
    
    int offset = 19; //TODO change this
    
    if(sped > 10) {
        sped = 10;
    }
    //set lights on
    for (int i = 0; i < sped; i++) {
        pixels.setPixelColor(i + offset, 0, 0, 20);
        pixels.show();
    }
    //set remaining lights off
    for (int i = sped; i < 10; i++) {
        pixels.setPixelColor(i + offset, 0, 0, 0);
        pixels.show();
    }
}

//void testStrobe() {
//    for (int i = 0; i < NUMPIXELS; i ++) {
//        pixels.setPixelColor(i, 0, 0, 100);
//        pixels.show();
//        delay(250);
//    }
//    for (int i = 0; i < NUMPIXELS; i ++) {
//        pixels.setPixelColor(i, 100, 0, 0);
//        pixels.show();
//        delay(250);
//    }
//}
//
//void teston () {
//    for (int i = 0; i < NUMPIXELS; i ++) {
//        pixels.setPixelColor(i, 100, 100, 100);
//        pixels.show();
//        //delay(250);
//    }
//}






