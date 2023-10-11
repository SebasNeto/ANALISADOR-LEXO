#include <stdio.h>

extern int yyparse(void);
extern int yydebug;

int main(int argc, char **argv) {
    int yydebug = 1;
    yyparse();
    return 0;
}
