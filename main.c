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
    ScopeTable *tglobal, *tmain, *ttest;
    Symbol *main, *a, *b, *c;
    Symbol *sym;
    

    if ((st = SymbolTable_new("global")) == NULL) {
	perror("SymbolTable_new");
	exit(1);
    }

    tglobal = st->scopetables;
    if ((main = Symbol_new()) == NULL) {
	perror("Symbol_new");
	exit(1);
    }
    main->name = "main";
    main->kind = K_FUNC;
    main->type = T_INT;
    ScopeTable_insertsymbol(tglobal, main);

    // insert sub scopetables 
    if ((tmain = ScopeTable_new("main")) == NULL) {
	perror("ScopeTable_new");
	exit(1);
    }
    if (ScopeTable_insertsubtable(tglobal, tmain) == NULL) {
	perror("ScopeTable_new");
	exit(1);
    }
    if ((ttest = ScopeTable_new("test")) == NULL) {
	perror("ScopeTable_new");
	exit(1);
    }
    if (ScopeTable_insertsubtable(tglobal, ttest) == NULL) {
	perror("ScopeTable_new");
	exit(1);
    }
    

    if ((a = Symbol_new()) == NULL) {
	perror("Symbol_new");
	exit(1);
    }
    a->name = "a";
    a->kind = K_VAR;
    a->type = T_INT;
    ScopeTable_insertsymbol(tmain, a);

    if ((b = Symbol_new()) == NULL) {
	perror("Symbol_new");
	exit(1);
    }
    b->name = "b";
    b->kind = K_VAR;
    b->type = T_INT;
    ScopeTable_insertsymbol(tmain, b);

    if ((c = Symbol_new()) == NULL) {
	perror("Symbol_new");
	exit(1);
    }
    c->name = "c";
    c->kind = K_VAR;
    c->type = T_INT;
    ScopeTable_insertsymbol(tmain, c);

    if ((sym = ScopeTable_getsymbol(tmain, "a")) == NULL) {
	perror("ScopeTable_getsymbol");
	exit(1);
    }

    ScopeTable_print(tglobal);
    ScopeTable_print(tmain);
    ScopeTable_print(ttest);
    // TODO: need to check inserting already existing symbol returns and error
    return 0;
}
