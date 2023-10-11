%{

#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.h"


extern int yylex();
extern char* yytext;
extern int yylineno;


int getLineNumber(void);
void yyerror(const char *s);

%}

%union {
    int intValue;

}

%token <symbol> ID
%token INT VOID
%token LIT_INT LIT_REAL
%token NUM
%token LOOP
%token INPUT
%token IF ELSE WHILE RETURN
%token EQ LEQ LT GT GEQ NEQ PLUS MINUS TIMES DIV MOD
%token KW_CHAR KW_INT KW_REAL LIT_CHAR

%%

programa: lista_decl lista_com;

lista_decl: lista_decl decl
          | decl
          ;

decl: decl_var
    | decl_func
    ;

decl_var: espec_tipo var '=' init_val ';'
        ;

init_val: LIT_INT
        | LIT_CHAR
        | LIT_REAL
        | '{' literal_list '}'
        ;

literal_list: literal_list LIT_INT
            | literal_list LIT_CHAR
            | literal_list LIT_REAL
            | LIT_INT
            | LIT_CHAR
            | LIT_REAL
            ;

espec_tipo: INT
          | VOID
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
           | IF '(' exp ')' LOOP comando
           ;

com_repeticao: WHILE '(' exp ')' comando ';';

com_retorno: RETURN ';'
           | RETURN exp ';'
           ;

exp: exp_soma op_relac exp_soma
   | exp_soma
   | INPUT '(' espec_tipo ')'
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
        | LIT_CHAR
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

int getLineNumber(void) {
    return yylineno;
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", getLineNumber(), s);
    exit(3);
}
