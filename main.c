#include <stdio.h>


#include "symbol.h"
#include "scanner.h"
#include "util.h"

extern int yylex(void);
extern FILE *yyin;
extern char *yytext;

int main(int argc, char *argv[]) {
	FILE *fp;
	int token;

	if (argc == 2) {
		if ((fp = fopen(argv[1], "r")) == NULL)
			error("fopen");
		else {
			yyin = fp;
			while ((token = yylex())) {
				if (token == L_CHARCONST)
					printf("text: %s,\tid: %d\tival: '%c'\n", yytext, token, yyval.cval);
				else
					printf("text: %s,\tid: %d\n", yytext, token);
			}
			fclose(fp);
		}
	}
	return 0;
}
