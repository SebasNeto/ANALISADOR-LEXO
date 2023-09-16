
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
#include <unistd.h>
#include "stdbool.h"

///////////////////////////////////// DEFINIÇÕES TOKENS ///////////////////////////////////////////


////////////////////////////////////// VARIAVEIS GLOBAIS //////////////////////////////////////


#define HASH_SIZE 997 

int linhaCorrente = 1; //rastrear linha atual
int running = 1; // Variável global para rastrear se a análise está em execução
///////////////////////////////////// ESTRUTURA TABELA DE SIMBOLOS /////////////////////////////////

typedef struct Symbol {
    char *lexeme;
    int token;
    struct Symbol *proximo; 
} Symbol;

Symbol *simboloTabela[HASH_SIZE];

///////////////////////////////////// ESTRTURA TABELA HASH ///////////////////////////////////Q

unsigned int hash(char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31) + *str++;
    }
    return hash % HASH_SIZE;
}

Symbol* pesquisaLexema(char *lexeme) {
    unsigned int index = hash(lexeme);
    Symbol *node = simboloTabela[index];
    while (node) {
        if (strcmp(node->lexeme, lexeme) == 0) {
            return node;
        }
        node = node->proximo;
    }
    return NULL; // Não encontrado
}

Symbol* insert(char *lexeme, int token) {
    unsigned int index = hash(lexeme);
    Symbol *novoNo = malloc(sizeof(Symbol));
    novoNo->lexeme = strdup(lexeme); // Faz uma cópia do lexema
    novoNo->token = token;
    novoNo->proximo = simboloTabela[index];
    simboloTabela[index] = novoNo; // Insere no início da lista para lidar com colisões
    printf("O lexema inserido foi: %s \n", lexeme);
    return novoNo;
}

