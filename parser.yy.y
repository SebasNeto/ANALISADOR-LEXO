%{
#include "tokens.h"
#include <stdio.h>
extern int yylex();
extern char* yytext;
extern int getLineNumber();
void yyerror(const char *s);
%}

%token <symbol> ID
%token INT VOID FLOAT
%token LIT_INT LIT_REAL
%token NUM
%token IF ELSE WHILE RETURN

%token EQ LEQ LT GT GEQ NEQ PLUS MINUS TIMES DIV MOD

%%

programa: lista_decl lista_com;

lista_decl: lista_decl decl
          | decl
          ;

decl: decl_var
    | decl_func
    ;

decl_var: espec_tipo var ';'
        ;

espec_tipo: INT
          | VOID
          | FLOAT
          ;

decl_func: espec_tipo ID '(' params ')' com_comp
         ;

params: lista_param
      | VOID
      | /* vazio */
      ;

lista_param: lista_param ',' param
           | param
           ;

param: espec_tipo var;

decl_locais: decl_locais decl_var
           | /* vazio */
           ;

lista_com: comando lista_com
         | /* vazio */
         ;

comando: com_expr
       | com_atrib
       | com_comp
       | com_selecao
       | com_repeticao
       | com_retorno
       ;

com_expr: exp ';'
        | ';'
        ;

com_atrib: var '=' exp ';';

com_comp: '{' decl_locais lista_com '}';

com_selecao: IF '(' exp ')' comando
           | IF '(' exp ')' com_comp ELSE comando
           ;

com_repeticao: WHILE '(' exp ')' comando ';';

com_retorno: RETURN ';'
           | RETURN exp ';'
           ;

exp: exp_soma op_relac exp_soma
   | exp_soma
   ;

op_relac: LEQ
        | LT
        | GT
        | GEQ
        | EQ
        | NEQ
        ;

exp_soma: exp_soma op_soma exp_mult
        | exp_mult
        ;

op_soma: PLUS
       | MINUS
       ;

exp_mult: exp_mult op_mult exp_simples
        | exp_simples
        ;

op_mult: TIMES
       | DIV
       | MOD
       ;

exp_simples: '(' exp ')'
           | var
           | cham_func
           | literais
           ;

literais: NUM
        | LIT_REAL
        ;

cham_func: ID '(' args ')';

var: ID
   | ID '[' NUM ']'
   ;

args: lista_arg
    | /* vazio */
    ;

lista_arg: lista_arg ',' exp
         | exp
         ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", getLineNumber(), s);
    exit(3);
}
