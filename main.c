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
    Scope *tglobal, *tmain, *ttest;
    Symbol *main, *a, *b, *c, *a2;
    Symbol *sym;
    

    if ((st = SymbolTable_new("global")) == NULL) {
	perror("SymbolTable_new");
	exit(1);
    }

    tglobal = st->root;
    if ((main = Symbol_new()) == NULL) {
	perror("Symbol_new");
	exit(1);
    }
    main->name = "main";
    main->kind = K_FUNC;
    main->type = T_INT;
    Scope_insertsymbol(tglobal, main);

    // insert sub scopetables 
    if ((tmain = Scope_new("main")) == NULL) {
	perror("ScopeTable_new");
	exit(1);
    }
    if (Scope_insertsubtable(tglobal, tmain) == NULL) {
	perror("ScopeTable_new");
	exit(1);
    }
    if ((ttest = Scope_new("test")) == NULL) {
	perror("ScopeTable_new");
	exit(1);
    }
    if (Scope_insertsubtable(tglobal, ttest) == NULL) {
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
    Scope_insertsymbol(tmain, a);

    if ((a2 = Symbol_new()) == NULL) {
	perror("Symbol_new");
	exit(1);
    }
    a2->name = "a";
    a2->kind = K_VAR;
    a2->type = T_INT;
    if (Scope_insertsymbol(ttest, a2) == NULL) {
	perror("SymbolTable_insertsymbol");
    }

    if ((b = Symbol_new()) == NULL) {
	perror("Symbol_new");
	exit(1);
    }
    b->name = "b";
    b->kind = K_VAR;
    b->type = T_INT;
    Scope_insertsymbol(tmain, b);

    if ((c = Symbol_new()) == NULL) {
	perror("Symbol_new");
	exit(1);
    }
    c->name = "c";
    c->kind = K_VAR;
    c->type = T_INT;
    Scope_insertsymbol(tmain, c);

    if ((sym = Scope_getsymbol(tmain, "a")) == NULL) {
	perror("ScopeTable_getsymbol");
	exit(1);
    }

    Scope_print(tglobal);
    Scope_print(tmain);
    Scope_print(ttest);
    Scope_delete(tmain);
    Scope_print(tglobal);


    // BUG: Actually, deleting a functions scope table, doesn't remove the
    // associated symbols in the parent table (e.g. removing main table doesn't
    // remove the main symbol in the global table). Maybe do a  bunch of
    // functions like ScopeTable_insertsymbolfunction ScopeTable_insertsumbolvariable etc
    // to handle this cases.
    return 0;
}
