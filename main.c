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
	printf(">>>>>>> %d\n", Scope_delsymbol(root, "main"));
	Scope_print(root);

	// BUG: Scope_del behaviour isn't clear enough, maybe it would be better to clearly
	// split Scope_del(), and Scope_delsymbol() (K_FUNC case), to allow better modularity
	// in the functions usage (i.e. do not link scope deletion and function symbol deletion
	// --- keeping them as two separated operations, because we can imagine that symbol can
	// be deleted without deleting his scope, ... I don't know ...
	return 0;

}