void initSimboloTabela() {
    for (int i = 0; i < HASH_SIZE; i++) {
        simboloTabela[i] = NULL;
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

    char ch = fgetc(file);

    if(ch == EOF){
        return ch;
    }else if(ch == '\n'){
        linhaCorrente ++;
    }

    

    // if (ch == '\r') {
    //     ch = fgetc(file);
    //     if (ch != '\n') {
    //         fprintf(stderr, "Erro: encontrado '\r' sem '\n' em seguida na linha %d.\n", linhaCorrente);
    //     }
    // }

    // if (ch == '\n') {
    //     linhaCorrente++;
    // }

    printf("Linha atual: %d \n", linhaCorrente);
    printf("Caractere atual prox_char: %c \n", ch);

    return ch;

    
}


// void grava_token(int token, char *lexema, FILE *output) {
//     fprintf(output, "Token: %d, Lexema: %s\n", token, lexema);
// }

// void erro(char *message, FILE *output) {
//     fprintf(output, "Erro na linha %d: %s\n", linhaCorrente, message);
//     exit(1);
// }

const char* token_to_string(int token) {
    switch (token) {

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

        case OPERATOR_ATRIB: return "OPERATOR_ATRIB";
        case OPERATOR_GT: return "OPERATOR_GT";
        case OPERATOR_LT: return "OPERATOR_LT";

        case TK_BOOL_TRUE: return "TK_BOOL_TRUE";
        case TK_BOOL_FALSE: return "TK_BOOL_FALSE";

        case TK_IDENTIFIER: return "TK_IDENTIFIER";


        case LIT_INT: return "LIT_INT";
        case LIT_REAL: return "LIT_REAL";
        case LIT_CHAR: return "LIT_CHAR";
        case LIT_STRING: return "LIT_STRING";


        case SG_SEMICOLON: return "SG_SEMICOLON";
        case OPEN_PAREN: return "OPEN_PAREN";
        case CLOSE_PAREN: return "CLOSE_PAREN";
        case OPEN_BRACE : return "OPEN_BRACE";
        case CLOSE_BRACE : return "CLOSE_BRACE";

        case TOKEN_ERROR: return "TOKEN_ERROR";

        case TOKEN_EOF : return "TOKEN_EOF";
        

        default: 
        printf("Erro: token não reconhecido com valor: %d\n", token);
        return "UNKNOWN_TOKEN";
    }
}

void grava_token(int token, char *lexema, FILE *output) {
    fprintf(output, "%-15s %s\n", token_to_string(token), lexema);
}

void erro(char *message) {
    fprintf(stderr, "Erro na linha %d: %s\n", linhaCorrente, message);
    exit(1);
}

int reconheceIdentificadoresORReservadas(char ch, FILE *input, char *lexema){

    int idx = 0;
    if (isalpha(ch) || ch == '_') {
        while (isalpha(ch) || ch == '_') {
            lexema[idx++] = ch;
            ch = prox_char(input);
        }

        //ungetc(ch, input); // Devolve o último caractere não utilizado para o buffer
        lexema[idx] = '\0';
        
        

        // Verifique as palavras reservadas
        if (strcmp(lexema, "char") == 0){
            
            // lexema[0] = 'c';
            // lexema[1] = 'h';
            // lexema[2] = 'a';
            // lexema[3] = 'r';
            // lexema[4] = '\0';
            //prox_char(input);
            return LIT_CHAR;
            
        }
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
        if (strcmp(lexema, "true") == 0){
            ungetc(ch,input);
            
            // prox_char(input);
            // lexema[0] = 't';
            // lexema[1] = 'r';
            // lexema[2] = 'u';
            // lexema[3] = 'e';
            // lexema[4] = '\0';
            //prox_char(input);
            return TK_BOOL_TRUE;
        } 
        if (strcmp(lexema, "false") == 0){
            ungetc(ch,input);
            //prox_char(input);
            return TK_BOOL_FALSE;
        }


        // Se não for uma palavra reservada, é um identificador - variaveis, vetores
        Symbol* s = pesquisaLexema(lexema);
        if (!s) {
            s = insert(lexema, TK_IDENTIFIER);
        }
        return s->token;
    }
}

int reconheceOperadoresCompostos(char ch, FILE *input, char *lexema){
    int idx = 0;
    char ch_proximo = prox_char(input);
    if (strchr("< > = ! ", ch)) {
        //ch_proximo = prox_char(input);
        //ungetc(ch_proximo, input); // coloca o caractere de volta no stream
        if (ch == '<' && ch_proximo == '=') {
            lexema[0] = '<';
            lexema[1] = '=' ;
            lexema[2] = '\0';
            //prox_char(input);
            return OPERATOR_LE;
        } else if (ch == '>' && ch_proximo == '=') {
            lexema[0] = '>';
            lexema[1] = '=' ;
            lexema[2] = '\0';
            //prox_char(input);
            return OPERATOR_GE;
        } else if (ch == '=' && ch_proximo == '=') {
            lexema[0] = '=';
            lexema[1] = '=';
            lexema[2] = '\0';
            //prox_char(input);
            return OPERATOR_EQ;
        } else if (ch == '!' && ch_proximo == '=') {
            lexema[0] = '!';
            lexema[1] = '=';
            lexema[2] = '\0';
            //prox_char(input);
            return OPERATOR_DIF;
        }else if (ch == '>'){
            lexema[0] = '>';
            lexema[1] = '\0';
            return OPERATOR_GT;
        }else if(ch == '<'){
            lexema[0] = '<';
            lexema[1] = '\0';
            return OPERATOR_LT;
        }else if(ch == '='){
            lexema[0] = '=';
            lexema[1] = '\0';
            return OPERATOR_ATRIB;
        }
        
        lexema[0] = ch;
        lexema[1]='\0';


        return ch;

    }


}

int reconheceCaracteresEspeciais(char ch, FILE *input, char *lexema){

    
    if (strchr(", ; () []  {} + - * / %  & | ~", ch)) {
        //ch_proximo = prox_char(input);
        //ungetc(ch_proximo, input);

        if(ch == ';'){
            lexema[0] = ';';
            lexema[1] = '\0';
            
            return SG_SEMICOLON;
        }
        

        if(ch == '('){
            lexema[0] = '(';
            lexema[1] = '\0';
            return OPEN_PAREN;
        }else if(ch == ')'){

            lexema[0] = ')';
            lexema[1] = '\0';
            return CLOSE_PAREN;   
        }

        // if(ch == '('){
        //     lexema[0] = '(';
        //     //lexema[1] = '\0';
        //     return OPEN_PAREN;
        // }else if(ch == ')'){
        //     lexema[0] = ')';
        //     lexema[1] = '\0';
        //     return CLOSE_PAREN;   
        // }

        if(ch == '{'){
            lexema[0] = '{';
            lexema[1] = '\0';
            return OPEN_BRACE;   
        }else if(ch == '}'){
            lexema[0] = '}';
            lexema[1] = '\0';
            return CLOSE_BRACE;  
        }
    


        lexema[0] = ch;
        lexema[1]='\0';


        //ch_proximo = prox_char(input);
        //ungetc(ch_proximo, input); // coloca o caractere de volta no stream

        return ch;
    }

}

int reconheceLiterais(char ch, FILE *input, char *lexema){
    int idx = 0;
    
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
            ungetc(ch, input);
            return LIT_REAL;
        }

        lexema[idx] = '\0';
        ungetc(ch, input);
        return LIT_INT;
    }

    //comentários
    if (ch == '\'') {
        ch = prox_char(input);
        lexema[idx++] = ch;
        ch = prox_char(input); // pegue o apóstrofo de fechamento
        lexema[idx] = '\0';
        ungetc(ch, input); 
        return LIT_CHAR;
    }

    if (ch == '\"') {
        ch = prox_char(input);
        while (ch != '\"') {
           lexema[idx++] = ch;
           ch = prox_char(input);
        }
        ch = prox_char(input);
        lexema[idx] = '\0';
        ungetc(ch, input);
        return LIT_STRING;
    }

    lexema[0] = ch;
    lexema[1] = '\0';
    return ch;
}

