
///////////////////////////////////// COMPONENTES /////////////////////////////////////////////////

/*
    SEBASTIAO BICHARRA NETO - MATRICULA = 22153624
    SOPHIA COSTA SEIXAS - MATRICULA = 22154533

*/

///////////////////////////////////// BIBLIOTECAS /////////////////////////////////////////////////

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "tokens.h"
#include <unistd.h>
#include "stdbool.h"

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

unsigned int hash(char *str);

Symbol* pesquisaLexema(char *lexeme);

Symbol* insert(char *lexeme, int token);

void initSimboloTabela();

void printSimboloTabela();


////////////////////////////////////FUNÇÕES AUXILIARES PARA TESTE/////////////////////////////

int getLineNumber(void);
int isRunning(void);

///////////////////////////////////// DECLARAÇÕES DE FUNÇÕES PARA ANALISE LEXICA //////////////////////////////////////

char prox_char(FILE *file);

const char* token_to_string(int token);

void grava_token(int token, char *lexema, FILE *output);

int reconheceIdentificadoresORReservadas(char ch, FILE *input, char *lexema);

int reconheceOperadoresCompostos(char ch, FILE *input, char *lexema);

int reconheceCaracteresEspeciais(char ch, FILE *input, char *lexema);

int reconheceLiterais(char ch, FILE *input, char *lexema);


int ignorandoComentarios(char ch, FILE *input);

void erro(const char *mensagem, const char *caractereErro, FILE *output);

int analex(FILE *input, char *lexema,FILE *output);



///////////////////////////////////// MAIN ////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

    initSimboloTabela();
    
    FILE *input = fopen(argv[1], "r");;
    FILE *output = fopen("output30.txt", "w");

    if (input == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    while (isRunning()) {
        char lexema[256];
        int token = analex(input, lexema,output);
        grava_token(token, lexema, output);
        printf("Token: %d\n", token);
        printf("Valor do lexema: %s\n", lexema);
    }


    //printSimboloTabela();
    printSimboloTabela(output);
    
    printf(" \n\n CÓDIGO COMPILADO COM SUCESSO \n \n");

    fclose(input);
    fclose(output);
    
    return 0;
}

///////////////////////////////////// IMPLEMENTAÇÃO DAS FUNÇÕES ////////////////////////////////////

///////////////////////////////////// TABELA //////////////////////////////////////////////////////

//Calcular um valor numérico (hash) para uma string.
unsigned int hash(char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31) + *str++;
    }
    return hash % HASH_SIZE;
}

//função que insere um dado lexema na tabela de simbolos
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

//função que pesquisa um dado lexema na tabela de simbolos
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

//função que inicializa tabela de simbolos
void initSimboloTabela() {
    for (int i = 0; i < HASH_SIZE; i++) {
        simboloTabela[i] = NULL;
        
    }
}

void printSimboloTabela(FILE *output) {
    fprintf(output, "\nTabela de Símbolos:\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        if (simboloTabela[i] != NULL) {
            Symbol *atual = simboloTabela[i];
            while (atual) {
                fprintf(output, "Posição %d: Lexema = %s, Token = %d\n", i, atual->lexeme, atual->token);
                atual = atual->proximo;
            }
        }
    }
    fprintf(output, "\nFim da Tabela de Símbolos\n");
}


////////////////////////////////////FUNÇÕES AUXILIARES PARA TESTE/////////////////////////////

//linha atual
int getLineNumber(void) {
    return linhaCorrente;
}

//analise em execução
int isRunning(void) {
    return running;
}


///////////////////////////////////// DECLARAÇÕES DE FUNÇÕES PARA ANALISE LEXICA //////////////////////////////////////

// Retorna o próximo caractere do arquivo e atualiza o contador de linhas.
char prox_char(FILE *file) {

    char ch = fgetc(file);

    if(ch == EOF){
        return ch;
    }else if(ch == '\n'){
        linhaCorrente ++;
    }

    return ch;
    
}

