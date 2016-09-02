%{
#include "arduino.hh"

extern int yylex();
extern int yyparse();
extern "C" FILE *yyin;

void yyerror(elcano::SerialData*, const char*);
%}

%parse-param { elcano::SerialData *dd }

%union {
	int32_t ival;
	elcano::MsgType mval;
}

%token <ival> INT
%type <mval> type

%%

message:
	type body { dd->kind = $1; } ;
type:
	'D'   { $$ = elcano::MsgType::drive;  }
	| 'S' { $$ = elcano::MsgType::sensor; }
	| 'G' { $$ = elcano::MsgType::goal;   }
	| 'X' { $$ = elcano::MsgType::seg;    } ;
body:
	value body | value ;
value:
	'{' 'n' INT '}'           { dd->number = $3;      }
	| '{' 's' INT '}'         { dd->speed = $3;       }
	| '{' 'a' INT '}'         { dd->angle = $3;       }
	| '{' 'b' INT '}'         { dd->bearing = $3;     }
	| '{' 'p' INT ',' INT '}' { dd->posE = $3;
	                            dd->posN = $5;        }
	| '{' 'r' INT '}'         { dd->probability = $3; } ;

%%

void yyerror(elcano::SerialData *, const char *s)
{
	std::cerr << "Parsing Error: " << s << std::endl;
}
