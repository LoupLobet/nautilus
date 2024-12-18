#include <stdio.h>


#include "symbol.h"
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
				printf("text: %s,\tid: %d\n", yytext, token);
			}
			fclose(fp);
		}
	}
	return 0;
}
