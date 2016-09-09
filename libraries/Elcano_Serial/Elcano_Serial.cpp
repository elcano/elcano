#include "Elcano_Serial.h"

// Detects if a character (c) is within a string (s) with size (n)
static bool contains(char *s, size_t n, char c) {
	for (char *i = s; i < s + n; ++i) if (*i == c) return 1;
	return 0;
}

// Proceses input from a device (dev) and yields the next token
// May set an auxillary value (aux) if applicable
static int yylex(HardwareSerial *dev, long *aux) {
	char c, flip = 0;
start:
	c = dev->read();
	if (c == '\n') return 0;
	if (c == '-' || (c >= '0' && c <= '9')) {
		*aux = 0;
		for (;;) {
			*aux *= 10;
			if (c == '-') flip = 1; else *aux += c - '0';
			c = dev->peek();
			if (c < '0' || c > '9') break;
			c = dev->read();
		}
		if (flip) *aux = -*aux;
		return '#';
	}
	if (contains("DSGXnsabpr,{}", 13, c)) return c;
	goto start;
}

// Parses input from a device (dev) and writes out serial data (dt)
// Returns 0 if successful, -1 otherwise
int readSerial(HardwareSerial *dev, SerialData *dt) {
	long aux;
	dt->Clear();
	if (dev->available() <= 0) return 1;
	
#define LEX yylex(dev, &aux)
	int l = LEX;
	switch (l) {
	case 'D': dt->kind = MSG_DRIVE;  break;
	case 'S': dt->kind = MSG_SENSOR; break;
	case 'G': dt->kind = MSG_GOAL;   break;
	case 'X': dt->kind = MSG_SEG;    break;
	default : return 1;
	}
arg:
	l = LEX;
	if (l == '{') {
		int c = LEX;
		int d = LEX;
		
		if (d != '#') return 4;
		switch (c) {
		case 'n': dt->number      = aux; break;
		case 's': dt->speed_cmPs  = aux; break;
		case 'a': dt->angle_deg   = aux; break;
		case 'b': dt->bearing_deg = aux; break;
		case 'r': dt->probability = aux; break;
		case 'p':
			dt->posE_cm = aux;
			if (LEX != ',') return 4;
			if (LEX != '#') return 5;
			dt->posN_cm = aux;
			break;
		default:  return 3;
		}
		
		if (LEX != '}') return 6;
		goto arg;
	} else if (l == '\n') return 0;
#undef LEX
	return 2;
}

// Writes a serial data structure (dt) to a device (dev)
// Returns 0 if successful, -1 otherwise
int writeSerial(HardwareSerial *dev, SerialData *dt) {
	switch (dt->kind) {
	case MSG_DRIVE:  dev->print("D"); break;
	case MSG_SENSOR: dev->print("S"); break;
	case MSG_GOAL:   dev->print("G"); break;
	case MSG_SEG:    dev->print("X"); break;
	default: return -1;
	}
	if (dt->number != NaN && (dt->kind == MSG_GOAL || dt->kind == MSG_SEG)) {
		dev->print("{n ");
		dev->print(dt->number);
		dev->print("}");
	}
	if (dt->speed_cmPs != NaN && dt->kind != MSG_GOAL) {
		dev->print("{s ");
		dev->print(dt->speed_cmPs);
		dev->print("}");
	}
	if (dt->angle_deg != NaN && (dt->kind == MSG_DRIVE || dt->kind == MSG_SENSOR)) {
		dev->print("{a ");
		dev->print(dt->angle_deg);
		dev->print("}");
	}
	if (dt->bearing_deg != NaN && dt->kind != MSG_DRIVE) {
		dev->print("{b ");
		dev->print(dt->bearing_deg);
		dev->print("}");
	}
	if (dt->posE_cm != NaN && dt->posN_cm != NaN && dt->kind != MSG_DRIVE) {
		dev->print("{p ");
		dev->print(dt->posE_cm);
		dev->print(",");
		dev->print(dt->posN_cm);
		dev->print("}");
	}
	if (dt->probability != NaN && dt->kind == MSG_GOAL) {
		dev->print("{r ");
		dev->print(dt->probability);
		dev->print("}");
	}
	dev->print("\n");
	return 0;
}

// Sets the value of a SerialData struct to the defaults
void SerialData::Clear() {
    kind = MSG_NONE;
    number = NaN;
    speed_cmPs = NaN;
    angle_deg = NaN;
    bearing_deg = NaN;
    posE_cm = NaN;
    posN_cm = NaN;
    probability = NaN;
}
