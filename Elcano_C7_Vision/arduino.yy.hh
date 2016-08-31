#pragma once

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse(elcano::SerialData *);
extern YY_BUFFER_STATE yy_scan_string(const char *);
extern void yy_delete_buffer(YY_BUFFER_STATE);