// int ignorandoComentarios(char ch, FILE *input, char *lexema){

//     char ch_proximo = prox_char(input);
//     // Ignorando comentário
//     if (ch == '\\' && ch_proximo == '\\') {
//         while (ch != '\n') {
//             ch = prox_char(input);
//         }
//         return analex(input, lexema); // continue a análise sem retornar um token
//     }

//     if (ch == '\\' && ch_proximo == '*') {
//         ch = prox_char(input); // avance para o próximo caractere
//         ch_proximo = prox_char(input);
//         while (!(ch == '*' && ch_proximo == '\\')) {
//             ch = prox_char(input);
//             ch_proximo = prox_char(input); 
//         }
//         prox_char(input); 
//         return analex(input, lexema); 
//     }
// }
int ignorandoComentarios(char ch, FILE *input) {
    char ch_proximo = prox_char(input);
    ungetc(ch_proximo, input); // Devolve o caractere para não consumir o próximo caractere

    if (ch == '\\' && ch_proximo == '\\') {
        while (ch != '\n') {
            ch = prox_char(input);
        }
        return 1; // Indica que um comentário foi detectado
    }

    if (ch == '\\' && ch_proximo == '*') {
        ch = prox_char(input); // avance para o próximo caractere
        ch_proximo = prox_char(input);
        while (!(ch == '*' && ch_proximo == '\\')) {
            ch = prox_char(input);
            ch_proximo = prox_char(input); 
        }
        prox_char(input); 
        return 1; // Indica que um comentário foi detectado
    }

    return 0; // Nenhum comentário foi detectado
}

int analex(FILE *input, char *lexema) {

    char ch = prox_char(input);
    printf(" \n--------------\n");
    int idx = 0;
    //printf("Valor do caractere analex: %c\n", ch);

    while (isspace(ch)) {
        ch = prox_char(input);  
    }

    //char ch_proximo = prox_char(input);
    //printf("Valor do próximo caractere analex: %c \n", ch_proximo);
    //ungetc(ch_proximo, input);

    if(isalpha(ch) || ch == '_'){
        return reconheceIdentificadoresORReservadas(ch, input, lexema);
    }

    if(strchr("< > = ! ", ch)){
        return reconheceOperadoresCompostos(ch , input, lexema );
    }

    if(strchr(", ; () [] {} + - * / %  & | ~", ch)){
        return reconheceCaracteresEspeciais(ch, input, lexema);
    }
    int token = reconheceLiterais(ch, input, lexema);
    if (token != ch) { 
        return token;
    }

    if (ignorandoComentarios(ch, input)) {
        return analex(input, lexema); // Se detectou um comentário, continue a análise sem retornar um token
    }

    if (ch == EOF) {
        lexema[0] = '\0';
        running = 0;
        return TOKEN_EOF; 
    }

    lexema[0] = ch;
    lexema[1] = '\0';
    fprintf(stderr, "Erro na linha %d: Caractere não reconhecido: %c \n", linhaCorrente, ch);
    return TOKEN_ERROR; // Token de erro
}



///////////////////////////////////// MAIN ////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

    initSimboloTabela();
    FILE *input = fopen(argv[1], "r");;
    FILE *output = fopen("output29.txt", "w");

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
        //sleep(1);
    } while (token != TOKEN_EOF);

    printf(" \n\n CÓDIGO COMPILADO COM SUCESSO \n \n");

    fclose(input);
    fclose(output);
    return 0;
}

///////////////////////////////////// IMPLEMENTAÇÃO DAS FUNÇÕES ////////////////////////////////////