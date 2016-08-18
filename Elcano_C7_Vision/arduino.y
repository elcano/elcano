%{
#include <stdio.h>
#include <stdint.h>
#include "arduino.hh"

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char*);
%}

%union {
	int32_t ival;
}

%token DRIVE SENSOR GOAL SEG
%token NUM SPEED ANG BR POS PROB
%token LBRACKET RBRACKET COMMA

%token <ival> INT

%%

message:
	type body { printf("Done reading a message\n"); } ;
type:
	DRIVE    { printf("Message->type = drive\n");  }
	| SENSOR { printf("Message->type = sensor\n"); }
	| GOAL   { printf("Message->type = goal\n");   }
	| SEG    { printf("Message->type = seg\n");    } ;
body:
	value body | value ;
value:
	LBRACKET NUM INT RBRACKET             { printf("Message->number = %d\n", $3);            }
	| LBRACKET SPEED INT RBRACKET         { printf("Message->speed = %d\n", $3);             }
	| LBRACKET ANG INT RBRACKET           { printf("Message->angle = %d\n", $3);             }
	| LBRACKET BR INT RBRACKET            { printf("Message->bearing = %d\n", $3);           }
	| LBRACKET POS INT COMMA INT RBRACKET { printf("Message->position = (%d,%d)\n", $3, $5); }
	| LBRACKET PROB INT RBRACKET          { printf("Message->probability = %d\n", $3);       } ;

%%

void yyerror(const char *s)
{
	fprintf(stderr, "Parsing error: %s\n", s);
}
