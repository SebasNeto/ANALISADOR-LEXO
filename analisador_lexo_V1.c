
///////////////////////////////////// COMPONENTES /////////////////////////////////////////////////

/*
    SEBASTIAO BICHARRA NETO - MATRICULA = 22153624
    SOPHIA COSTA SEIXAS - 

*/

///////////////////////////////////// BIBLIOTECAS /////////////////////////////////////////////////

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "tokens.h"
#include <time.h>
#include <unistd.h>

///////////////////////////////////// DEFINIÇÕES TOKENS ///////////////////////////////////////////


////////////////////////////////////// VARIAVEIS GLOBAIS //////////////////////////////////////


#define HASH_SIZE 997 

int linhaCorrente = 1; //rastrear linha atual
int running = 1; // Variável global para rastrear se a análise está em execução

///////////////////////////////////// ESTRUTURA TABELA DE SIMBOLOS /////////////////////////////////

typedef struct Symbol {
    char *lexema;
    int token;
    struct Symbol *proximo; 
} Symbol;

Symbol *simboloTabela[HASH_SIZE];

///////////////////////////////////// ESTRUTURA TABELA HASH ///////////////////////////////////Q

unsigned int hash(char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31) + *str++; //evitar colisões 
    }
    return hash % HASH_SIZE; //dentro do limite da tabela 
}

Symbol* pesquisaLexema(char *lexema) {
    unsigned int index = hash(lexema);
    Symbol *node = simboloTabela[index];
    while (node) {
        if (strcmp(node->lexema, lexema) == 0) {
            return node;
        }
        node = node->proximo;
    }
    return NULL; // Não encontrado
}

Symbol* insert(char *lexema, int token) { //lexema (inserir) -> token (associado)

    unsigned int index = hash(lexema);//posição do lexema na tabela

    Symbol *novoNo = malloc(sizeof(Symbol));
    novoNo->lexema = strdup(lexema); // Faz uma cópia do lexema

    novoNo->token = token;

    novoNo->proximo = simboloTabela[index]; //verificando se o novo lexema está na tebala

    simboloTabela[index] = novoNo; // Insere no início da lista para lidar com colisões
    return novoNo;
}

void iniciarSimboloTabela(){
    for (int i=0 ;i<HASH_SIZE;++i){
        simboloTabela[i]=NULL;
    }
}
///////////////////////////////////////////////////////////////////////////q

int getLineNumber(void) {
    return linhaCorrente;
}

int isRunning(void) {
    return running;
}

///////////////////////////////////// DECLARAÇÕES DE FUNÇÕES //////////////////////////////////////

char prox_char(FILE *file) {
    char ch = fgetc(file); //lê o proximo caractere 
    if (ch == '\n') {
        linhaCorrente++;
    }
    return ch;
}

// void grava_token(int token, char *lexema, FILE *output) {
//     fprintf(output, "Token: %d, Lexema: %s\n", token, lexema);
// }

// void erro(char *message, FILE *output) {
//     fprintf(output, "Erro na linha %d: %s\n", linhaCorrente, message);
//     exit(1);
// }

//token (repesentação númerica) -> string (representação string)
const char* token_to_string(int token) {
    switch (token) {
        case TK_IDENTIFIER: return "TK_IDENTIFIER";
        case OPERATOR_ATRIB: return "OPERATOR_ATRIB";
        case KW_CHAR: return "KW_CHAR";
        case KW_INT: return "KW_INT";
        case KW_REAL: return "KW_REAL";
        case KW_BOOL: return "KW_BOOL";
        case KW_IF: return "KW_IF";
        case KW_THEN: return "KW_THEN";
        case KW_ELSE: return "KW_ELSE";
        case KW_LOOP: return "KW_LOOP";
        case KW_INPUT: return "KW_INPUT";
        case KW_OUTPUT: return "KW_OUTPUT";
        case KW_RETURN: return "KW_RETURN";
        case OPERATOR_LE: return "OPERATOR_LE";
        case OPERATOR_GE: return "OPERATOR_GE";
        case OPERATOR_EQ: return "OPERATOR_EQ";
        case OPERATOR_DIF: return "OPERATOR_DIF";
        case LIT_INT: return "LIT_INT";
        case LIT_REAL: return "LIT_REAL";
        case LIT_CHAR: return "LIT_CHAR";
        case LIT_STRING: return "LIT_STRING";
        case SG_SEMICOLON: return "SG_SEMICOLON";
        case OPERATOR_GT: return "OPERATOR_GT";
        case OPERATOR_LT: return "OPERATOR_LT";
        case TK_BOOL_TRUE: return "TK_BOOL_TRUE";
        case TK_BOOL_FALSE: return "TK_BOOL_FALSE";
        case TOKEN_ERROR: return "TOKEN_ERROR";

        //tratar nos caracteres especiais 
        case OPEN_PAREN : return "OPEN_PAREN";
        case CLOSE_PAREN : return "CLOSE_PAREN";
        case OPEN_BRACE : return "OPEN_BRACE";
        case CLOSE_BRACE : return "CLOSE_BRACE";


        default: return "UNKNOWN_TOKEN";
    }
}

