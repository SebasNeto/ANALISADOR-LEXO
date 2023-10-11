%option yylineno
%option noyywrap

%{

#include "parser.tab.h"
#include <stdio.h>
void yyerror(const char *s);

%}

%%

"char"               { return KW_CHAR; }
"int"                { printf("TOKEN: KW_INT\n"); return KW_INT; }
"real"               { printf("TOKEN: KW_REAL\n"); return KW_REAL; }
"void"               { return VOID; }
"if"                 { return IF; }
"else"               { return ELSE; }
"while"              { return WHILE; }
"loop"  { return LOOP; }
"input" { return INPUT; }

"return"             { printf("TOKEN: KW_RETURN\n"); return RETURN; }

"=="                 { return EQ; }
"<="                 { return LEQ; }
"<"                  { return LT; }
">"                  { return GT; }
">="                 { return GEQ; }
"!="                 { return NEQ; }
"+"                  { return PLUS; }
"-"                  { return MINUS; }
"*"                  { return TIMES; }
"/"                  { return DIV; }
"%"                 { return MOD; }

[0-9]+               { printf("TOKEN: LIT_INT\n"); return LIT_INT; }
[0-9]+[.][0-9]+      { printf("TOKEN: LIT_REAL\n"); return LIT_REAL; }
'.'                  { printf("TOKEN: LIT_CHAR\n"); return LIT_CHAR; }

[a-zA-Z_][a-zA-Z_0-9]* { printf("TOKEN: ID (%s)\n", yytext); return ID; }

"//"[^"\n"]*          { /* Ignora comentários de uma linha */ }
"/*"(.|\n)*?"*/"     { /* Ignora comentários de várias linhas */ }

[ \t\r\n]+           /* Ignora espaços em branco e quebras de linha */
.                    { yyerror("Caractere inválido"); }

%%

