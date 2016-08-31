%{
#include <stdlib.h>
#include "arduino.hh"
#include "arduino.tab.hh"
#define YY_DECL extern "C" int yylex()
%}

%option noyywrap noinput nounput

%%

[ \t\n]  { }
[0-9]+   { yylval.ival = atoi(yytext); return INT; }
"DRIVE"  { return DRIVE; }
"SENSOR" { return SENSOR; }
"GOAL"   { return GOAL; }
"SEG"    { return SEG; }
"Num"    { return NUM; }
"Speed"  { return SPEED; }
"Ang"    { return ANG; }
"Br"     { return BR; }
"Pos"    { return POS; }
"Prob"   { return PROB; }
"{"      { return LBRACKET; }
"}"      { return RBRACKET; }
","      { return COMMA; }
.        { }
