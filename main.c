#include <stdio.h>
#include <stdlib.h>

#include "symboltable.h"

extern int yylex(void);
extern FILE *yyin;
extern char *yytext;

int
main(int argc, char *argv[])
{
    /*
    FILE *fp;
    int token;

    if (argc == 2) {
	if ((fp = fopen(argv[1], "r")) == NULL) {
	    perror("fopen");
	    exit(1);
	}
	else {
	    yyin = fp;
	    while ((token = yylex())) {
		printf("text: %s,\tid: %d\n", yytext, token);
	    }
	    fclose(fp);
	}
    }
    */
    SymbolTable *st;
    ScopeTable *global;
    Symbol *sym;
    Symbol *retrieve;

    if ((st = SymbolTable_new()) == NULL) {
	perror("SymbolTable_new");
	exit(1);
    }
    global = st->scopetables;
    if ((sym = Symbol_new()) == NULL) {
	perror("Symbol_new");
	exit(1);
    }
    sym->name = "main";
    sym->kind = KIND_FUNC;
    sym->type = TYPE_INT;
    ScopeTable_insertsymbol(global, sym);

    if ((retrieve = ScopeTable_getsymbol(global, "main")) == NULL) {
	perror("ScopeTable_getsymbol");
	exit(1);
    }
    printf("name: %s, kind: %d, type: %d\n", retrieve->name, retrieve->kind, retrieve->type);
    // TODO: need to test insertion of subtables
    return 0;
}
