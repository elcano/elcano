	/*
 * RCLib.h
 * This lib is based on the code created by Duane and shared at
 * http://rcarduino.blogspot.com/2012/04/how-to-read-multiple-rc-channels-draft.html
 *
 * * This library gives easy access to this functionality without (much) overhead
 *
 * mega users:
 * I tested following pins to work with pinchangeint
 * 15,A8,A9,A10,A11,A12,A13,A14,A15
 * Pin 14 did not work for me
 * and
 *
 * Note: People who feel like criticizing the code are advised to
 * 		*Not look at the code
 * 		*think before giving comments on the code
 * 		*keep it positive
 * 		*use something else
 * 		*Create their own "better" library
 *
 *  Created on: Oct 24, 2012
 *      Author: Jantje
 */

#ifndef RCLIB_H_
#define RCLIB_H_

#include "Arduino.h"


// holds the update flags of the channels
volatile uint8_t SharedUpdateFlag=0;
volatile uint16_t SharedRCValue[NUM_RC_CHANNELS];



#if NUM_DIGITAL_PINS == 70
#define MEGA
#warning "If the used board is not a mega there is a problem"
#elif NUM_DIGITAL_PINS == 20
#define UNO
#warning "If the used board is not a UNO there is a problem"
#elif NUM_ANALOG_INPUTS  ==8
#error "this chip is not supported"
#else
#define LEONARDO
#warning "If the used board is not a LEONARDO there is a problem"
#ifdef PinChangeInt_h
#error "the PinChangeInt library does not support Leonardo"
#endif
#endif

#ifdef PinChangeInt_h
#define ATTACHEINTERRUPT PCintPort::attachInterrupt
#define GETINTERRUPT(x) RC_Channel_Pin[x]
#else
#define ATTACHEINTERRUPT attachInterrupt
#define GETINTERRUPT(x) InterruptMap[RC_Channel_Pin[x]]
#ifdef MEGA
#if NUM_RC_CHANNELS >6
#error "The mega can only have 6 interrupts if you do not use the pinchangeint library"
#endif
#define LastAvailablePin 21
const uint8_t InterruptMap[22]  = {255,255,0,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,5,4,3,2} ;
#endif

#ifdef UNO
#if NUM_RC_CHANNELS >2
#error "The UNO can only have 2 interrupts if you do not use the pinchangeint library"
#endif
#define LastAvailablePin 4
prog_uint8_t InterruptMap[4]={255,255,0,1};
#endif

#ifdef LEONARDO
#if NUM_RC_CHANNELS >4
#error "The leonardo can only have 4 interrupts you can not use the pinchangeint library"
#endif
#define LastAvailablePin 4
prog_uint8_t InterruptMap[4]={2,3,1,0};
#endif

#endif


#define InteruptFunction(Interuptindex) \
{\
	static uint32_t ulStart;\
 if(digitalRead(RC_Channel_Pin[ Interuptindex ]) == HIGH) 	 ulStart = micros();\
 else {\
   SharedRCValue[Interuptindex] = (uint16_t)(micros() - ulStart);\
   SharedUpdateFlag |= (1<<Interuptindex);\
 }\
}



void RC_Channel0_interrupt () InteruptFunction(0)
#if NUM_RC_CHANNELS >1
void RC_Channel1_interrupt () InteruptFunction(1)
#if NUM_RC_CHANNELS >2
void RC_Channel2_interrupt () InteruptFunction(2)
#if NUM_RC_CHANNELS >3
void RC_Channel3_interrupt () InteruptFunction(3)
#if NUM_RC_CHANNELS >4
void RC_Channel4_interrupt () InteruptFunction(4)
#if NUM_RC_CHANNELS >5
void RC_Channel5_interrupt () InteruptFunction(5)
#if NUM_RC_CHANNELS >6
void RC_Channel6_interrupt () InteruptFunction(6)
#if NUM_RC_CHANNELS >7
void RC_Channel7_interrupt () InteruptFunction(7)
#if NUM_RC_CHANNELS >8
#error "The code only foresees in 8 interrupt. If you want more some serious code change is needed."
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif



