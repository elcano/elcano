%{
#include <stdlib.h>
#include "arduino.hh"
#include "arduino.tab.hh"
%}

%option noyywrap noinput nounput

%%

[ \t\n\r]         { }
-?[0-9]+          { yylval.ival = atoi(yytext); return INT; }
[DSGXnsabpr,\{\}] { return *yytext; }
.                 { }
