%{
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "scanner.h"

int column = 0;
int line = 0;

YYPOS yypos;
YYVAL yyval;

int
yyerror(const char *s)
{
	printf("error: %s\n", s);
	return 0;
}

long
estrtol(const char *nptr, int base)
{
	char *endptr = NULL;
	errno = 0;
	long out;

	out = strtol(nptr, &endptr, base);
	if ((endptr == nptr) || ((out == LONG_MAX || out == LONG_MIN)
	&& errno == ERANGE)) {
		return LONG_MAX;
	}
	return out;
}

double
estrtod(const char *nptr)
{
	char *endptr = NULL;
	errno = 0;
	double out;

	out = strtod(nptr, &endptr);
	if ((endptr == nptr) || ((out == HUGE_VAL || out == 0 - HUGE_VAL)
	&& errno == ERANGE)) {
		return HUGE_VAL;
	}
	return out;
}

%}

%option yylineno
%option outfile = "scanner.c"
%option nounput
%option noinput
%option noyywrap

DIGIT [0-9]
%%
[ \t]+			column += yyleng;

\. 			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_DOT;
			}
;			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_SEMICOLON;
			}
=			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_EQUAL;
			}
\{			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_LEFTCURLY;
			}
\}			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_RIGHTCURLY;
			}
\(			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_LEFTBRACE;
			}
\)			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_RIGHTBRACE;
			}
\[			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_LEFTBRACKET;
			}
\]			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_RIGHTBRACKET;
			}
,			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_COMMA;
			}
\<			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_LESSTHAN;
			}
\>			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_GREATERTHAN;
			}
\+			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_ADD;
			}
\-			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_SUB;
			}
\*			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_STAR;
			}
\/			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_DIV;
			}
if			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_IF;
			}
else			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_ELSE;
			}
"!="			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_NOTEQUAL;
			}
"||"			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_OR;
			}
"&&"			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_AND;
			}
!			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_NOT;
			}
\%			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_MOD;
			}
while			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_WHILE;
			}
for			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_FOR;
			}
return			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_RETURN;
			}
func			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_FUNCTION;
			}
int			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_TYPEINT;
			}
float			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_TYPEFLOAT;
			}
string			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_TYPESTRING;
			}
[0-9]+			{
				/* litteral integer constant */
				long val;
				
				if ((val = estrtol(yytext, 10)) == LONG_MAX)
					yyerror("Integer out of range");
				yyval.ival = val;
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_INTCONST;
			}
(({DIGIT}+\.|\.{DIGIT}+|{DIGIT}+\.{DIGIT}+)(e[+-]?{DIGIT}+)?)|({DIGIT}+e[+-]?{DIGIT}+) {
				/* litteral integer constant */
				double val;

				if ((val = estrtod(yytext)) == HUGE_VAL)
					yyerror("Floating point number out of range");
				yyval.dval = val;
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
                		column += yyleng;
				return L_REALCONST;
        		}

\"([^\"\n]|\"\")*[\"\n]	{
				/* litteral string constant */
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				if (strchr(yytext, '\n')) {
					yyerror("Newline in string");
					column = 0;
				} else {
					/* here need to insert string in symbol table */
					return L_STRINGCONST;
				}
			}
[a-zA-Z_][a-zA-Z0-9_]*	{
				/* here need to insert identifier in symbol table */
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return L_IDENTIFIER;
			}
\n			{
				column = 0;
			}
.			yyerror("Illegal symbol");

<<EOF>>			yyterminate();