void grava_token(int token, char *lexema, FILE *output) {
    fprintf(output, "%-15s %s\n", token_to_string(token), lexema);
}

void erro(char *message) {
    fprintf(stderr, "Erro na linha %d: %s\n", linhaCorrente, message);
    exit(1);
}

// int palavrasReservadasOrIdentificadores(char ch, FILE *input, char *lexema){

//     int idx = 0;


//     while (isalpha(ch) || ch == '_') {
//         lexema[idx++] = ch;
//         ch = prox_char(input);
//     }

//     lexema[idx] = '\0';

//     // Verifique as palavras reservadas
//     if (strcmp(lexema, "char") == 0) return KW_CHAR;
//     if (strcmp(lexema, "int") == 0) return KW_INT;
//     if (strcmp(lexema, "real") == 0) return KW_REAL;
//     if (strcmp(lexema, "bool") == 0) return KW_BOOL;
//     if (strcmp(lexema, "if") == 0) return KW_IF;
//     if (strcmp(lexema, "then") == 0) return KW_THEN;
//     if (strcmp(lexema, "else") == 0) return KW_ELSE;
//     if (strcmp(lexema, "loop") == 0) return KW_LOOP;
//     if (strcmp(lexema, "input") == 0) return KW_INPUT;
//     if (strcmp(lexema, "output") == 0) return KW_OUTPUT;
//     if (strcmp(lexema, "return") == 0) return KW_RETURN;
//     if (strcmp(lexema, "true") == 0) return TK_BOOL_TRUE; 
//     if (strcmp(lexema, "false") == 0) return TK_BOOL_FALSE;

//     // Se não for uma palavra reservada, é um identificador
//     Symbol* s = pesquisaLexema(lexema);
//     if (!s) {
//         s = insert(lexema, TK_IDENTIFIER);
//     }
//     return s->token;

// }

// int reconhecerLiterais(char ch, FILE *input, char *lexema){

//     int idx = 0;

//     if (isdigit(ch)) {
//         while (isdigit(ch)) {
//             lexema[idx++] = ch;
//             ch = prox_char(input);
//         }
        
//         if (ch == '.') {
//             lexema[idx++] = ch;
//             ch = prox_char(input);
//             while (isdigit(ch)) {
//                 lexema[idx++] = ch;
//                 ch = prox_char(input);
//             }
//             lexema[idx] = '\0';
//             return LIT_REAL;
//         }
//         lexema[idx] = '\0';
//         return LIT_INT;
//     }

//     if (ch == '\'') {
//         ch = prox_char(input);
//         lexema[idx++] = ch;
//         lexema[idx] = '\0';
//         ch = prox_char(input); // pegue o apóstrofo de fechamento
//         return LIT_CHAR;
//     }

//     if (ch == '\"') {
//         ch = prox_char(input);
//         while (ch != '\"') {
//             lexema[idx++] = ch;
//             ch = prox_char(input);
//         }
//         lexema[idx] = '\0';
//         return LIT_STRING;
//     }

//     return TOKEN_ERROR;

// }

