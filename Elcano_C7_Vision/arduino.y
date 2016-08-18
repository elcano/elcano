%{
#include <stdio.h>
#include <stdint.h>
#include "arduino.hh"

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char*);
namespace elcano { SerialData *data; }
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
	type body ;
type:
	DRIVE    { elcano::data->kind = elcano::MsgType::drive;  }
	| SENSOR { elcano::data->kind = elcano::MsgType::sensor; }
	| GOAL   { elcano::data->kind = elcano::MsgType::goal;   }
	| SEG    { elcano::data->kind = elcano::MsgType::seg;    } ;
body:
	value body | value ;
value:
	LBRACKET NUM INT RBRACKET             { elcano::data->number = $3;      }
	| LBRACKET SPEED INT RBRACKET         { elcano::data->speed = $3;       }
	| LBRACKET ANG INT RBRACKET           { elcano::data->angle = $3;       }
	| LBRACKET BR INT RBRACKET            { elcano::data->bearing = $3;     }
	| LBRACKET POS INT COMMA INT RBRACKET { elcano::data->posE = $3;
											elcano::data->posN = $5;        }
	| LBRACKET PROB INT RBRACKET          { elcano::data->probability = $3; } ;

%%

void yyerror(const char *s)
{
	fprintf(stderr, "Parsing error: %s\n", s);
}