// Converte um token numérico em sua representação de string.
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

        case OPEN_COLCH : return "OPEN_COLCH";
        case CLOSE_COLCH : return "CLOSE_COLCH";
        case OPERADOR_SUM :  return "OPERADOR_SUM";
        case OPERADOR_SUB :  return "OPERADOR_SUB";
        case OPERADOR_ASTE : return "OPERADOR_ASTE";
        case OPERADOR_VIR : return "OPERADOR_VIR";
        case OPERADOR_BARRA : return "OPERADOR_BARRA";
        case OPERADOR_DIV : return "OPERADOR_DIV";
        case OPERADOR_ECOMERCIAL : return "OPERADOR_ECOMERCIAL";
        case OPERADOR_BARRAVERTICAL : return "OPERADOR_BARRAVERTICAL";
        case OPERADOR_NEG : return "OPERADOR_NEG";
        case OPERATOR_EXCLAMARK: return "OPERATOR_EXCLAMARK";
        case OPERATOR_ASPASDUPLAS : return "OPERATOR_ASPASDUPLAS";

        case TOKEN_EOF : return "TOKEN_EOF";
        

        default: 
        printf("Erro: token não reconhecido com valor: %d, linha %d: \n\n", token,linhaCorrente);
        case TOKEN_ERROR: return "TOKEN_ERROR";
    }
}

// Grava a representação em string do token e o lexema no arquivo de saída.
void grava_token(int token, char *lexema, FILE *output) {
    fprintf(output, "%-15s %s\n", token_to_string(token), lexema);
}

// Reconhece identificadores e palavras reservadas.
int reconheceIdentificadoresORReservadas(char ch, FILE *input, char *lexema){

    int idx = 0;
    if (isalpha(ch) || ch == '_') {
        while (isalpha(ch) || ch == '_') {
            lexema[idx++] = ch;
            ch = prox_char(input);
        }

        ungetc(ch, input); // Devolve o último caractere não utilizado para o buffer
        lexema[idx] = '\0';
        

        // Verifique as palavras reservadas
        if (strcmp(lexema, "char") == 0){
        
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

            return TK_BOOL_TRUE;
        } 
        if (strcmp(lexema, "false") == 0){
            ungetc(ch,input);
            
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

// Reconhece operadores compostos como <=, >=, ==, etc.
int reconheceOperadoresCompostos(char ch, FILE *input, char *lexema){
    int idx = 0;
    char ch_proximo = prox_char(input);
    if (strchr("< > = ! ", ch)) {

        if (ch == '<' && ch_proximo == '=') {
            lexema[0] = '<';
            lexema[1] = '=' ;
            lexema[2] = '\0';
            return OPERATOR_LE;
        } else if(ch == '"' && ch_proximo == '"'){
            lexema[0] = '"';
            lexema[1] = '"' ;
            lexema[2] = '\0';
            return OPERATOR_ASPASDUPLAS;
        }else if (ch == '>' && ch_proximo == '=') {
            lexema[0] = '>';
            lexema[1] = '=' ;
            lexema[2] = '\0';
            return OPERATOR_GE;
        } else if (ch == '=' && ch_proximo == '=') {
            lexema[0] = '=';
            lexema[1] = '=';
            lexema[2] = '\0';
            return OPERATOR_EQ;
        } else if (ch == '!' && ch_proximo == '=') {
            lexema[0] = '!';
            lexema[1] = '=';
            lexema[2] = '\0';
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
        }else if(ch == '!'){
            lexema[0] = '!';
            lexema[1] = '\0';
            return OPERATOR_EXCLAMARK;
        }
        
        lexema[0] = ch;
        lexema[1]='\0';


        return ch;

    }
}

// Reconhece caracteres especiais como símbolos e operadores.
int reconheceCaracteresEspeciais(char ch, FILE *input, char *lexema){

    if (strchr(", ; () []  {} + - * / %  & | ~", ch)) {

        if(ch == ';'){
            lexema[0] = ';';
            lexema[1] = '\0';
            return SG_SEMICOLON;
        }else if(ch == ','){
            lexema[0] = ',';
            lexema[1] = '\0';
            return OPERADOR_VIR;
        }else if(ch == '+'){
            lexema[0] = '+';
            lexema[1] = '\0';
            return OPERADOR_SUM;
        }else if(ch == '-'){
            lexema[0] = '-';
            lexema[1] = '\0';
            return OPERADOR_SUB;
        }else if(ch == '*'){
            lexema[0] = '*';
            lexema[1] = '\0';
            return OPERADOR_ASTE;
        }else if(ch == '/'){
            lexema[0] = '/';
            lexema[1] = '\0';
            return OPERADOR_BARRA;
        }else if(ch == '%'){
            lexema[0] = '%';
            lexema[1] = '\0';
            return OPERADOR_DIV;
        }else if(ch == '&'){
            lexema[0] = '&';
            lexema[1] = '\0';
            return OPERADOR_ECOMERCIAL;
        }else if(ch == '|'){
            lexema[0] = '|';
            lexema[1] = '\0';
            return OPERADOR_BARRAVERTICAL;
        }else if(ch == '~'){
            lexema[0] = '~';
            lexema[1] = '\0';
            return OPERADOR_NEG;
        }else if(ch == '('){
            lexema[0] = '(';
            lexema[1] = '\0';
            return OPEN_PAREN;
        }else if(ch == ')'){

            lexema[0] = ')';
            lexema[1] = '\0';
            return CLOSE_PAREN;   
        }else if(ch == '{'){
            lexema[0] = '{';
            lexema[1] = '\0';
            return OPEN_BRACE;   
        }else if(ch == '}'){
            lexema[0] = '}';
            lexema[1] = '\0';
            return CLOSE_BRACE;  
        }else if(ch == '['){
            lexema[0] = '[';
            lexema[1] = '\0';
            return OPEN_COLCH;   
        }else if(ch == ']'){
            lexema[0] = ']';
            lexema[1] = '\0';
            return CLOSE_COLCH;  
        }
    
        lexema[0] = ch;
        lexema[1]='\0';

        return ch;
    }

}

// Reconhece literais como números, caracteres e strings.
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
            insert(lexema, LIT_REAL);
            return LIT_REAL;
        }

        lexema[idx] = '\0';
        ungetc(ch, input);
        insert(lexema, LIT_INT);
        return LIT_INT;
    }

    //comentários
    if (ch == '\'') {
        ch = prox_char(input);
        lexema[idx++] = ch;
        ch = prox_char(input); // pegue o apóstrofo de fechamento
        lexema[idx] = '\0';
        ungetc(ch, input);
        insert(lexema, LIT_CHAR);
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
        insert(lexema, LIT_STRING);
        return LIT_STRING;
    }


    lexema[0] = ch;
    lexema[1] = '\0';
    return ch;
}

