

/* [in] Digital Signal 0: J1 pin 1 (RxD) Read Serial Data */
const int RxD = 0;

/* [out] Digital Signal 1: J1 pin 2 (TxD). Transmit Serial Data.  */
const int TxD = 1;

/* [out] Digital Signal 13: J3 pin 5 (SCK)  LED on Arduino board. Used for testing.    */
const int LED = 13;

#ifdef SIMULATOR
const int INPUT = 0;
const int OUTPUT = 1;
const int LOW = 0;
typedef  unsigned char byte;
void pinMode(int a1, int a2);
void digitalWrite(int a1, int a2);
#endif SIMULATOR