int analex(FILE *input, char *lexema) {
    char ch = prox_char(input);
    printf("--------------\n");
	printf("Entrou antes do ispace\n");

    while (isspace(ch)) {
        ch = prox_char(input);  
    }

    char ch_proximo = fgetc(input);

    printf("pegou o proximo %c\n", ch_proximo);
    ungetc(ch, input);
    ungetc(ch_proximo, input);
    ungetc(ch_proximo, input);
    printf("Valor do caractere: %c\n", ch);
    printf("|%s| ", lexema); 
    int idx = 0;

    if (isspace(ch)) {
        return analex(input, lexema);
    }

    // Reconhecendo palavras reservadas e identificadores
    if (isalpha(ch) || ch == '_') {
        while (isalpha(ch) || ch == '_') {
            lexema[idx++] = ch;
            ch = prox_char(input);
        }
        lexema[idx] = '\0';

        // Verifique as palavras reservadas
        if (strcmp(lexema, "char") == 0) return KW_CHAR;
        if (strcmp(lexema, "int") == 0) return KW_INT;
        if (strcmp(lexema, "real") == 0) return KW_REAL;
        if (strcmp(lexema, "bool") == 0) return KW_BOOL;
        if (strcmp(lexema, "if") == 0) return KW_IF;
        if (strcmp(lexema, "then") == 0) return KW_THEN;
        if (strcmp(lexema, "else") == 0) return KW_ELSE;
        if (strcmp(lexema, "loop") == 0) return KW_LOOP;
        if (strcmp(lexema, "input") == 0) return KW_INPUT;
        if (strcmp(lexema, "output") == 0) return KW_OUTPUT;
        if (strcmp(lexema, "return") == 0) return KW_RETURN;
        if (strcmp(lexema, "true") == 0) return TK_BOOL_TRUE; 
        if (strcmp(lexema, "false") == 0) return TK_BOOL_FALSE;

        // Se não for uma palavra reservada, é um identificador
        Symbol* s = pesquisaLexema(lexema);
        if (!s) {
            s = insert(lexema, TK_IDENTIFIER);
        }
        return s->token;
    }

    // Reconhecendo caracteres especiais e operadores compostos
    if (strchr(",;()[]{}=+-*/%&|~>", ch)) {
        ch_proximo = prox_char(input);
        ungetc(ch_proximo, input); // coloca o caractere de volta no stream

        if (ch == '<' && ch_proximo == '=') {
            prox_char(input);
            return OPERATOR_LE;
        } else if (ch == '>' && ch_proximo == '=') {
            prox_char(input);
            return OPERATOR_GE;
        } else if (ch == '=' && ch_proximo == '=') {
            prox_char(input);
            return OPERATOR_EQ;
        } else if (ch == '!' && ch_proximo == '=') {
            prox_char(input);
            return OPERATOR_DIF;
        }else if(ch == '>'){
            return OPERATOR_GT;
        }else if(ch == '<'){
            return OPERATOR_LT;
        }else if(ch == '='){
            return OPERATOR_ATRIB;
        }else {
            return ch; // Retorne o caractere ASCII para caracteres simples
        }
    }

    // Reconhecendo literais

    if (isdigit(ch)) {
        while (isdigit(ch)) {
            lexema[idx++] = ch;
            ch = prox_char(input);
        }
        
        if (ch == '.') {
            lexema[idx++] = ch;
            ch = prox_char(input);
            while (isdigit(ch)) {
                lexema[idx++] = ch;
                ch = prox_char(input);
            }
            lexema[idx] = '\0';
            return LIT_REAL;
        }
        lexema[idx] = '\0';
        return LIT_INT;
    }

    if (ch == '\'') {
        ch = prox_char(input);
        lexema[idx++] = ch;
        lexema[idx] = '\0';
        ch = prox_char(input); // pegue o apóstrofo de fechamento
        return LIT_CHAR;
    }

    if (ch == '\"') {
        ch = prox_char(input);
        while (ch != '\"') {
            lexema[idx++] = ch;
            ch = prox_char(input);
        }
        lexema[idx] = '\0';
        return LIT_STRING;
    }

    // Ignorando comentários
    if (ch == '\\' && ch_proximo == '\\') {
        while (ch != '\n') {
            ch = prox_char(input);
        }
        return analex(input, lexema); // continue a análise sem retornar um token
    }

    if (ch == '\\' && ch_proximo == '*') {
        ch = prox_char(input); // avance para o próximo caractere
        ch_proximo = prox_char(input);
        while (!(ch == '*' && ch_proximo == '\\')) {
            ch = prox_char(input);
            ch_proximo = prox_char(input); 
        }
        prox_char(input); 
        return analex(input, lexema); 
    }

    if (feof(input)) {
        running = 0;
        return TOKEN_EOF; 
    }

	//idx++;
    fprintf(stderr, "Erro na linha %d: Caractere não reconhecido: %c \n", linhaCorrente, ch);
    return TOKEN_ERROR; // Token de erro
}

///////////////////////////////////// MAIN ////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

    // if (argc < 2) {
    //     printf("Uso: %s <nome do arquivo de entrada>\n", argv[0]);
    //     return 1;
    // }

    FILE *input = fopen(argv[1], "r");;
    FILE *output = fopen("output10.txt", "w");

    if (input == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // while (!feof(input)) {
    //     char lexema[256]; // Supondo um tamanho máximo para simplificar
    //     int token = analex(input, lexema);
    //     grava_token(token, lexema, output);
    // }

    int token;
    do {
        char lexema[256];
        token = analex(input, lexema);
        grava_token(token, lexema, output);
        printf("Token: %d\n", token);
        printf("Valor do lexema: %s\n", lexema);
        sleep(1);
    } while (token != TOKEN_EOF);

    printf(" \n\n CÓDIGO COMPILADO COM SUCESSO \n \n");

    fclose(input);
    fclose(output);
    return 0;
}

///////////////////////////////////// IMPLEMENTAÇÃO DAS FUNÇÕES ////////////////////////////////////