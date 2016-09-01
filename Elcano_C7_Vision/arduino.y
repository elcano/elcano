%{
#include "arduino.hh"

extern "C" int yylex();
extern int yyparse();
extern "C" FILE *yyin;

void yyerror(elcano::SerialData*, const char*);
%}

%parse-param { elcano::SerialData *dd }

%union {
	int32_t ival;
	elcano::MsgType mval;
}

%token DRIVE SENSOR GOAL SEG
%token NUM SPEED ANG BR POS PROB
%token LBRACKET RBRACKET COMMA

%token <ival> INT
%type <mval> type

%%

message:
	type body { dd->kind = $1; } ;
type:
	DRIVE    { $$ = elcano::MsgType::drive;  }
	| SENSOR { $$ = elcano::MsgType::sensor; }
	| GOAL   { $$ = elcano::MsgType::goal;   }
	| SEG    { $$ = elcano::MsgType::seg;    } ;
body:
	value body | value ;
value:
	LBRACKET NUM INT RBRACKET             { dd->number = $3;      }
	| LBRACKET SPEED INT RBRACKET         { dd->speed = $3;       }
	| LBRACKET ANG INT RBRACKET           { dd->angle = $3;       }
	| LBRACKET BR INT RBRACKET            { dd->bearing = $3;     }
	| LBRACKET POS INT COMMA INT RBRACKET { dd->posE = $3;
	                                        dd->posN = $5;        }
	| LBRACKET PROB INT RBRACKET          { dd->probability = $3; } ;

%%

void yyerror(elcano::SerialData *, const char *s)
{
	std::cerr << "Parsing Error: " << s << std::endl;
}
