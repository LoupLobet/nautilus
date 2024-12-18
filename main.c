#include <stdio.h>
#include <stdlib.h>

#include "symbol.h"

extern int yylex(void);
extern FILE *yyin;
extern char *yytext;

int main(int argc, char *argv[]) {
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
	ScopeTree *st;
	Scope *root;
	Symbol *symmain, *symargc, *symargv;
	Symbol *symage;

	if ((root = Scope_new("global")) == NULL) {
		perror("Scope_new");
		exit(1);
	}

	if ((st = ScopeTree_new(root)) == NULL) {
		perror("ScopeTree_new");
		exit(1);
	}

	// create a main function

	if ((symage = Symbol_newvariable("age", T_INT)) == NULL) {
		perror("Symbol_newvariable");
		exit(1);
	}
	if (Scope_addsymbol(root, symage) == NULL) {
		perror("Symbol_newvariable");
		exit(1);
	}

	if ((symargc = Symbol_newvariable("argc", T_INT)) == NULL) {
		perror("Symbol_newvariable");
		exit(1);
	}
	if ((symargv = Symbol_newvariable("argv", T_INT)) == NULL) {
		perror("Symbol_newvariable");
		exit(1);
	}
	symargc->next = symargv;
	if ((symmain = Symbol_newfunction("main", T_INT, symargc, 2)) == NULL) {
		perror("Symbol_newfunction");
		exit(1);
	}
	Scope_addsymbol(root, symmain);


	Scope_print(root);
	Scope_print(root->childs);
	Scope_delsymbol(root, "main");
	Scope_print(root);

	return 0;

}