void SetRCInterrupts()
{
#ifndef PinChangeInt_h
	boolean error=false;
	 for (uint8_t curChannel = 0; curChannel < NUM_RC_CHANNELS; ++curChannel) {
		 if ( (RC_Channel_Pin[curChannel] > LastAvailablePin ) || (InterruptMap[RC_Channel_Pin[curChannel]]==255))
		 {
			 Serial.print(F("Pin "));
			 Serial.print( curChannel);
			 Serial.print(F(" ("));
			 Serial.print(RC_Channel_Pin[curChannel]);
			 Serial.println(F(") you are trying to use is not supporting interrupts"));
			 error=true;
		 }
	 }
		 if (error) return;
#endif

	 ATTACHEINTERRUPT(GETINTERRUPT(0), RC_Channel0_interrupt,CHANGE);
		 #if NUM_RC_CHANNELS >1
	 ATTACHEINTERRUPT(GETINTERRUPT(1), RC_Channel1_interrupt,CHANGE);
		 #if NUM_RC_CHANNELS >2
	 ATTACHEINTERRUPT(GETINTERRUPT(2), RC_Channel2_interrupt,CHANGE);
		 #if NUM_RC_CHANNELS >3
	 ATTACHEINTERRUPT(GETINTERRUPT(3), RC_Channel3_interrupt,CHANGE);
		 #if NUM_RC_CHANNELS >4
	 ATTACHEINTERRUPT(GETINTERRUPT(4), RC_Channel4_interrupt,CHANGE);
		 #if NUM_RC_CHANNELS >5
	 ATTACHEINTERRUPT(GETINTERRUPT(5), RC_Channel5_interrupt,CHANGE);
		 #if NUM_RC_CHANNELS >6
	 ATTACHEINTERRUPT(GETINTERRUPT(6), RC_Channel6_interrupt,CHANGE);
		 #if NUM_RC_CHANNELS >7
	 ATTACHEINTERRUPT(GETINTERRUPT(7), RC_Channel7_interrupt,CHANGE);
		 #if NUM_RC_CHANNELS >7
		 #error "This code only support 8 interrupts"
		 #endif
		 #endif
		 #endif
		 #endif
		 #endif
		 #endif
		 #endif
		 #endif

}

//boolean haveAllRCChannelsReceivedInfo()
//{
//	 // check shared update flags to see if any channels have a new signal
//	 if(SharedUpdateFlag == ((1<<NUM_RC_CHANNELS)-1))
//	 {
//	   noInterrupts(); // turn interrupts off while we take local copies of the shared variables
//	   memcpy(RC_Channel_Value,(void*)SharedRCValue,sizeof(uint16_t)*NUM_RC_CHANNELS);
//	   SharedUpdateFlag = 0;
//	   interrupts();
//	   return true;
//	 }
//	 return false;
//}

uint8_t getChannelsReceiveInfo()
{
	uint8_t ret=0;
	 // check shared update flags to see if any channels have a new signal
	 if(SharedUpdateFlag != 0)
	 {
	   noInterrupts(); // turn interrupts off while we take local copies of the shared variables
	   memcpy(RC_Channel_Value,(void*)SharedRCValue,sizeof(uint16_t)*NUM_RC_CHANNELS);
//	   for(int i=0; i< NUM_RC_CHANNELS;i++)
//		   {
//		   Serial.print("shared :");
//		   Serial.print(SharedRCValue[i]);
//		   RC_Channel_Value[i]=SharedRCValue[i];
//		   Serial.print(" local :");
//		   Serial.println(RC_Channel_Value[i]);
//		   }
	   ret=SharedUpdateFlag;
	   SharedUpdateFlag = 0;
	   interrupts();
	 }
	 return ret;
}



#endif /* RCLIB_H_ */
