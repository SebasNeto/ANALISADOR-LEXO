/*
 * Compiladores - etapa1 - tokens.h - semestre 2023/1 
 *
 * Lista dos tokens, com valores constantes associados.
 * Este arquivo serah posterioremente substituido, nao acrescente nada.
 * Os valores das constantes sao arbitrarios, mas nao podem ser alterados.
 * Cada valor deve ser distinto e fora da escala ascii.
 * Assim, nao conflitam entre si e com os tokens representados pelo proprio
 * valor ascii de caracteres isolados.
 */


//tipos de dados do analisador 
#define KW_CHAR           256
#define KW_INT            257
#define KW_REAL           258
#define KW_BOOL           259

//tipos de comandos/declarações do analisador 
#define KW_IF             261
#define KW_THEN           262
#define KW_ELSE           263
#define KW_LOOP           264
#define KW_INPUT          266
#define KW_OUTPUT         267
#define KW_RETURN         268

////tipos de operadores do analisador 
#define OPERATOR_LE       270 // <=
#define OPERATOR_GE       271 // >=
#define OPERATOR_EQ       272 // ==
#define OPERATOR_DIF      273 // !=

#define OPERATOR_ATRIB    274 // = 
#define OPERATOR_GT       275 // >
#define OPERATOR_LT       276 // <

////tipos boleanos do analisador 
#define TK_BOOL_TRUE      277
#define TK_BOOL_FALSE     278

////tipos de dentificadores do analisador 
#define TK_IDENTIFIER     280

//tipos de literais do analisador 
#define LIT_INT           281 // int 10, 1
#define LIT_REAL          282 // real" 2.0
#define LIT_CHAR          285 // 'char 'X'
#define LIT_STRING        286 // "nome", "mensagem"

//tipos de simbolos do analisador 
#define SG_SEMICOLON      290 //;
#define OPEN_PAREN        292 //(
#define CLOSE_PAREN       293 //)
#define OPEN_BRACE        294 //{
#define CLOSE_BRACE       295 //}
#define OPEN_COLCH        296 //[
#define CLOSE_COLCH       297 //]
#define OPERADOR_SUM      298
#define OPERADOR_SUB      299
#define OPERADOR_ASTE     300
#define OPERADOR_VIR      301
#define OPERADOR_BARRA    302
#define OPERADOR_DIV      303
#define OPERADOR_ECOMERCIAL 304
#define OPERADOR_BARRAVERTICAL  305
#define OPERADOR_NEG      306
#define OPERATOR_EXCLAMARK  307
#define OPERATOR_ASPASDUPLAS    308



#define TOKEN_ERROR       390

//fim de arquivo
#define TOKEN_EOF         392

/* END OF FILE */