// Ignora comentários no arquivo de entrada.
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

// Exibe uma mensagem de erro no arquivo de saída.
void erro(const char *mensagem, const char *caractereErro, FILE *output) {
    fprintf(output, "Erro na linha %d: %s - Caractere: %s\n", linhaCorrente, mensagem, caractereErro);
    running = 0; // Encerra a análise
}

// Função principal para análise léxica.
int analex(FILE *input, char *lexema,FILE *output) {

    char ch = prox_char(input);
    printf(" \n--------------\n");
    int idx = 0;

    int token;

    while (isspace(ch)) {
        ch = prox_char(input);  
    }

    if(isalpha(ch) || ch == '_'){
        return reconheceIdentificadoresORReservadas(ch, input, lexema);
    }

    if(strchr("< > = ! ", ch)){
        return reconheceOperadoresCompostos(ch , input, lexema );
    }

    
    if(strchr(", ; () [] {} + - * / %  & | ~", ch)){
        return reconheceCaracteresEspeciais(ch, input, lexema);
    }

    
    token = reconheceLiterais(ch, input, lexema);
    if (token != ch) { 
        return token;
    }


    if (ignorandoComentarios(ch, input)) {
        return analex(input, lexema, output); // Se detectou um comentário, continue a análise sem retornar um token
    }

    if (ch == EOF) {
        lexema[0] = '\0';
        running = 0;
        return TOKEN_EOF; 
    }


    lexema[0] = ch;
    lexema[1] = '\0';

    //erro("Erro",lexema, output);
    return TOKEN_ERROR; // Token de erro
}