%{
#include "tokens.h"
#include <stdio.h>
int getLineNumber(void);
void yyerror(const char*);
%}

%%

"char"               { return KW_CHAR; }
"int"                { return KW_INT; }
"real"               { return KW_REAL; }

[0-9]+               { return LIT_INT; }
[0-9]+[.][0-9]+      { return LIT_REAL; }
'.'                  { return LIT_CHAR; }

[a-zA-Z_][a-zA-Z_0-9]* { return ID; }

[ \t\r\n]+           /* Ignora espaços em branco e quebras de linha */
.                    { yyerror("Caractere inválido"); }

%%

int getLineNumber(void) {
    return yylineno;
}

void yyerror(const char *msg) {
    fprintf(stderr, "Erro léxico na linha %d: %s\n", getLineNumber(), msg);
    exit(1);
}
